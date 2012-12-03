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
      * Correspond aux coordonnées du premier clic de la souris quand le mode est égale à 1
      */
    QPointF first;

    /**
      * Correspond aux coordonnées du deuxième clic de la souris quand le mode est égale à 2
      */
    QPointF second;


    QPointF origin;

    /**
      * Zone d'interêt sous forme d'item d'une QGraphicScene
      */
    QGraphicsRectItem *regionOfInterest;

    /**
      * Le rectangle de la zone d'interêt
      */
    QRectF *confirmedROI;

    /**
      * Le style de dessine de la zone d'intêret
      */
    QPen pen;

    /**
     * Première composante du rapporteur manuel
     **/
    QGraphicsLineItem *FirstLine;

    /**
     * Seconde composante du rapporteur manuel
     **/
    QGraphicsLineItem *SecondLine;

    /**
     * Pinceaux utilisés pour dessiner les lignes
     **/
    QPen *movePen;
    QPen *releasePen;
    QPen *horizontalPen;

    /**
      * Mode pour récupérer les clics de souris
      * 0 : efface first et second
      * 1 : récupère le premier clic
      * 2 : récupère le deuxième clic
      * 3 : récupération terminé, emission du signal scalefinish
      *
      */
    int scaleMode;

    /**
     * Mode pour récupérer les évènements souris en mode ANGLEDETECTION
     * 0 : rien n'a encore était fait
     * 1 : la première ligne est en cours de fabriquation
     * 2 : la première ligne est terminée
     * 3 : la deuxième ligne est en cours de fabriquation
     * 4 : la deuxième ligne est terminée
     **/
    int angleMode;

    /**
      * Mode de traitement de la souris
      * Soit le mode SCROLLHAND, pour se déplacer dans les images
      * Soit le mode RUBBERBAND de sélection de région d'intérêt
      * Soit le mode MANUALSELECTION de sélection cercle par cercle
      * Soit le mode ANGLEDETECTION pour le rapporteur dynamique
      */
    int graphicMode;

    /**
      * True si il n'y a pas encore de rectangle de sélection, false si il y'en a déjà un
      */
    bool firstRec;

    /**
      * Permet de savoir si une image est présente sur la scene
      **/
    bool imageLoaded;

    /**
      * Redéfinition des fonctions liés à la souris pour capter les clics, et récupérer les rectangles
      * des sélections
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
     * Accesseur de la zone d'interêt
     */
    QGraphicsRectItem* getRegionOfInterest();

    /**
     * Supprimer la zone d'interêt
     */
    void clearRegionOfInterest();

    /**
     * Accesseur en écriture de mode
     @see mode
     */
    void setMode(int nmode);

    /**
     * Accesseur en écriture du mode de la souris (main ou rectangle)
     */
    void setGraphicMode(int mode);

    /**
     * Vide la scene et le rectangle, s'il existe
     */
    void clear();

    /**
     * Retourne le bool imageLoaded qui permet de savoir si une image est chargée dans la scène
     */
    bool isImageLoaded();

signals:
    /**
     * Emit quand on a définit l'échelle
     */
    void scaleFinish();

    /**
     * Emet le rectangle de sélection
     */
    void confirmROI(QRectF *);

    /**
     * Emit quand on est en mode manuelle pour trouver un cercle
     */
    void offset(QPointF);

    /**
     * Emit à chaque fois que l'on a 2 lignes en mode ANGLEDETECTION
     */
    void angle(float);

    /**
     * Emit lorsque l'utilisateur veut enlever les cercles
     */
    void needToCleanItems();

};
