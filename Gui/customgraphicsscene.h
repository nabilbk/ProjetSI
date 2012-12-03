#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QtGui/QPen>
#include <QLineF>

enum MouseMode { NORMAL, SCROLLHAND, RUBBERBAND, MANUALSELECTION, ANGLEDETECTION };

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

protected:
    /**
      * Correspond aux coordonn�es du premier clic de la souris quand le mode est �gale � 1
      */
    QPointF first;

    /**
      * Correspond aux coordonn�es du deuxi�me clic de la souris quand le mode est �gale � 2
      */
    QPointF second;


    QPointF origin;

    /**
      * Zone d'inter�t sous forme d'item d'une QGraphicScene
      */
    QGraphicsRectItem *regionOfInterest;

    /**
      * Le rectangle de la zone d'inter�t
      */
    QRectF *confirmedROI;

    /**
      * Le style de dessine de la zone d'int�ret
      */
    QPen pen;

    /**
     * Premi�re composante du rapporteur manuel
     **/
    QGraphicsLineItem *FirstLine;

    /**
     * Seconde composante du rapporteur manuel
     **/
    QGraphicsLineItem *SecondLine;

    /**
     * Pinceaux utilis�s pour dessiner les lignes
     **/
    QPen *movePen;
    QPen *releasePen;
    QPen *horizontalPen;

    /**
      * Mode pour r�cup�rer les clics de souris
      * 0 : efface first et second
      * 1 : r�cup�re le premier clic
      * 2 : r�cup�re le deuxi�me clic
      * 3 : r�cup�ration termin�, emission du signal scalefinish
      *
      */
    int scaleMode;

    /**
     * Mode pour r�cup�rer les �v�nements souris en mode ANGLEDETECTION
     * 0 : rien n'a encore �tait fait
     * 1 : la premi�re ligne est en cours de fabriquation
     * 2 : la premi�re ligne est termin�e
     * 3 : la deuxi�me ligne est en cours de fabriquation
     * 4 : la deuxi�me ligne est termin�e
     **/
    int angleMode;

    /**
      * Mode de traitement de la souris
      * Soit le mode SCROLLHAND, pour se d�placer dans les images
      * Soit le mode RUBBERBAND de s�lection de r�gion d'int�r�t
      * Soit le mode MANUALSELECTION de s�lection cercle par cercle
      * Soit le mode ANGLEDETECTION pour le rapporteur dynamique
      */
    int graphicMode;

    /**
      * True si il n'y a pas encore de rectangle de s�lection, false si il y'en a d�j� un
      */
    bool firstRec;

    /**
      * Permet de savoir si une image est pr�sente sur la scene
      **/
    bool imageLoaded;

    /**
      * Red�finition des fonctions li�s � la souris pour capter les clics, et r�cup�rer les rectangles
      * des s�lections
      */
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

public:
    explicit CustomGraphicsScene(QObject *parent = 0);

    /**
     * Destructeur
     */
    virtual ~CustomGraphicsScene();

    QGraphicsPixmapItem* addPixmap (const QPixmap &pixmap);

    /**
     * Accesseur de first (premier clic de souris)
     */
    QPointF getFirst();

    /**
     * Accesseur de second (second clic de souris)
     */
    QPointF getSecond();

    /**
     * Accesseur de la zone d'inter�t
     */
    QGraphicsRectItem* getRegionOfInterest();

    /**
     * Supprimer la zone d'inter�t
     */
    void clearRegionOfInterest();

    /**
     * Accesseur en �criture de mode
     @see mode
     */
    void setMode(int nmode);

    /**
     * Accesseur en �criture du mode de la souris (main ou rectangle)
     */
    void setGraphicMode(int mode);

    /**
     * Vide la scene et le rectangle, s'il existe
     */
    void clear();

    /**
     * Retourne le bool imageLoaded qui permet de savoir si une image est charg�e dans la sc�ne
     */
    bool isImageLoaded();

signals:
    /**
     * Emit quand on a d�finit l'�chelle
     */
    void scaleFinish();

    /**
     * Emet le rectangle de s�lection
     */
    void confirmROI(QRectF *);

    /**
     * Emit quand on est en mode manuelle pour trouver un cercle
     */
    void offset(QPointF);

    /**
     * Emit � chaque fois que l'on a 2 lignes en mode ANGLEDETECTION
     */
    void angle(float);

    /**
     * Emit lorsque l'utilisateur veut enlever les cercles
     */
    void needToCleanItems();

};
