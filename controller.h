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
     * Fen�tre principale du programme
     */
    MainWindow *mainWindow;

    /**
     * Fen�tre de contr�le des param�tres
     */
    ParameterDock *parameterDock;

    /**
     * Le mode de d�tection actuel (par d�faut 0 = CUSTOM)
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
     * L'image affich�e actuellement
     */
    IplImage *image;

    /**
     * L'image filtr�e affich�e actuellement
     */
    IplImage *filteredImage;

    /**
     * Les images affich�es (en cas de traitement group�) convertit en IplImage
     */
    QList<IplImage *> images;

    /**
     * Les images affich�es (en cas de traitement group�)
     */
    QList<QImage *> *qimages;

    /**
     * Liste des passes a appliquer pour a m�thode dynamique
     */
    QList<QString> dynamicList;

    /**
     * Les r�sultats affich�es (en cas de traitement group�) : cercles, images binaris�s
     */
    QList< Result > resultList;

    /**
     * Permet de conna�tre quelle �lement de resultList modifier quand il faut copier la liste de cercles
     * voir getCircleList
     */
    int resultIndex;

    /**
     * Zone d'int�ret
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
     * Passe de conversion en noir et blanc invers�
     */
    BinaryConversionPass *binaryInverse;

    /**
     * Passe de d�tection des contours Sobel
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
     * Passe de detection des cercles personnalis�e
     */
    CustomCircleDetectionPass *custom;

    /**
     * Pointeur sur la m�thode de d�tection des cercles (Hough ou Custom)
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
     * Thread de traitement vid�o
     */
    VideoProcessor *videoProcessor;

    /**
     * True les contours sont affich�s, false sinon
     */
    bool edgeVal;

    /**
     * True la deuxi�me image (binaris�e) est affich�e, false sinon
     */
    bool binaryVal;

    /**
     * True si les cercles parfait sont affich�s, false sinon
     */
    bool circleVal;

    /**
     * True si le traitement des images se fait en direct (pour la vid�o et la webcam), false sinon
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
     * Echelle en nanom�tres, calcul� par la fonction scale
     */
    qreal nanoScale;

    /**
     * Liste des cercles r�cup�r�s apr�s un traitement pour affichage
     */
    list<Circle> circles;

    /**
     * Classe de traitement vid�o
     * (envoie les images au controleur)
     */
    VideoExtractor *video;

    /**
     * Mutex partag�e aux thread pour emp�cher le traitement ou changement
     * d'images simultan�e.
     */
    QMutex *mutex;

    /**
     * Variable de condition qui permet de r�veiller le thread de traitement
     * quand une image est � traiter
     */
    QWaitCondition *waitcond;

    /**
     * Le chemin de l'executable
     */
    QString path;

    /**
     * Map repr�sentant la factory des passes, chaque passe est associ�e
     * a une chaine de caract�re
     */
    QMap<QString, IPass*> iPassMap;

public:

    /**
     * Constructeur
     * @param path Le chemin de l'executable (n�cessaire pour ouvrir le guide utilisateur)
     */
    explicit Controller(const QString &path);

    /**
     * Destructeur
     */
    virtual ~Controller();

    /**
     * La scene contenu par ImageView (la QGraphicsView � gauche dans la MainWindow)
     * utilis� pour afficher les images non trait�s, ou les images trait�s provenant
     * de la webcam ou d'une vid�o
     */
    CustomGraphicsScene scene;

    /**
     * La scene contenu par secondImage (la QGraphicsView � droite dans la MainWindow)
     * utilis� pour afficher les images trait�s, ou le flux webcam ou vid�o
     */
    CustomGraphicsScene binaryScene;

    /**
     * Renvoie la m�thode utilis�e
     * @return HOUGH, PERSONNALISEE, GOUTTE
     */
    int  getMode();

    /**
     * Renvoie l'activation du traitement en direct
     * @return True si le traitement en direct est activ�e, false sinon
     */
    bool getStreamProcessActivated();

    /**
     * M�thode appel� lors d'un changement de m�thode
     * @param index 0 = Custom, 1 = Hough, 2 = Goutte
     */
    void methodChanged(int index);

    /**
     * Cr�e une chaine de passe correspondant � la m�thode custom
     * Gray, roi, binary
     */
    void customPassChain();

    /**
     * Cr�e une chaine de passe correspondant � la m�thode custom
     * Roi, gray, binary, gausse, binary, hough
     */
    void houghPassChain();

    /**
     * Cr�e une chaine de passe correspondant � la m�thode custom
     * Roi, binary, binary, canny, binary, angle
     */
    void dropPassChain();

    /**
     * Cr�e une chaine de passe correspondant � la m�thode
     * meanShift
     */
    void meanShiftPassChain();

    /**
     * Cr�e une chaine de passe correspondant � la m�thode
     * dynamique (DYNAMIC)
     */
    void dynamicPassChain();

    /**
     * Affiche les points pass�s en param�tres
     * @param point Pair avec un x et un y
     */
    void drawPoint(pair<int, int> point);

    /**
     * D�finit le mode de traitment, false si on utilise le curseur pour s�lectionner
     * un cercle manuellement, true si la d�tection est automatique
     * @param val True si mode automatique
     */
    void setAutoMode(bool val);

    /**
     * Met � jour les statistiques
     */
    void updateStats();

    /**
     * Recherche a quelle passe correspond une cha�ne de carat�re
     * @param str La chaine de caract�re a analyser
     */
    IPass* passFactory(const QString &str);

    /**
     * Initialisation des passes associ�es aux chaines de caract�res
     */
    void initIPassMap();
    
public slots:
    /**
     * Evenement : Lance le traitement sur l'image courante et appelle la
     * fonction de traitement actuelle ( hough, custom ou canny (goutte) )
     */
    void process();

    /**
     * Evenement : Demande un changement de position de la vid�o quand le slider
     * est boug� par l'utilisateur
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
     * Evenement : Affiche la pr�c�dente image de la liste
     */
    void previousImages();

    /**
     * Ev�nement : clic sur le bouton Play
     */
    void play();

    /**
     * Ev�nement : clic sur le bouton Stop
     */
    void pause();

    /**
     * Ev�nement : clic sur le bouton "avance rapide"
     * @param enabled True si le bouton est coch�
     */
    void fastForward(bool enabled);

    /**
     * Supprime les cercles et contours calcul�s
     */
    void cleanItems();

    /**
     * Ev�nement : image d�fini
     * @param image La nouvelle image � afficher dans la sc�ne
     */
    void loadImage(QImage* image);

    /**
     * Ev�nement : images d�finis
     * @param image Les nouvelles images � afficher dans la sc�ne
     */
    void loadImages(QList<QImage *> *images);

    /**
     * Ev�nement : vid�o d�fini
     * @param string Le nom de la nouvelle vid�o � traiter
     */
    void loadVideo(QString &string);

    /**
     * Ev�nement : dessine des cercles sur l'image
     * @param c Le cercle � dessiner
     */
    void draw(Circle c);

    /**
     * Ev�nement : affiche la fen�tre de param�tres avec le clic sur le bouton
     * @param check True si le dock est affich�
     */
    void showParameter(bool check);

    /**
     * Ev�nement : d�coche le menu quand la fen�tre de param�tres est ferm� manuellement
     * @param val True si le dock est affich�
     */
    void tickParameter(bool val);

    /**
     * Ev�nement : changement du mode (hand ou rubberBand)
     * @param mode SCROLLHAND : main, RUBBERHAND : souris avec rectangle de selection
     */
    void changeGraphicMode(int mode);

    /**
     * Modifie les parametre de la methode de hough avec les parametre de la barre de reglage.
     */
    void setParameters();

    /**
     * Mode de d�finition d'�chelle
     */
    void scaleMode();

    /**
     * Le thread a termin� son traitement, recuperation des cercles et affichage.
     *
     * @param image Image trait�e
     */
    void imageReady(IplImage *image);

    /**
     * Le thread a termin� son traitement, recuperation des cercles et affichage.
     *
     * @param image Images trait�es
     */
    void imagesReady(QList<IplImage *> *images);

    /**
     * Le thread a envoy� un nouveau frame
     *
     * @param image Nouveau frame a afficher
     */
    void nextFrame(IplImage *image);

    /**
     * Ev�nement : le controleur doit �tre remis � z�ro (suppression des scenes)
     */
    void reset();

    /**
     * Ev�nement : clic sur le bouton Ouvrir un flux vid�o
     */
    void openWebcam();

    /**
     * R�cup�re les coordonn�es de la souris.
     * @param realDist La distance r�elle entre les deux points r�cup�r�s par la fen�tre de dialogue
     */
    void scale(qreal realDist);

    /**
     * Ev�nement : s�lectionne la r�gion d'int�r�t sur l'image
     * @param r le rectangle de s�lection
     */
    void confirmROI(QRectF *r);

    /**
     * Evenement : Detection d'un seul cercle
     * @param mouse les coordonn�es de la souris
     */
    void onClick(QPointF mouse);

    /**
     * Ev�nement : afficher les contours a chang�
     * @param val True si la case Affichage de la deuxi�me image est coch�e
     */
    void binaryBox_checked(bool val);

    /**
     * Ev�nement : afficher les contours a chang�
     * * @param val True si la case Affichage des contours est coch�e
     */
    void edgeBox_checked(bool val);

    /**
     * Ev�nement : afficher les cercles
     * * @param val True si la case Affichage des cercles est coch�e
     */
    void circleBox_checked(bool val);

    /**
     * Evenement : Fermeture de l'application
     */
    void onClose();

    /**
     * Evenement : Lance le traitement sur le flux charg� ou l'arr�te
     * @param process D�fini si le traitement du flux vid�o doit �tre lanc� ou arr�t�
     */
    void processStream(bool process);

    /**
     * Evenement : Le contr�leur doit r�cup�rer les cercles (uniquement le traitement d'images en simultan�)
     */
    void getCircleList();

    /**
     * Evenement : re�oit l'angle entre 2 lignes d�finies par l'utilisateur
     * @param angle l'angle entre les 2 lignes
     **/
    void receiveAngle(float angle);

    /**
     * Evenement : l'utilisateur souhaite ouvrir le guide utilisateur.
     */
    void openHelp();

    /**
      * Envoie les param�tre d'extraction des images au video extractor
      */
    void extractSequence(int n);

    /**
      * L'utilisateur souhaite remettre la vid�o au d�but
      */
    void restartVideo();

    /**
     * Ev�nement : Clic sur le bouton "Ok" de la boite de dialogue de cr�ation
     * dynamique de chaines
     * @param list La liste des passes s�lectionn�es
     */
    void dynamicMethodSetted(QList<QString> list);

    /**
     * Ev�nement : Le bouton de sauvegarde du r�sultat est pr�ss�
     */
    void saveResultButtonPressed();

    /**
     * Ev�nement : Le bouton d'exportation de l'image r�sultat en fichier (texte) de sauvegarde est pr�ss�
     */
    void exportResultButtonPressed();

signals:

    /**
     * Ev�nement : emet un signal pour que MainWindow d�coche la case
     * @param val Valeur de la case � cocher (true si elle doit �tre coch�)
     */
    void tickShowParameterOption(bool val);

    /**
     * Indique a la vue qu'un second traitement peut �tre lanc�
     * @param enabled True si un traitement peut �tre lanc�, false si il y'en a d�j� un en cours
     */
    void processReady(bool enabled);

    /**
     * Met a jour le dock de parametre pour indiquer la nouvelle echelle.
     * @param str La distance r�el
     */
    void setScale(QString str);
};
