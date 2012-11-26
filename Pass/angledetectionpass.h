#pragma once

#include "ipass.h"

#include <vector>
#include <QLineF>

using namespace std;

/**
 * @struct Line
 * permet de stocker une droite dont l'�quation
 * est de la forme y = ax + b
 **/
struct Line
{
    /**
     * coefficient directeur de la droite
     **/
    float a;

    /**
     *  translation par rapport � l'origine
     **/
    float b;
};


/**
 * @class AngleDetectionPass
 * Detecte l'angle entre le support et la tangente
 * du bord d'une goutte.
 **/
class AngleDetectionPass : public IPass
{
private:

    /**
     * largeur de la bande pour detecter la composante horizontale
     **/
    int horizontalBand;

    /**
     * distance jusqu'� laquelle on considerera que l'on est dans la tangente
     **/
    int tangentSensibility;

protected:

    /**
     * Tableau de bool�ens repr�sentant les pixels de l'image
     * Un �l�ment est true s'il appartient au "vide" de l'image, false s'il appartient � un "segment"
     */
    vector<bool>  pixels;

    /**
     * Tableau de short indiquant l'appartenance de chaque pixel � un groupe
     * 0 signifie que le pixel n'appartient � aucun groupe
     */
    vector<int> pixelGroups;

    /**
     * Largeur de la zone d'int�r�t
     */
    int width;

    /**
     * Hauteur de la zone d'int�r�t
     */
    int height;

    /**
     * Offset en x de la zone d'int�r�t
     */
    int xOffset;

    /**
     * Offset en y de la zone d'int�r�t
     */
    int yOffset;

    /**
     * angle entre les deux droite
     **/
    double angle;

    /**
     * �quation de la droite horizontale
     **/
    Line horizontale;

    /**
     * �quation de la tangente
     **/
    Line tangente;

    /**
     * �quation de la droite horizontale
     **/
    QLineF *LineHorizontale;

    /**
     * �quation de la tangente
     **/
    QLineF *LineTangente;

    /**
     * permet de savoir si l'on veut d�tecter l'angle
     * de gauche ou l'angle de droite
     * false � gauche, true � droite
     **/
    bool dichotomie;

    /**
     * effectue la detection des droites (horizontale et tangente)
     * et calcule leur angle d'incidence.
     **/
    void detect();

    /**
     * Detecte la droite horizontale (correspond au socle)
     **/
    void horizontalLineDetection();

    /**
     * Detecte la tangente au cercle
     **/
    void tangenteLineDetection();

    /**
     * Calcule l'angle d'incidence entre l'horizontale et la tangente
     **/
    void angleCalcul();

public:
    /**
     * constructeur
     * @param horizontalBand largeur de la bande pour detecter la composante horizontale
     * @param tangentSensibility distance jusqu'� laquelle on considerera que l'on est dans la tangente
     **/
    explicit AngleDetectionPass(int horizontalBand = 5,
                       int tangentSensibility = 10);

    /**
     * Destructeur
     */
    virtual ~AngleDetectionPass();

    /**
     * s�teur
     * @param horizontalBand largeur de la bande pour detecter la composante horizontale
     **/
    void setHorizontalBand(int horizontalBand);

    /**
     * gu�teure
     * @return horizontalBand
     **/
    int getHorizontalBand();

    /**
     * cette heure
     * @param tangentSensibility distance jusqu'� laquelle on considerera que l'on est dans la tangente
     **/
    void setTangentSensibility(int tangentSensibility);

    /**
     * gay t' heureux
     * @return tangentSensibility
     **/
    int getTangentSensibility();

    /**
     * getter
     * @return horizontale la droite horizontale d�tect�e par horizontalLineDetection
     **/
    Line getHorizontaleLine();

    /**
     * getter
     * @return tangente la tangente detect�e par tangenteLineDetection
     **/
    Line getTangenteLine();

    /**
     * getter
     * @return horizontale sous forme QLineF
     **/
    QLineF * getLineHorizontale();

    /**
     * getter
     * @return tangente sous forme QLineF
     **/
    QLineF * getLineTangente();

    /**
     * getter
     * @return angle l'angle entre la tangente et l'horizontale detect�e pr�c�dement
     **/
    double getAngle();

    /**
     * Modification des param�tres
     */
    void setParameters(int horizontalBand, int tangentSensibility);

    /**
     * Impl�mentation : IPass
     */
    IplImage *operator()(IplImage *source);
};


