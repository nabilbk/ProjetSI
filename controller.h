#pragma once

#include "Gui/mainwindow.h"
#include "Gui/parameterdock.h"
#include "Pass/houghcircledetectionpass.h"
#include "Pass/customcircledetectionpass.h"
#include "Pass/angledetectionpass.h"
#include "Pass/cannyfilterpass.h"
#include "Pass/binaryconversionpass.h"
#include "Pass/graylevelpass.h"
#include "Pass/gaussianblurpass.h"
#include "Pass/meanshiftfilterpass.h"
#include "Pass/sobelfilterpass.h"
#include "imageprocessor.h"
#include "imageconverter.h"
#include "Pass/angledetectionpass.h"

#include "videoextractor.h"
#include "videoprocessor.h"
#include "Pass/roipass.h"
#include "Gui/customgraphicsscene.h"

#include <QObject>
#include <QDesktopServices>
#include <QUrl>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QtGui/QPen>
#include <sstream>


enum
{
    TOUT,
    IMAGE,
    VIDEO,
    WEBCAM,
    IMAGES
};

enum
{
    PERSONNALISEE,
    HOUGH,
    GOUTTE,
    RAPPORTEUR,
    MEANSHIFT,
    DYNAMIC
};

struct Result
{
    IplImage *result;
    list<Circle> circles;

};

class Controller : public QObject
{
    Q_OBJECT

protected:
    /**
     * Fenêtre principale du programme
     */
    MainWindow *mainWindow;

    /**
     * Fenêtre de contrôle des paramètres
     */
    ParameterDock *parameterDock;

    /**
     * Le mode de détection actuel (par défaut 0 = CUSTOM)
     */
    int mode;

    /**
     * Style de dessin des cercles
     */
    QPen redPen;

    /**
     * Style de dessin des contours
     */
    QPen bluePen;

    /**
     * L'image affichée actuellement
     */
    IplImage *image;

    /**
     * L'image filtrée affichée actuellement
     */
    IplImage *filteredImage;

    /**
     * Les images affichées (en cas de traitement groupé) convertit en IplImage
     */
    QList<IplImage *> images;

    /**
     * Les images affichées (en cas de traitement groupé)
     */
    QList<QImage *> *qimages;

    /**
     * Liste des passes a appliquer pour a méthode dynamique
     */
    QList<QString> dynamicList;

    /**
     * Les résultats affichées (en cas de traitement groupé) : cercles, images binarisés
     */
    QList< Result > resultList;

    /**
     * Permet de connaître quelle élement de resultList modifier quand il faut copier la liste de cercles
     * voir getCircleList
     */
    int resultIndex;

    /**
     * Zone d'intéret
     */
    ROIPass *roi;

    /**
     * Passe de niveau de gris
     */
    GrayLevelPass *grayLevel;

    /**
     * Passe de conversion en noir et blanc
     */
    BinaryConversionPass *binary;

    /**
     * Passe de conversion en noir et blanc inversé
     */
    BinaryConversionPass *binaryInverse;

    /**
     * Passe de détection des contours Sobel
     */
    SobelFilterPass *sobel;

    /**
     * Applique meanshift a l'image
     */
    MeanShiftFilterPass *meanshift;

    /**
     * Passe flou
     */
    BlurPass *gaussian;

    /**
     * Passe de detection de gouttes
     */
    CannyFilterPass *canny;

    /**
     * Passe de detection des cercles personnalisée
     */
    CustomCircleDetectionPass *custom;

    /**
     * Pointeur sur la méthode de détection des cercles (Hough ou Custom)
     */
    IPass *currentMethod;

    /**
     * Passe de detection des cercles de Hough
     */
    HoughCircleDetectionPass *hough;

    /**
     * Passe de detection des angles
     */
    AngleDetectionPass *angle;

    /**
     * Chaine de passe, permet d'enchainer les passes
     */
    PassChain passChain;

    /**
     * Thread de traitement d'une image
     */
    ImageProcessor *imgProcessor;

    /**
     * Thread de traitement vidéo
     */
    VideoProcessor *videoProcessor;

    /**
     * True les contours sont affichés, false sinon
     */
    bool edgeVal;

    /**
     * True la deuxième image (binarisée) est affichée, false sinon
     */
    bool binaryVal;

    /**
     * True si les cercles parfait sont affichés, false sinon
     */
    bool circleVal;

    /**
     * True si le traitement des images se fait en direct (pour la vidéo et la webcam), false sinon
     */
    bool streamProcessActivated;

    /**
     * True si on ferme le programme, false sinon
     */
    bool closed;

    /**
     * Type de traitement actuels (IMAGE, WEBCAM, VIDEO)
     */
    unsigned currentView;

    /**
     * Liste des contours, permet de faire un groupe
     */
    QList<QGraphicsItem *> edgeList;

    /**
     * Liste des cercles, permet de faire un groupe
     */
    QList<QGraphicsItem *> circleList;

    /**
     * Groupe des contours, permet de les afficher ou non
     */
    QGraphicsItemGroup *edgeGroup;

    /**
     * Groupe des cercles, permet de les afficher ou non
     */
    QGraphicsItemGroup *circleGroup;

    /**
     *
     */
    short edgeFilter;

    /**
     * Echelle en nanomètres, calculé par la fonction scale
     */
    qreal nanoScale;

    /**
     * Liste des cercles récupérés après un traitement pour affichage
     */
    list<Circle> circles;

    /**
     * Classe de traitement vidéo
     * (envoie les images au controleur)
     */
    VideoExtractor *video;

    /**
     * Mutex partagée aux thread pour empécher le traitement ou changement
     * d'images simultanée.
     */
    QMutex *mutex;

    /**
     * Variable de condition qui permet de réveiller le thread de traitement
     * quand une image est à traiter
     */
    QWaitCondition *waitcond;

    /**
     * Le chemin de l'executable
     */
    QString path;

    /**
     * Map représentant la factory des passes, chaque passe est associée
     * a une chaine de caractère
     */
    QMap<QString, IPass*> iPassMap;

public:

    /**
     * Constructeur
     * @param path Le chemin de l'executable (nécessaire pour ouvrir le guide utilisateur)
     */
    explicit Controller(const QString &path);

    /**
     * Destructeur
     */
    virtual ~Controller();

    /**
     * La scene contenu par ImageView (la QGraphicsView à gauche dans la MainWindow)
     * utilisé pour afficher les images non traités, ou les images traités provenant
     * de la webcam ou d'une vidéo
     */
    CustomGraphicsScene scene;

    /**
     * La scene contenu par secondImage (la QGraphicsView à droite dans la MainWindow)
     * utilisé pour afficher les images traités, ou le flux webcam ou vidéo
     */
    CustomGraphicsScene binaryScene;

    /**
     * Renvoie la méthode utilisée
     * @return HOUGH, PERSONNALISEE, GOUTTE
     */
    int  getMode();

    /**
     * Renvoie l'activation du traitement en direct
     * @return True si le traitement en direct est activée, false sinon
     */
    bool getStreamProcessActivated();

    /**
     * Méthode appelé lors d'un changement de méthode
     * @param index 0 = Custom, 1 = Hough, 2 = Goutte
     */
    void methodChanged(int index);

    /**
     * Crée une chaine de passe correspondant à la méthode custom
     * Gray, roi, binary
     */
    void customPassChain();

    /**
     * Crée une chaine de passe correspondant à la méthode custom
     * Roi, gray, binary, gausse, binary, hough
     */
    void houghPassChain();

    /**
     * Crée une chaine de passe correspondant à la méthode custom
     * Roi, binary, binary, canny, binary, angle
     */
    void dropPassChain();

    /**
     * Crée une chaine de passe correspondant à la méthode
     * meanShift
     */
    void meanShiftPassChain();

    /**
     * Crée une chaine de passe correspondant à la méthode
     * dynamique (DYNAMIC)
     */
    void dynamicPassChain();

    /**
     * Affiche les points passés en paramètres
     * @param point Pair avec un x et un y
     */
    void drawPoint(pair<int, int> point);

    /**
     * Définit le mode de traitment, false si on utilise le curseur pour sélectionner
     * un cercle manuellement, true si la détection est automatique
     * @param val True si mode automatique
     */
    void setAutoMode(bool val);

    /**
     * Met à jour les statistiques
     */
    void updateStats();

    /**
     * Recherche a quelle passe correspond une chaîne de caratère
     * @param str La chaine de caractère a analyser
     */
    IPass* passFactory(const QString &str);

    /**
     * Initialisation des passes associées aux chaines de caractères
     */
    void initIPassMap();
    
public slots:
    /**
     * Evenement : Lance le traitement sur l'image courante et appelle la
     * fonction de traitement actuelle ( hough, custom ou canny (goutte) )
     */
    void process();

    /**
     * Evenement : Demande un changement de position de la vidéo quand le slider
     * est bougé par l'utilisateur
     */
    void sliderMoved();

    /**
     * Evenement : Lance le traitement sur toute les images et appelle la
     * fonction de traitement actuelle ( hough, custom ou canny (goutte) )
     */
    void processBatch();

    /**
     * Evenement : Affiche la prochaine image de la liste
     */
    void nextImages();

    /**
     * Evenement : Affiche la précédente image de la liste
     */
    void previousImages();

    /**
     * Evènement : clic sur le bouton Play
     */
    void play();

    /**
     * Evènement : clic sur le bouton Stop
     */
    void pause();

    /**
     * Evènement : clic sur le bouton "avance rapide"
     * @param enabled True si le bouton est coché
     */
    void fastForward(bool enabled);

    /**
     * Supprime les cercles et contours calculés
     */
    void cleanItems();

    /**
     * Evènement : image défini
     * @param image La nouvelle image à afficher dans la scène
     */
    void loadImage(QImage* image);

    /**
     * Evènement : images définis
     * @param image Les nouvelles images à afficher dans la scène
     */
    void loadImages(QList<QImage *> *images);

    /**
     * Evènement : vidéo défini
     * @param string Le nom de la nouvelle vidéo à traiter
     */
    void loadVideo(QString &string);

    /**
     * Evènement : dessine des cercles sur l'image
     * @param c Le cercle à dessiner
     */
    void draw(Circle c);

    /**
     * Evènement : affiche la fenêtre de paramètres avec le clic sur le bouton
     * @param check True si le dock est affiché
     */
    void showParameter(bool check);

    /**
     * Evènement : décoche le menu quand la fenêtre de paramètres est fermé manuellement
     * @param val True si le dock est affiché
     */
    void tickParameter(bool val);

    /**
     * Evènement : changement du mode (hand ou rubberBand)
     * @param mode SCROLLHAND : main, RUBBERHAND : souris avec rectangle de selection
     */
    void changeGraphicMode(int mode);

    /**
     * Modifie les parametre de la methode de hough avec les parametre de la barre de reglage.
     */
    void setParameters();

    /**
     * Mode de définition d'échelle
     */
    void scaleMode();

    /**
     * Le thread a terminé son traitement, recuperation des cercles et affichage.
     *
     * @param image Image traitée
     */
    void imageReady(IplImage *image);

    /**
     * Le thread a terminé son traitement, recuperation des cercles et affichage.
     *
     * @param image Images traitées
     */
    void imagesReady(QList<IplImage *> *images);

    /**
     * Le thread a envoyé un nouveau frame
     *
     * @param image Nouveau frame a afficher
     */
    void nextFrame(IplImage *image);

    /**
     * Evènement : le controleur doit être remis à zéro (suppression des scenes)
     */
    void reset();

    /**
     * Evènement : clic sur le bouton Ouvrir un flux vidéo
     */
    void openWebcam();

    /**
     * Récupére les coordonnées de la souris.
     * @param realDist La distance réelle entre les deux points récupérés par la fenêtre de dialogue
     */
    void scale(qreal realDist);

    /**
     * Evènement : sélectionne la région d'intérêt sur l'image
     * @param r le rectangle de sélection
     */
    void confirmROI(QRectF *r);

    /**
     * Evenement : Detection d'un seul cercle
     * @param mouse les coordonnées de la souris
     */
    void onClick(QPointF mouse);

    /**
     * Evènement : afficher les contours a changé
     * @param val True si la case Affichage de la deuxième image est cochée
     */
    void binaryBox_checked(bool val);

    /**
     * Evènement : afficher les contours a changé
     * * @param val True si la case Affichage des contours est cochée
     */
    void edgeBox_checked(bool val);

    /**
     * Evènement : afficher les cercles
     * * @param val True si la case Affichage des cercles est cochée
     */
    void circleBox_checked(bool val);

    /**
     * Evenement : Fermeture de l'application
     */
    void onClose();

    /**
     * Evenement : Lance le traitement sur le flux chargé ou l'arrête
     * @param process Défini si le traitement du flux vidéo doit être lancé ou arrêté
     */
    void processStream(bool process);

    /**
     * Evenement : Le contrôleur doit récupérer les cercles (uniquement le traitement d'images en simultané)
     */
    void getCircleList();

    /**
     * Evenement : reçoit l'angle entre 2 lignes définies par l'utilisateur
     * @param angle l'angle entre les 2 lignes
     **/
    void receiveAngle(float angle);

    /**
     * Evenement : l'utilisateur souhaite ouvrir le guide utilisateur.
     */
    void openHelp();

    /**
      * Envoie les paramètre d'extraction des images au video extractor
      */
    void extractSequence(int n);

    /**
      * L'utilisateur souhaite remettre la vidéo au début
      */
    void restartVideo();

    /**
     * Evènement : Clic sur le bouton "Ok" de la boite de dialogue de création
     * dynamique de chaines
     * @param list La liste des passes sélectionnées
     */
    void dynamicMethodSetted(QList<QString> list);

    /**
     * Evènement : Le bouton de sauvegarde du résultat est préssé
     */
    void saveResultButtonPressed();

    /**
     * Evènement : Le bouton d'exportation de l'image résultat en fichier (texte) de sauvegarde est préssé
     */
    void exportResultButtonPressed();

signals:

    /**
     * Evènement : emet un signal pour que MainWindow décoche la case
     * @param val Valeur de la case à cocher (true si elle doit être coché)
     */
    void tickShowParameterOption(bool val);

    /**
     * Indique a la vue qu'un second traitement peut être lancé
     * @param enabled True si un traitement peut être lancé, false si il y'en a déjà un en cours
     */
    void processReady(bool enabled);

    /**
     * Met a jour le dock de parametre pour indiquer la nouvelle echelle.
     * @param str La distance réel
     */
    void setScale(QString str);
};
