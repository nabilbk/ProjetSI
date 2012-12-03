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
 * Fen�tre principale du programme
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
     * La pixmap repr�sentant l'image, renvoy� par le controleur lors du changement de sc�ne
     */
    QGraphicsPixmapItem *imageItem;

    /**
     * Flottant permettant d'emp�cher un zoom trop puissant, ou inversement.
     */
    qreal normalFactor;

    /**
     * Dialogue demandant la taille r�elle entre deux points,
     * permet de calculer l'�chelle
     */
    ImageScaleDialog imageScaleDialog;

    /**
     * Boite de dialogue qui demande les param�tre d'extraction d'une vid�o
     */
    ExtractSequenceDialog extractSequenceDialog;

    /**
     * Boite de dialogue qui demande qui affiche les possibilit�s de sauvegarde
     */
    SaveDialog saveDialog;

    /**
     * Dialogue affichant le "A propos".
     */
    AboutDialog aboutDialog;

    /**
     * Dialogue de cr�ation de passes dynamiques
     */
    DynamicPassChain dynamicPassChainDialog;

    /**
     * Permet de synchroniser les barres de d�filements quand on affiche la deuxi�me
     * qgraphicsview
     */
    QMutex dualScrollBarMutex;

    /**
     * Le slider permettant d'avancer ou reculer dans la vid�o
     */
    QSlider *seekBar;

    /**
     * N�cessaire pour cacher le s�parateur utilis� pour le traitement simultan�e de
     * plusieurs images
     */
    QAction *separator;

    /**
     * Mode actuelle de traitement d'image :
     * IMAGE, VIDEO, WEBCAM, TOUT (correspondant � aucun mode)
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
     * Mise � jour des menus en fonction des incompatibilit�s (zoom et �chelle)
     */
    void updateActions();

public:
    /**
     * Constructeur
     *
     * @param controller Le contr�leur poss�dant la fen�tre
     * @param parent     Le QWidget parent
     */
    MainWindow(Controller *control, QWidget *parent = 0);

    /**
     * Destructeur
     */
    virtual ~MainWindow();

    /**
     * Surcharge de l'�venement de redimension pour
     * adapter la taille de l'image si elle doit �tre
     * � la taille de la fen�tre
     * @param event l'�venement de redimension
     */
    void resizeEvent(QResizeEvent *event);

    /**
     * Sc�ne chang�e
     * @param cScene La nouvelle sc�ne pass�e par le controleur
     * @param cImageItem la pixmap de l'image ajout� � la sc�ne
     */
    void sceneChanged(CustomGraphicsScene* cScene, QGraphicsPixmapItem* cImageItem);

    /**
      * Sc�ne binare chang�e
      * @param binaryScene La nouvelle sc�ne pass� par le controleur � secondImage
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
      * D�sactive ou active tous les boutons li�s au traitement
      * @param val bouton activ� ou non
      * @param type quels boutons
      */
    void resetButtons(bool val, int type);

    /**
      * Active ou d�sactive le bouton de selection manuel d'un cercle
      * @param val True si bouton activ�, false sinon
      */
    void manualSelectionSetEnabled(bool val);

    /**
      * Active ou d�sactive le bouton de rectangle de selection
      * @param val True si bouton activ�, false sinon
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
     * En cas de changement de m�thode de d�tection
     *
     */
    void methodChanged(int mode);

    /**
      * Accesseur : SeekBar
      */
    QSlider* getSlider();

    /**
      * D�finit si le bouton de s�lection d'une zone d'interet est actif
      *
      * @param act Booleen d�finissant si le bouton doit �tre actif ou non
      */
    void setSelectionActEnabled(bool act);

public slots:
    /**
     * Ev�nement : clic sur le bouton Ouvrir une image
     */
    void openImage();

    /**
     * Ev�nement : clic sur le bouton Ouvrir plusieurs images
     */
    void openImages();

    /**
     * Ev�nement : clic sur le bouton Ouvrir une vid�o
     */
    void openVideo();

    /**
     * Ev�nement : clic sur le bouton Ouvrir la webcam
     */
    void openWebcam();

    /**
     * Ev�nement : clic sur le bouton Fermer
     */
    void closeCurrent();

    /**
     * Ev�nement : clic sur le bouton Zoom+
     */
    void zoomIn();

    /**
     * Ev�nement : clic sur le bouton Zoom-
     */
    void zoomOut();

    /**
     * Ev�nement : clic sur le bouton Zoom normal
     */
    void normalSize();

    /**
     * Ev�nement : click sur le bouton Zoom auto
     * @param val True si l'image doit correspondre � la taille de la fen�tre
     */
    void fitToWindow(bool val);

    /**
     * Ev�nement : coche l'affichage des param�tres
     * @param val True si le dock des param�tres doit �tre affich�
     */
    void tickParameter(bool val);

    /**
     * Rend le bouton de traitement cliquable ou non
     * @param enabled D�sactive le bouton traitement
     */
    void enableProcess(bool enabled);

    /**
     * Ev�nement : Bouton de d�finition d'�chelle cliqu�e
     */
    void imageScale();

    /**
     * Ev�nement : Deux points sur l'image ont �t� cliqu�,
     * calcule l'�chelle de l'image
     * @param val La distance entre les deux points
     */
    void imageScale(qreal val);

    /**
     * Ev�nement : clique sur le bouton de selection de zone d'int�r�t
     */
    void clickZoneInteret();

    /**
     * Ev�nement : clique sur le bouton de d�placement avec la main
     */
    void hand();

    /**
     * Ev�nement : clique sur le bouton de selection manuelle
     */
    void manualSelection();

    /**
     * Ev�nement : clique sur le bouton de calcul de l'�chelle
     */
    void showScale();

    /**
     * Ev�nement : clique sur le bouton de sauvegarde de s�quence
     */
    void showExtract();

    /**
     * Ev�nement : clique sur le bouton "sauvegarder"
     */
    void showSaveDialog();

    /**
     * Ev�nement : d�placement de la premi�re fen�tre en fonction de la seconde
     */
    void firstDrag();

    /**
     * Ev�nement : d�placement de la seconde fen�tre en fonction de la premi�re
     */
    void secondDrag();

    /**
     * Ev�nement : clic sur le bouton avance rapide
     * @param val True si le bouton coch�
     */
    void fast(bool val);

    /**
     * Ev�nement : clic sur le bouton de traitement en direct (webcam ou vid�o)
     * @param val True si le bouton coch�
     */
    void processStream(bool val);

    /**
     * Evenement: Bouton de sauvegarde de s�quences relach�
     */
    void saveSequencesButtonPressed();

    /**
     * Evenement: Bouton de d'importation d'un fichier de sauvegarde appuy�
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
     * L'image � chang�, envoie la QImage ouverte au controleur
     * @param image La nouvelle image
     */
    void imageChanged(QImage *image);

    /**
     * Les images ont chang�s, envoie la list de QImage* ouvertes au controleur
     * @param images La liste de nouvelles images
     */
    void imagesChanged(QList<QImage *> *images);

    /**
     * La vid�o � chang�, envoie le nom du fichier au controleur
     * @param str La nom du fichier vid�o
     */
    void videoChanged(QString &str);

    /**
     * Le traitement doit �tre arr�t�, et l'affichage remis � z�ro
     */
    void mediaClosed();

    /**
     * Change le mode de s�lection
     * @param mode SCROLLHAND : main, RUBBERHAND : souris avec rectangle de selection
     */
    void changeGraphicMode(int mode);

private slots:
    void on_extractAct_triggered();
};
