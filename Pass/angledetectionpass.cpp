#include "angledetectionpass.h"

#include <math.h>

AngleDetectionPass::AngleDetectionPass(int horizontalBand, int tangentSensibility)
{
   setParameters(horizontalBand, tangentSensibility);
   LineHorizontale = 0;
   LineTangente = 0;
}

AngleDetectionPass::~AngleDetectionPass()
{
    if (LineHorizontale != 0)
        delete LineHorizontale;
    if (LineTangente != 0)
        delete LineTangente;
}

void AngleDetectionPass::setHorizontalBand(int horizontalBand)
{
    this->horizontalBand = horizontalBand;
}

int AngleDetectionPass::getHorizontalBand()
{
    return horizontalBand;
}

void AngleDetectionPass::setTangentSensibility(int tangentSensibility)
{
    this->tangentSensibility = tangentSensibility;
}

int AngleDetectionPass::getTangentSensibility()
{
    return tangentSensibility;
}

void AngleDetectionPass::setParameters(int horizontalBand, int tangentSensibility)
{
    setHorizontalBand(horizontalBand);
    setTangentSensibility(tangentSensibility);
}

IplImage *AngleDetectionPass::operator()(IplImage *source)
{

    // Image temporaire utilisée pour construire notre vue binaire
    IplImage *tmp;

    //abcisse du centre de la ROI utilisé pour savoir quel angle détecter
    int xROICenter;

    // Nombre de pixels sur l'image
    long pixelCount;

    // Si une ROI est déjà définie, on récupère ses paramètres
    if (source->roi)
    {
        xOffset = source->roi->xOffset;
        yOffset = source->roi->yOffset;
        width   = source->roi->width;
        height  = source->roi->height;
        xROICenter = xOffset + (width / 2);
    }
    // Sinon, on ne travaille pas sur l'image
    else
    {
        return source;
    }

    // On va travailler sur une copie de l'image
    tmp = cvCreateImage(cvSize(width, height), source->depth, source->nChannels);
    cvCopy(source, tmp);

    pixelCount = width * height;

    // Initialiation des pixels
    pixels.resize(pixelCount);

    // On determine dans quelle partie de l'image se situe la ROI
    // afin de détecter l'angle de gauche ou l'angle de droite
    if (xROICenter <= (source->width / 2))
        dichotomie = 0;
    else
        dichotomie = 1;

    // Initialisation des groupes de pixels, à 0 par défaut
    pixelGroups.resize(pixelCount);
    fill(pixelGroups.begin(), pixelGroups.end(), 0);

    // Début de l'algorithme, (ré)initialisation


    // On calcule quels pixels sont blancs (true) ou noirs (false)
    // Notez que l'on s'intéresse justement aux pixels noirs et pas aux blancs
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            pixels[y * width + x] = (CV_IMAGE_ELEM(tmp, unsigned char, y, x) == 255);

    // Plus besoin de la copie d'image, à partir de maintenant on travaille
    // sur pixels et pixelGroups
    cvReleaseImage(&tmp);

    detect();

    return source;

}

void AngleDetectionPass::detect()
{
    horizontalLineDetection();
    tangenteLineDetection();
    angleCalcul();
}

void AngleDetectionPass::horizontalLineDetection()
{
    int dx = 0, dy;
    int yFirst = 0;
    float sigmaDy = 0;
    int yInf, ySup;
    bool outOfBand = false, flag = false;
    unsigned long i = 0;
    pixelGroups.clear();
    if (LineHorizontale != NULL)
    {
        delete LineHorizontale;
        LineHorizontale = NULL;
    }
    // si on se trouve du côté gauche de l'image
    if (!dichotomie)
    {
        // on commence par récupérer le premier pixel de la droite horizontale
        for(dy = 0; dy < height; ++dy)
        {
            // si on tombe sur un pixel noir
            if (!pixels[dy * width + dx])
            {
                // on stock la valeur
                yFirst = dy;
                // ainsi que que le pixel
                pixelGroups.push_back(dy * width + dx);
                break;
            }
            if (dy == (height - 1))
                return;
        }
        // maintenant on commence à parcourir l'image
        // Pour chaque colonne de l'image on va chercher
        // le pixel le plus proche de yFirst dans la
        // bande définie qui correspond
        yInf = yFirst - horizontalBand;
        ySup = yFirst + horizontalBand;
        for(; (dx < width && !outOfBand ); ++dx)
        {
            for(dy = yInf; (dy < ySup && !flag); ++dy)
            {
                // si on tombe sur un pixel noir
                if (dy * width + dx > 0 && !pixels[dy * width + dx])
                {
                    // on stock le pixel
                    pixelGroups.push_back(dy * width + dx);

                    // on lève le flag
                    flag = true;
                    break;
                }
            }
            if (flag)
                flag = false;
            else
                outOfBand = true;
        }
    }
    // si on se trouve du côté droit de l'image
    else
    {
        // on commence par récupérer le premier pixel de la droite horizontale
        for(dy = 0; dy < height; ++dy)
        {
            // si on tombe sur un pixel noir
            if (!pixels[dy * width + (width - 1)])
            {
                // on stock la valeur
                yFirst = dy;
                // ainsi que que le pixel
                pixelGroups.push_back(dy * width + (width -1));
                break;
            }
            if (dy == (height - 1))
                return;
        }
        // maintenant on commence à parcourir l'image
        // Pour chaque colonne de l'image on va chercher
        // le pixel le plus proche de yFirst dans la
        // bande définie qui correspond
        yInf = yFirst - horizontalBand;
        ySup = yFirst + horizontalBand;
        for(dx = (width - 1); (dx >= 0 && !outOfBand ); --dx)
        {
            for(dy = yInf; (dy <= ySup && !flag); ++dy)
            {
                // si on tombe sur un pixel noir
                if (!pixels[dy * width + dx])
                {
                    // on stock le pixel
                    pixelGroups.push_back(dy * width + dx);

                    // on lève le flag
                    flag = true;
                    break;
                }
            }
            if (flag)
                flag = false;
            else
                outOfBand = true;
        }
    }
    // on retire les horizontalBand pixels du groupe
    // afin de ne pas fausser les résultats
    for(dy = 0; dy < horizontalBand; ++dy)
        pixelGroups.pop_back();

    // et on calcule la somme de dy
    for(i = 0; i < pixelGroups.size(); ++i)
        sigmaDy += (float) ((pixelGroups[i] - 1) / width);

    // >=======================================================<


    // on va calculer l'equation de la droite tangente. Pour cela on prends 2 points.
    // le premier, le point d'origine (x, y) avec x = xOffset + xFirst et y = (pixelGroups[0] -1) / width
    // le deuxieme point sera la moyene des points trouvé ci dessus. On le note (x', y')
    // on a:
    // | ( y = ax + b )
    // | ( y' = ax' + b )
    // d'où :
    // | a = (y - y') / (x - x')
    // | b = (xy' - x'y) / (x - x')

    Line h;
    float x;
    if (!dichotomie)
        x = xOffset;
    else
        x = xOffset + width;
    float y = ((pixelGroups[0] -1) / width) + yOffset;
    float xPrime;
    if (!dichotomie)
        xPrime = (pixelGroups.size() / 2) + xOffset;
    else
        xPrime = (pixelGroups.size() / 2) + xOffset + width;
    float yPrime = (sigmaDy / pixelGroups.size()) + yOffset;

    h.a = (y - yPrime) / (x - xPrime);
    h.b = (x * yPrime - xPrime * y) / (x - xPrime);

    if  (!dichotomie)
        LineHorizontale = new QLineF(x, y, (x + 200), (x + 200) * h.a + h.b);
    else
        LineHorizontale = new QLineF(x, y, (x - 200), (x - 200) * h.a + h.b);
    // maintenant que l'on a tout les elements
    // on peut calculer l'équation de la droite
    /*
    horizontale.a = sigmaDy / (float) pixelGroups.size();
    horizontale.b = (sigmaDy / (float) pixelGroups.size()) + yOffset;
    */
    horizontale.a = h.a;
    horizontale.b = h.b;
}

void AngleDetectionPass::tangenteLineDetection()
{
    int xFirst, yFirst;
    int dx = 0, dy = 0;
    bool flag = false;
    float sigmaDy = 0;
    unsigned int i;
    if (LineTangente != NULL)
    {
        delete LineTangente;
        LineTangente = NULL;
    }

    // récupération de xFirst et yFirst
    if (!dichotomie)
        xFirst = pixelGroups.size();
    else
        xFirst = width - pixelGroups.size();
    yFirst = (pixelGroups[pixelGroups.size() - 1] - 1) / width;

    // on vide le groupe de pixels
    pixelGroups.clear();

    // on ajoute le premier pixel
    pixelGroups.push_back(yFirst * width + xFirst);

    // si on se trouve dans la partie gauche de l'image
    if (!dichotomie)
    {
        // on parcourt les pixels un par un
        for(dx = xFirst; dx <= (tangentSensibility + xFirst) && dx < width; ++dx)
        {
            for(dy = yFirst; (dy > 0 && !flag); --dy)
            {
                // si on tombe sur un pixel noir
                if (!pixels[dy * width + dx])
                {
                    // alors on l'ajoute au groupe
                    pixelGroups.push_back(dy * width + dx);

                    // on lève le flag
                    flag = true;
                    break;
                }
            }
            if (flag)
                flag = false;
        }
        if (dx <= (tangentSensibility +xFirst) && dx < width)
        {
            // on vide la liste
            pixelGroups.clear();
            // on ajoute le premier pixel
            pixelGroups.push_back(yFirst * width + xFirst);
            for(dx = xFirst; dx <= (tangentSensibility - xFirst) && dx > 0; --dx)
            {
                for(dy = yFirst + 1; (dy > 0 && !flag); --dy)
                {
                    // si on tombe sur un pixel noir
                    if (!pixels[dy * width + dx])
                    {
                        // alors on l'ajoute au groupe
                        pixelGroups.push_back(dy * width + dx);

                        // on lève le flag
                        flag = true;
                        break;
                    }
                }
                if (flag)
                    flag = false;
            }
        }
    }
    // si on se trouve dans la partie droite de l'image
    else
    {
        // on parcourt les pixels un par un
        for(dx = xFirst; dx <= (tangentSensibility - xFirst) && dx > 0; --dx)
        {
            for(dy = yFirst; (dy > 0 && !flag); --dy)
            {
                // si on tombe sur un pixel noir
                if (!pixels[dy * width + dx])
                {
                    // alors on l'ajoute au groupe
                    pixelGroups.push_back(dy * width + dx);

                    // on lève le flag
                    flag = true;
                    break;
                }
            }
            if (flag)
                flag = false;
        }
        if (dx <= (tangentSensibility +xFirst) && dx < width)
        {
            // on vide la liste
            pixelGroups.clear();
            // on ajoute le premier pixel
            pixelGroups.push_back(yFirst * width + xFirst);
            for(dx = xFirst; dx <= (tangentSensibility + xFirst) && dx < width; ++dx)
            {
                for(dy = yFirst + 1; (dy > 0 && !flag); --dy)
                {
                    // si on tombe sur un pixel noir
                    if (!pixels[dy * width + dx])
                    {
                        // alors on l'ajoute au groupe
                        pixelGroups.push_back(dy * width + dx);

                        // on lève le flag
                        flag = true;
                        break;
                    }
                }
                if (flag)
                    flag = false;
            }
        }
    }

    // et on calcule la somme de dy
    for(i = 0; i < pixelGroups.size(); ++i)
        sigmaDy += (float) ((pixelGroups[i] - 1) / width);

    // on va calculer l'equation de la droite tangente. Pour cela on prends 2 points.
    // le premier, le point d'origine (x, y) avec x = xOffset + xFirst et y = (pixelGroups[0] -1) / width
    // le deuxieme point sera la moyene des points trouvé ci dessus. On le note (x', y')
    // on a:
    // | ( y = ax + b )
    // | ( y' = ax' + b )
    // d'où :
    // | a = (y - y') / (x - x')
    // | b = (xy' - x'y) / (x - x')

    Line t;
    float x;
    if (!dichotomie)
        x = xOffset + xFirst;
    else
        x = xOffset + width;
    float y = ((pixelGroups[0] -1) / width) + yOffset;
    float xPrime;
    if (!dichotomie)
        xPrime = (pixelGroups.size() / 2) + xOffset + xFirst;
    else
        xPrime = (pixelGroups.size() / 2) + xOffset + width - xFirst;
    float yPrime = (sigmaDy / pixelGroups.size()) + yOffset;

    t.a = (y - yPrime) / (x - xPrime);
    t.b = (x * yPrime - xPrime * y) / (x - xPrime);

    if (!dichotomie)
        LineTangente = new QLineF(x, t.a * x + t.b, x + 200,(x+ 200) * t.a + t.b);
    else
        LineTangente = new QLineF(x, t.a * x + t.b, x - 200,(x - 200) * t.a + t.b);


    // maintenant que l'on a tout les elements
    // on peut calculer l'équation de la droite
    /*
    tangente.a = sigmaDy / (float) pixelGroups.size();
    tangente.b = (sigmaDy / (float) pixelGroups.size()) + yOffset;
    */
    /*
    LineTangente = new QLineF(xFirst + xOffset,
                             (xFirst) * t.a + t.b + yOffset,
                             70,
                             yOffset + 70 * t.a + t.b);
                             */
    tangente.a = t.a;
    tangente.b = t.b;
}

void AngleDetectionPass::angleCalcul()
{
    // pour calculer l'angle entre les deux droites
    // on va calculer l'angle entre les deux vecteurs directeurs des droites
    // soit h et t les vecteur directeurs respectifs de horizontale et tangente
    // soit nh et nt leur norme respective
    /*
    float hx, hy;
    float tx, ty;
    float nh, nt;
    float scal;
    */
    // calcul des coordonées de h et t
    /*
    hx = tx = 1;
    ty = tangente.a + tangente.b;
    hy = horizontale.a + horizontale.b;
    */

    // calcul de la norme de h et t;
    /*
    nh = std::sqrt(hx * hx + hy * hy);
    nt = std::sqrt(tx * tx + ty * ty);
    */

    // calcul du produit scalaire entre h et t
    /*
    scal = hx * tx + hy * ty;
    */

    // on a la formule : h.t = ||h|| * ||t|| * cos(h,t)
    // d'où :(h,t) = arccos(h.t / ||h|| * ||t||)
    /*
    angle = std::acos(scal / (nh * nt));
    */
    if(LineHorizontale != 0)
    {
        if (!dichotomie)
            angle = 360 - LineTangente->angleTo(*LineHorizontale);
        else
            angle = LineTangente->angleTo(*LineHorizontale);
    }
}

Line AngleDetectionPass::getHorizontaleLine()
{
    return horizontale;
}

Line AngleDetectionPass::getTangenteLine()
{
    return tangente;
}

double AngleDetectionPass::getAngle()
{
    return angle;
}

QLineF * AngleDetectionPass::getLineHorizontale()
{
    return LineHorizontale;
}

QLineF * AngleDetectionPass::getLineTangente()
{
    return LineTangente;
}
