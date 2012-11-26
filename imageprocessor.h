#pragma once

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QList>

#include <opencv2/opencv.hpp>

#include "Pass/passchain.h"

/**
 * @class ImageProcessor
 *
 * Ex�cute une cha�ne de passes sur une image, dans un thread s�par�
 */
class ImageProcessor : public QThread
{
    Q_OBJECT

protected:
    /**
     * Cha�ne de passes � appliquer � l'image
     */
    PassChain *passChain;

    /**
     * Image en entr�e
     */
    IplImage *input;

    /**
     * Image en sortie
     */
    IplImage *output;

    /**
     * Images en entr�e
     */
    QList<IplImage *> *inputs;

    /**
     * Images en sortie
     */
    QList<IplImage *> *outputs;

public:
    /**
     * Constucteur
     *
     * @param passChain La cha�ne de passes � appliquer
     * @param input     L'image en entr�e
     */
    explicit ImageProcessor(PassChain *passChain, IplImage *input);

    /**
     * Destructeur
     */
    virtual ~ImageProcessor();

    /**
     * Accesseur : passChain
     *
     * @return La cha�ne de passes
     */
    PassChain *getPassChain() const;

    /**
     * Accesseur : passChain
     *
     * @param passChain La cha�ne de passes
     */
    void setPassChain(PassChain *passChain);

    /**
     * Accesseur : input
     *
     * @return L'image en entr�e
     */
    IplImage *getInput() const;

    /**
     * Accesseur : input
     *
     * @param input L'image en entr�e
     */
    void setInput(IplImage *input);

    /**
     * Accesseur : input
     *
     * @param input Les images en entr�e
     */
    void setInputs(QList<IplImage *> *inputs);

    /**
     * Accesseur : output
     *
     * @return L'image en sortie
     */
    IplImage *getOutput() const;

    /**
     * Accesseur : outputs
     *
     * @return L'image en sortie
     */
    QList<IplImage *> *getOutputs() const;

    /**
     * Met output � 0, n�cessaire car le controleur va lib�rer le r�sultat en cas de traitement group�
     * il ne faut donc pas le refaire dans run
     */
    void clearOutput();

    /**
     * Lance le traitement
     */
    void run();

signals:
    /**
      * Envoi l'image trait�e au controleur
      * @param output L'image trait�e
      */
    void finished(IplImage *output);

    /**
      * Envoi la s�quence d'images trait�es au controleur
      * @param outputs La liste des images trait�es s�quentiellements
      */
    void finished(QList<IplImage *> *outputs);

    /**
     * Le controleur doit r�cup�rer les statistiques
     */
    void getCircleList();
};
