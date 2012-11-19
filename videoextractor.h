#pragma once

#include <QObject>
#include <iostream>
#include <opencv2/opencv.hpp>
//#include <opencv/cxcore.h>
//#include <opencv/highgui.h>
#include <QRegExp>

using namespace std;
using namespace cv;

/**
  * @class VideoExtractor
  *
  * Permet d'extraire des images depuis un fichier vidéo ou un flux vidéo
  * tel une webcam
  */
class VideoExtractor : public QObject
{
    Q_OBJECT

protected:
    /**
     * Le flux vidéo
     */
    CvCapture *video;

    /**
     * Frame actuelle
     */
    IplImage *frame;

    /**
     * Expression régulière définissant le format utilisé
     * pour atteindre une trame
     */
    QRegExp timeFormat;

    /**
     * Indicateur de mode (webcam)
     *
     * @see isWebcam()
     */
    bool webcam;

    /**
      * Nombre d'images restant à extraire de la vidéo
      * Fixé à -2 pour une extraction continue.
      * 0 par défaut
      */
    int extractSequence;

    /**
      * Frames passées depuis le début de la vidéo ou depuis la remise a 0 de la vidéo
      */
    int framesRetrieved;

public:
    /**
      * Nom du fichier vidéo ouvert
      */
    QString fileName;

    /**
     * Constructeur
     */
    explicit VideoExtractor();

    /**
      * Constructeur
      *
      * @param videoPath Chemin vers un fichier vidéo
      */
    explicit VideoExtractor(const string &videoPath);

    /**
     * Destructeur
     */
    virtual ~VideoExtractor();

    /**
      * Operateur de cast, effectue le conversion entre
      * Un VideoExtractor et une IplImage, concrétement
      * retourne la frame actuelle.
      */
    operator IplImage*();

    /**
      * Trame suivante du flux vidéo
      *
      * @param timeToWait Temps d'attente pour passer à la trame suivant (pas d'attente par défaut)
      * @return La trame actuelle
      */
    IplImage* nextFrame(int timeToWait = -1);

    /**
      * Charge un flux vidéo
      *
      * @param videoPath Chemin vers un fichier vidéo
      * @return Code de retour, erreur si négatif
      */
    int loadVideo(const string &videoPath);

    /**
      * Ouvre le flux vidéo depuis une webcam
      *
      * @return Code de retour, erreur si négatif
      */
    int loadDirectStream();

    /**
      * Détermine si le flux est arrivé à la fin
      *
      * @return Vrai si on est la fin du flux, faux sinon
      */
    bool isOver();

    /**
      * Avance la position dans le flux
      *
      * @param frameNumber Nombre de trames à sauter
      * @return La nouvelle trame
      */
    IplImage* advance(int frameNumber);

    /**
      * Déplace le flux au temps indiqué
      * Cette méthode est ignorée en mode stream
      *
      * @param ms Position dans le flux vidéo, en millisecondes
      */
    void goTo(int ms);

    /**
      * Déplace le flux au temps indiqué
      * Cette méthode est ignorée en mode stream
      *
      * @param time Position dans le flux vidéo, sous la forme hh:mm:ss
      */
    void goTo(string time);

    /**
     * Détermine si le mode est webcam
     *
     * @return Vrai si on effectue une lecture sur la webcam , faux si non.
     */
    bool isWebcam();

    /**
      * Arête l'action d'enregistrement courante
      */
    void stopRecording();

    /**
      * Ordonne au video extractor d'oublier les ressources qu'il utilise
      * (Libère la vidéo, mais pas la frame.)
      */
    void terminate();

    /**
      * Extrait la frame courante jpg dans un dossier
      */
    void saveFrame();

    /**
      * Définit le nombre d'images à extraire
      *
      * @param n Le nombre d'images à extraire, -2 si toutes les images doivent être extraites
      */
    void extractImagesFromVideo(int n);

    /**
      * Remet la vidéo au début
      *
      * @param method 0 Relachement de la vidéo actuelle et recréation du contexte de la vidéo
      *               1 Methode goTo(0)
      */
    void restartVideo(int method = 1);

public slots:
    /**
      * Slot actionné lorsque la slidebar est modifiée
      *
      * @param val La nouvelle valeur de la position (0 - 100)
      */
    void goToPosition(int val);

signals:
    /**
      * Signal émis lors du changement de position de la vidéo
      *
      * @param val La nouvelle valeur de la position (0 - 100)
      */
    void newVideoPosition(int val);

    void info(QString, QString);
};
