#pragma once

#include <list>

#include <opencv2/opencv.hpp>

#include "ipass.h"

using namespace std;

/**
 * @class PassChain
 *
 * Représente une chaîne de passes
 */
class PassChain
{
protected:
    /**
     * Liste des passes à exécuter
     */
    list<IPass*> passes;

public:
    /**
     * Destructeur
     */
    virtual ~PassChain();

    /**
     * Renvoie la taille de la liste de passes
     */
    int size() const;

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
