#pragma once

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QList>

#include <opencv2/opencv.hpp>

#include "passchain.h"

/**
 * @class ImageProcessor
 *
 * Exécute une chaîne de passes sur une image, dans un thread séparé
 */
class ImageProcessor : public QThread
{
    Q_OBJECT

protected:
    /**
     * Chaîne de passes à appliquer à l'image
     */
    PassChain *passChain;

    /**
     * Image en entrée
     */
    IplImage *input;

    /**
     * Image en sortie
     */
    IplImage *output;

    /**
     * Images en entrée
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
     * @param passChain La chaîne de passes à appliquer
     * @param input     L'image en entrée
     */
    explicit ImageProcessor(PassChain *passChain, IplImage *input);

    /**
     * Destructeur
     */
    virtual ~ImageProcessor();

    /**
     * Accesseur : passChain
     *
     * @return La chaîne de passes
     */
    PassChain *getPassChain() const;

    /**
     * Accesseur : passChain
     *
     * @param passChain La chaîne de passes
     */
    void setPassChain(PassChain *passChain);

    /**
     * Accesseur : input
     *
     * @return L'image en entrée
     */
    IplImage *getInput() const;

    /**
     * Accesseur : input
     *
     * @param input L'image en entrée
     */
    void setInput(IplImage *input);

    /**
     * Accesseur : input
     *
     * @param input Les images en entrée
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
     * Met output à 0, nécessaire car le controleur va libérer le résultat en cas de traitement groupé
     * il ne faut donc pas le refaire dans run
     */
    void clearOutput();

    /**
     * Lance le traitement
     */
    void run();

signals:
    /**
      * Envoi l'image traitée au controleur
      * @param output L'image traitée
      */
    void finished(IplImage *output);

    /**
      * Envoi la séquence d'images traitées au controleur
      * @param outputs La liste des images traitées séquentiellements
      */
    void finished(QList<IplImage *> *outputs);

    /**
     * Le controleur doit récupérer les statistiques
     */
    void getCircleList();
};
