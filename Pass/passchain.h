#pragma once

#include <list>

#include <opencv2/opencv.hpp>

#include "ipass.h"

using namespace std;

/**
 * @brief
    Un PassChain est un traitement composée de plusieurs sous-traitements (passes, pass en anglais).
    Ces sous-traitements sont modélisés ici par la classe IPass.
 */
class PassChain
{
protected:
    /** @brief Ensemble de IPass */
    typedef std::list<IPass *> ListPass;

    /** @brief Liste des passes à exécuter lors du traitement de l'image
     */
    ListPass passes;

public:
    virtual ~PassChain();

    /**
     * @brief Retourne le nombre de passes du traitement.
     @return int : nombre de passes du traitement.
     */
    int size(void) const;

    /**
     * Vide la liste de passes
     */
    void clear();

    /**
     * Ajoute une passe à la liste des passes à exécuter
     *
     * @param pass La passe à ajouter
     */
    void add(IPass *pass);

    /**
     * Traite une image en suivant la chaîne de passes
     * L'image résultante doit être libérée par l'appelant
     * avec cvReleaseImage
     *
     * @param source L'image à traiter
     * @return L'image traitée
     */
    virtual IplImage *process(const IplImage *source);
};
