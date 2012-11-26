#include "customcircledetectionpass.h"

#include <stdexcept>

using namespace std;

CustomCircleDetectionPass::CustomCircleDetectionPass(int minPoints, double minRadius,
                                                     double maxRadius, bool autoDetect)
    : width(0), height(0), xOffset(0), yOffset(0)
{
    setParameters(minPoints, minRadius, maxRadius, autoDetect);
}

CustomCircleDetectionPass::~CustomCircleDetectionPass()
{
}

int CustomCircleDetectionPass::getMinPoints() const
{
    return minPoints;
}

void CustomCircleDetectionPass::setMinPoints(int minPoints)
{
    this->minPoints = minPoints;
}

double CustomCircleDetectionPass::getMinRadius() const
{
    return minRadius;
}

void CustomCircleDetectionPass::setMinRadius(double minRadius)
{
    this->minRadius = minRadius;
}

double CustomCircleDetectionPass::getMaxRadius() const
{
    return maxRadius;
}

void CustomCircleDetectionPass::setMaxRadius(double maxRadius)
{
    this->maxRadius = maxRadius;
}

bool CustomCircleDetectionPass::getAutoDetect() const
{
    return autoDetect;
}

void CustomCircleDetectionPass::setAutoDetect(bool autoDetect)
{
    this->autoDetect = autoDetect;
}

void CustomCircleDetectionPass::setParameters(int minPoints, double minRadius, double maxRadius,
                                              bool autoDetect)
{
    setMinPoints(minPoints);
    setMinRadius(minRadius);
    setMaxRadius(maxRadius);
    setAutoDetect(autoDetect);
}

void CustomCircleDetectionPass::setOffset(int xOffset, int yOffset)
{
    this->xOffset = xOffset;
    this->yOffset = yOffset;
}

IplImage *CustomCircleDetectionPass::operator()(IplImage *source)
{
    // Image temporaire utilis�e pour construire notre vue binaire
    IplImage *tmp;

    // Nombre de pixels sur l'image
    long pixelCount;

    // Si une ROI est d�j� d�finie, on r�cup�re ses param�tres
    if (source->roi)
    {
        xOffset = source->roi->xOffset;
        yOffset = source->roi->yOffset;
        width   = source->roi->width;
        height  = source->roi->height;
    }
    // Sinon, on consid�re que la ROI couvre toute l'image
    else
    {
        // En mode manuel, xOffset et yOffset correspondent aux coordonn�es de d�part de la
        // recherche, il ne faut donc pas les modifier
        if (autoDetect)
        {
            xOffset = 0;
            yOffset = 0;
        }

        width   = source->width;
        height  = source->height;
    }

    // On va travailler sur une copie de l'image (redimensionn�e)
    tmp = cvCreateImage(cvSize(width, height), source->depth, source->nChannels);
    cvCopy(source, tmp);

    pixelCount = width * height;

    // Initialiation des pixels
    pixels.resize(pixelCount);

    // Initialisation des groupes de pixels, � 0 par d�faut
    pixelGroups.resize(pixelCount);
    fill(pixelGroups.begin(), pixelGroups.end(), false);

    // D�but de l'algorithme, (r�)initialisation s'il s'agit d'une d�tection automatique
    // Sinon, on conserve les cercles pr�c�dents
    if (autoDetect)
        clear();

    // On calcule quels pixels sont blancs (true) ou noirs (false)
    // Notez que l'on s'int�resse justement aux pixels noirs et pas aux blancs
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            pixels[y * width + x] = (CV_IMAGE_ELEM(tmp, unsigned char, y, x) == 255);

    // Plus besoin de la copie d'image, � partir de maintenant on travaille
    // sur pixels et pixelGroups
    cvReleaseImage(&tmp);

    if (autoDetect)
        doAutoDetection();
    else
        doManualDetection();

    return source;
}

void CustomCircleDetectionPass::doAutoDetection()
{
    // Liste (utilis�e comme une pile) contenant les points � parcourir
    list<pair<int, int> > visitMe;

    // Groupes de points d�tect�s
    map<int, list<pair<int, int> > > groups;

    // currentGroup indique dans quel groupe l'algorithme se trouve actuellement
    int currentGroup = 0;

    // Pour chaque pixel dans l'image...
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Pixel courant
            pair<int, int> currentPixel(x, y);

            // Si le pixel n'a pas d�j� �t� visit�
            // (C'est � dire s'il est noir et n'appartient � aucun groupe)
            if (nonWalked(currentPixel) && isEdge(currentPixel))
            {
                // C'est qu'on a d�couvert un nouveau groupe
                // On le cr�e et on le sauvegarde
                groups[++currentGroup] = list<pair<int, int> >();

                // On ajoute ce pixel � la liste des pixels � visiter
                visitMe.push_back(currentPixel);

                // Et tant qu'il reste des pixels � visiter, eh ben, on les visite
                while (visitMe.size())
                {
                    // R�cup�ration d'un pixel � visiter
                    // Notez que l'ordre de visite n'a absolument aucune importance
                    // On le fait ici en LIFO
                    pair<int, int> p = visitMe.back();
                    visitMe.pop_back();

                    // Marquage du pixel comme ayant �t� visit�
                    pixelGroups[p.second * width + p.first] = true;

                    // Ajout du pixel au groupe
                    groups[currentGroup].push_back(p);

                    // p0 � p7 sont les pixels adjacents au pixel courant, p0 est en haut � gauche
                    // On obtient les autres en tournant dans le sens des aiguilles d'une montre
                    pair<int, int> p0(p.first - 1, p.second - 1);
                    pair<int, int> p1(p.first    , p.second - 1);
                    pair<int, int> p2(p.first + 1, p.second - 1);
                    pair<int, int> p3(p.first - 1, p.second    );
                    pair<int, int> p4(p.first + 1, p.second    );
                    pair<int, int> p5(p.first - 1, p.second + 1);
                    pair<int, int> p6(p.first    , p.second + 1);
                    pair<int, int> p7(p.first + 1, p.second + 1);

                    // Petites optimisations
                    list<pair<int, int> >::iterator beg = visitMe.begin();
                    list<pair<int, int> >::iterator end = visitMe.end();

                    // On aurait pu faire un foreach, mais autant faire plus performant !
                    if (nonWalked(p0) && isEdge(p0) && find(beg, end, p0) == end) visitMe.push_back(p0);
                    if (nonWalked(p1) && isEdge(p1) && find(beg, end, p1) == end) visitMe.push_back(p1);
                    if (nonWalked(p2) && isEdge(p2) && find(beg, end, p2) == end) visitMe.push_back(p2);
                    if (nonWalked(p3) && isEdge(p3) && find(beg, end, p3) == end) visitMe.push_back(p3);
                    if (nonWalked(p4) && isEdge(p4) && find(beg, end, p4) == end) visitMe.push_back(p4);
                    if (nonWalked(p5) && isEdge(p5) && find(beg, end, p5) == end) visitMe.push_back(p5);
                    if (nonWalked(p6) && isEdge(p6) && find(beg, end, p6) == end) visitMe.push_back(p6);
                    if (nonWalked(p7) && isEdge(p7) && find(beg, end, p7) == end) visitMe.push_back(p7);
                }
            }
        }
    }

    // A ce stade l�, on a s�par� tous les pixels noirs de l'image en diff�rents groupes
    // On va maintenant traiter les dits groupes (statistiques, filtrage, etc)
    for (map<int, list<pair<int, int> > >::iterator beg = groups.begin(), end = groups.end();
        beg != end; ++beg)
    {
        // On ignore les ensembles de points de moins de minPoints �l�ments
        if (beg->second.size() < (unsigned int)minPoints)
            continue;

        // Le cercle que l'on va consid�rer
        Circle c(beg->second);

        // On ignore les cercles dont le rayon est en dehors de la plage qui nous int�resse
        if (c.r < minRadius || c.r > maxRadius)
            continue;

        // On ignore les cercles partiels aussi
        if (isPartial(c))
            continue;

        // On translate le centre du cercle
        c.x += xOffset;
        c.y += yOffset;

        // Puis on translate le reste des points du cercle
        for (list<pair<int, int> >::iterator beg2 = c.edge.begin(), end2 = c.edge.end(); beg2 != end2; ++beg2)
        {
            beg2->first  += xOffset;
            beg2->second += yOffset;
        }

        circles.push_back(c);
    }
}

void CustomCircleDetectionPass::doManualDetection()
{
    // Liste (utilis�e comme une pile) contenant les points � parcourir
    list<pair<int, int> > visitMe;

    // Liste des poins de contours d�tect�s
    list<pair<int, int> > edge;

    // Lorsqu'on est en mode manuel, xOffset et yOffset contiennent les coordonn�es de d�tection
    pair<int, int> currentPixel(xOffset, yOffset);

    // Ce pixel n'appartient pas � un cercle, inutile de pousuivre
    if (!nonWalked(currentPixel))
        return;

    // D�calage du pixel vers la gauche jusqu'� trouver un contour
    while (nonWalked(currentPixel) && !isEdge(currentPixel))
        --currentPixel.first;

    // On ajoute ce pixel � la liste des pixels � visiter
    visitMe.push_back(currentPixel);

    while (visitMe.size())
    {
        // R�cup�ration d'un pixel � visiter
        // Notez que l'ordre de visite n'a absolument aucune importance
        // On le fait ici en LIFO
        pair<int, int> p = visitMe.back();
        visitMe.pop_back();

        // Changement du groupe du pixel
        pixelGroups[p.second * width + p.first] = true;

        // Ajout du pixel aux points du groupe uniquement s'il fait partie du contour
        if (isEdge(p))
            edge.push_back(p);

        // Petites optimisations
        list<pair<int, int> >::iterator beg = visitMe.begin();
        list<pair<int, int> >::iterator end = visitMe.end();

        // p0 � p7 sont les pixels adjacents au pixel courant, p0 est en haut � gauche
        // On obtient les autres en tournant dans le sens des aiguilles d'une montre
        pair<int, int> p0(p.first - 1, p.second - 1);
        pair<int, int> p1(p.first    , p.second - 1);
        pair<int, int> p2(p.first + 1, p.second - 1);
        pair<int, int> p3(p.first - 1, p.second    );
        pair<int, int> p4(p.first + 1, p.second    );
        pair<int, int> p5(p.first - 1, p.second + 1);
        pair<int, int> p6(p.first    , p.second + 1);
        pair<int, int> p7(p.first + 1, p.second + 1);

        // On aurait pu faire un foreach, mais autant faire plus performant !
        if (nonWalked(p0) && isEdge(p0) && find(beg, end, p0) == end) visitMe.push_back(p0);
        if (nonWalked(p1) && isEdge(p1) && find(beg, end, p1) == end) visitMe.push_back(p1);
        if (nonWalked(p2) && isEdge(p2) && find(beg, end, p2) == end) visitMe.push_back(p2);
        if (nonWalked(p3) && isEdge(p3) && find(beg, end, p3) == end) visitMe.push_back(p3);
        if (nonWalked(p4) && isEdge(p4) && find(beg, end, p4) == end) visitMe.push_back(p4);
        if (nonWalked(p5) && isEdge(p5) && find(beg, end, p5) == end) visitMe.push_back(p5);
        if (nonWalked(p6) && isEdge(p6) && find(beg, end, p6) == end) visitMe.push_back(p6);
        if (nonWalked(p7) && isEdge(p7) && find(beg, end, p7) == end) visitMe.push_back(p7);
    }

    Circle c(edge);

    // On ignore les cercles dont le rayon est en dehors de la plage qui nous int�resse
    if (c.r < minRadius || c.r > maxRadius)
        return;

    // On ignore les cercles partiels aussi
    if (isPartial(c))
        return;

    // On n'enregistre que les cercles n'ayant pas d�j� �t� d�tect�s
    if (find(circles.begin(), circles.end(), c) == circles.end())
    {
        circles.push_back(c);
    }
}

bool CustomCircleDetectionPass::nonWalked(pair<int, int> point) const
{
    // On ne s'int�resse pas aux points en dehors de la zone d'int�r�t
    // PS : c'est le principe d'une zone d'int�r�t, en fait
    if (point.first < 0 || point.second < 0 || point.first == width || point.second == height)
        return false;

    // Sinon, on v�rifie que le pixel est � false n'appartient � aucun groupe
    return !pixels     [point.second * width + point.first] &&
           !pixelGroups[point.second * width + point.first];
}

bool CustomCircleDetectionPass::isEdge(pair<int, int> point) const
{
    // On consid�re que les extr�mit�s de la zone d'int�r�t sont des points de contour
    // C'est �videmment faux, mais vu que l'on ne peut pas en sortir de toute fa�on,
    // �a n'a aucune importance
    if (point.first == 0         || point.second == 0 ||
        point.first == width - 1 || point.second == height - 1)
        return true;

    // Le pixel est un point de contour si un de ses voisins imm�diats (gauche, haut, droite, bas) est true
    return pixels[point.second       * width + (point.first - 1)] ||
           pixels[(point.second - 1) * width +  point.first     ] ||
           pixels[point.second       * width + (point.first + 1)] ||
           pixels[(point.second + 1) * width +  point.first     ];
}

bool CustomCircleDetectionPass::isPartial(Circle c) const
{
    // Un cercle est partiel si un de ses points de contour sort de la zone d'int�r�t ou si un
    // de ses points de contour est � la limite de la zone d'int�r�t
    for (list<pair<int, int> >::iterator beg = c.edge.begin(), end = c.edge.end(); beg != end; ++beg)
    {
        // Point en dehors de la zone d'int�r�t, � gauche ou � droite
        if (beg->first  <= 0 || beg->first  >= width - 1)
            return true;

        // Point en dehors de la zone d'int�r�t, en haut ou en bas
        if (beg->second <= 0 || beg->second >= height - 1)
            return true;
    }

    return false;
}
