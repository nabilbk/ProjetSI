#pragma once

#include <opencv2/opencv.hpp>

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>

#include "videoextractor.h"

/**
 * @class VideoProcessor
 *
 * Lance la lecture de la video en parallele
 */
class VideoProcessor : public QThread
{
    Q_OBJECT

protected:
    /**
     * Flux video en entrée
     */
    VideoExtractor *input;

    /**
     * Image en sortie
     */
    IplImage *output;

    /**
     * Indique si le jeux est en pause
     */
    bool paused;

    /**
     * Indique que le thread fait son premier tour de boucle
     */
    bool firstTime;

    /**
      * Mutex de traitement des images
      */
    QMutex *mutex;

    /**
      * Mutex de la pause
      */
    QMutex mutexPause;

    /**
      * Variable de conditions qui attend le réveil par le controller
      */
    QWaitCondition *waitcond;

    /**
      * Condition d'attente de la pause
      */
    QWaitCondition waitcondPause;

    /**
     * S'il est à True, arrête le run
     */
    bool isStopped;

    /**
      * Définit la vitesse de la vidéo, plus exactement le temps que
      * le videoProcessor doit attendre (en ms) avant de demander
      * la nouvelle image a l'extracteur de vidéo
      * Initialement 180ms
      */
    int forward;

public:
    /**
     * Constucteur
     *
     * @param input     L'image en entrée
     */
    explicit VideoProcessor(VideoExtractor *input, QMutex *mutex, QWaitCondition *waitcond);

    /**
     * Destructeur
     */
    virtual ~VideoProcessor();

    /**
     * Accesseur : input
     *
     * @return Le flux video en entrée
     */
    VideoExtractor *getInput() const;

    /**
     * Accesseur : input
     *
     * @param input Le flux video en entrée
     */
    void setInput(VideoExtractor *input);

    /**
     * Accesseur : output
     *
     * @return L'image en sortie
     */
    IplImage *getOutput() const;

    /**
     * Lance le traitement
     */
    void run();

    /**
     * La lecture de la vidéo n'est plus en pause
     */
    void play();

    /**
     * Si la vidéo est en pause ou non
     * @return True si la vidéo est en pause, false sinon
     */
    bool isPaused();

    /**
     * Indique si une vidéo est chargée.
     *
     * @return Vrai si un flux vidéo est chargé, faux si non
     */
    bool videoLoaded();

    /**
     * Passe le booléen stop à false pour arrêter le traitement
     */
    void stop();

    /**
      * Paramètre l'avance rapide ou non.
      *
      * @param enabled Définit si on doit être en avance rapide ou non
      */
    void setForward(bool enabled);

signals:
    /**
     * Envoi la frame suivante.
     */
    void nextFrame(IplImage *output);

public slots:
    /**
     * La lecture de la vidéo est en pause
     */
    void pause();
};
