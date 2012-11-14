#pragma once

#include <math.h>
#include "imagescaledialog.h"
#include "aboutdialog.h"
#include "extractsequencedialog.h"
#include "customgraphicsscene.h"
#include "savedialog.h"
#include "dynamicpasschain.h"

#include <QtCore/QtGlobal>

#include <QtGui/QMainWindow>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QScrollBar>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QPen>
#include <QtGui/QSlider>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QResizeEvent>
#include <QRubberBand>
#include <QPoint>
#include <QMutex>

namespace Ui
{
    class MainWindow;
}

class Controller;
/**
 * @class MainWindow
 *
 * Fenêtre principale du programme
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    /**
     * Le controleur
     */
    Controller *controller;

    /**
     * La pixmap représentant l'image, renvoyé par le controleur lors du changement de scène
     */
    QGraphicsPixmapItem *imageItem;

    /**
     * Flottant permettant d'empécher un zoom trop puissant, ou inversement.
     */
    qreal normalFactor;

    /**
     * Dialogue demandant la taille réelle entre deux points,
     * permet de calculer l'échelle
     */
    ImageScaleDialog imageScaleDialog;

    /**
     * Boite de dialogue qui demande les paramètre d'extraction d'une vidéo
     */
    ExtractSequenceDialog extractSequenceDialog;

    /**
     * Boite de dialogue qui demande qui affiche les possibilités de sauvegarde
     */
    SaveDialog saveDialog;

    /**
     * Dialogue affichant le "A propos".
     */
    AboutDialog aboutDialog;

    /**
     * Dialogue de création de passes dynamiques
     */
    DynamicPassChain dynamicPassChainDialog;

    /**
     * Permet de synchroniser les barres de défilements quand on affiche la deuxième
     * qgraphicsview
     */
    QMutex dualScrollBarMutex;

    /**
     * Le slider permettant d'avancer ou reculer dans la vidéo
     */
    QSlider *seekBar;

    /**
     * Nécessaire pour cacher le séparateur utilisé pour le traitement simultanée de
     * plusieurs images
     */
    QAction *separator;

    /**
     * Mode actuelle de traitement d'image :
     * IMAGE, VIDEO, WEBCAM, TOUT (correspondant à aucun mode)
     */
    int mode;

    /**
     * Interface utilisateur
     */
    Ui::MainWindow *ui;

    /**
     * Ajuste la taille de l'image
     *
     * @param factor facteur d'ajustement de la taille
     */
    void scaleImage(qreal factor);

    /**
     * Mise à jour des menus en fonction des incompatibilités (zoom et échelle)
     */
    void updateActions();

public:
    /**
     * Constructeur
     *
     * @param controller Le contrôleur possédant la fenêtre
     * @param parent     Le QWidget parent
     */
    MainWindow(Controller *control, QWidget *parent = 0);

    /**
     * Destructeur
     */
    virtual ~MainWindow();

    /**
     * Surcharge de l'évenement de redimension pour
     * adapter la taille de l'image si elle doit être
     * à la taille de la fenêtre
     * @param event l'évenement de redimension
     */
    void resizeEvent(QResizeEvent *event);

    /**
     * Scène changée
     * @param cScene La nouvelle scène passée par le controleur
     * @param cImageItem la pixmap de l'image ajouté à la scène
     */
    void sceneChanged(CustomGraphicsScene* cScene, QGraphicsPixmapItem* cImageItem);

    /**
      * Scène binare changée
      * @param binaryScene La nouvelle scène passé par le controleur à secondImage
      */
    void binarySceneChanged(CustomGraphicsScene *binaryScene);

    /**
      * Affichage de l'image en noir & blanc
      */
    void showBinaryImage();

    /**
      * Cache l'image en noir & blanc
      */
    void hideBinaryImage();

    /**
      * Désactive ou active tous les boutons liés au traitement
      * @param val bouton activé ou non
      * @param type quels boutons
      */
    void resetButtons(bool val, int type);

    /**
      * Active ou désactive le bouton de selection manuel d'un cercle
      * @param val True si bouton activé, false sinon
      */
    void manualSelectionSetEnabled(bool val);

    /**
      * Active ou désactive le bouton de rectangle de selection
      * @param val True si bouton activé, false sinon
      */
    void rectangleSelectionSetEnabled(bool val);

    /**
     * Accesseur : binaryScene
     *
     * @return Scene binaire de droite
     */
    QGraphicsScene* getBinaryScene();

    void fitToWindow();

    /**
     * Rapporteur
     *
     */
    void protractor();

    /**
     * En cas de changement de méthode de détection
     *
     */
    void methodChanged(int mode);

    /**
      * Accesseur : SeekBar
      */
    QSlider* getSlider();

    /**
      * Définit si le bouton de sélection d'une zone d'interet est actif
      *
      * @param act Booleen définissant si le bouton doit être actif ou non
      */
    void setSelectionActEnabled(bool act);

public slots:
    /**
     * Evènement : clic sur le bouton Ouvrir une image
     */
    void openImage();

    /**
     * Evènement : clic sur le bouton Ouvrir plusieurs images
     */
    void openImages();

    /**
     * Evènement : clic sur le bouton Ouvrir une vidéo
     */
    void openVideo();

    /**
     * Evènement : clic sur le bouton Ouvrir la webcam
     */
    void openWebcam();

    /**
     * Evènement : clic sur le bouton Fermer
     */
    void closeCurrent();

    /**
     * Evènement : clic sur le bouton Zoom+
     */
    void zoomIn();

    /**
     * Evènement : clic sur le bouton Zoom-
     */
    void zoomOut();

    /**
     * Evènement : clic sur le bouton Zoom normal
     */
    void normalSize();

    /**
     * Evènement : click sur le bouton Zoom auto
     * @param val True si l'image doit correspondre à la taille de la fenêtre
     */
    void fitToWindow(bool val);

    /**
     * Evènement : coche l'affichage des paramètres
     * @param val True si le dock des paramètres doit être affiché
     */
    void tickParameter(bool val);

    /**
     * Rend le bouton de traitement cliquable ou non
     * @param enabled Désactive le bouton traitement
     */
    void enableProcess(bool enabled);

    /**
     * Evènement : Bouton de définition d'échelle cliquée
     */
    void imageScale();

    /**
     * Evènement : Deux points sur l'image ont été cliqué,
     * calcule l'échelle de l'image
     * @param val La distance entre les deux points
     */
    void imageScale(qreal val);

    /**
     * Evènement : clique sur le bouton de selection de zone d'intérêt
     */
    void clickZoneInteret();

    /**
     * Evènement : clique sur le bouton de déplacement avec la main
     */
    void hand();

    /**
     * Evènement : clique sur le bouton de selection manuelle
     */
    void manualSelection();

    /**
     * Evènement : clique sur le bouton de calcul de l'échelle
     */
    void showScale();

    /**
     * Evènement : clique sur le bouton de sauvegarde de séquence
     */
    void showExtract();

    /**
     * Evènement : clique sur le bouton "sauvegarder"
     */
    void showSaveDialog();

    /**
     * Evènement : déplacement de la première fenêtre en fonction de la seconde
     */
    void firstDrag();

    /**
     * Evènement : déplacement de la seconde fenêtre en fonction de la première
     */
    void secondDrag();

    /**
     * Evènement : clic sur le bouton avance rapide
     * @param val True si le bouton coché
     */
    void fast(bool val);

    /**
     * Evènement : clic sur le bouton de traitement en direct (webcam ou vidéo)
     * @param val True si le bouton coché
     */
    void processStream(bool val);

    /**
     * Evenement: Bouton de sauvegarde de séquences relaché
     */
    void saveSequencesButtonPressed();

    /**
     * Evenement: Bouton de d'importation d'un fichier de sauvegarde appuyé
     */
    void importFileButtonPressed();

    /**
     * Evenement: Erreur de traitement
     */
    void displayError(QString err);

    /**
     * Evenement: information de traitement
     */
    void displayInfo(QString title, QString info);

signals:
    /**
     * L'image à changé, envoie la QImage ouverte au controleur
     * @param image La nouvelle image
     */
    void imageChanged(QImage *image);

    /**
     * Les images ont changés, envoie la list de QImage* ouvertes au controleur
     * @param images La liste de nouvelles images
     */
    void imagesChanged(QList<QImage *> *images);

    /**
     * La vidéo à changé, envoie le nom du fichier au controleur
     * @param str La nom du fichier vidéo
     */
    void videoChanged(QString &str);

    /**
     * Le traitement doit être arrêté, et l'affichage remis à zéro
     */
    void mediaClosed();

    /**
     * Change le mode de sélection
     * @param mode SCROLLHAND : main, RUBBERHAND : souris avec rectangle de selection
     */
    void changeGraphicMode(int mode);

private slots:
    void on_extractAct_triggered();
};
