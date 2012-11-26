#pragma once

#include <list>

#include <opencv2/opencv.hpp>

#include "ipass.h"

using namespace std;

/**
 * @brief
    Un PassChain est un traitement compos�e de plusieurs sous-traitements (passes, pass en anglais).
    Ces sous-traitements sont mod�lis�s ici par la classe IPass.
 */
class PassChain
{
protected:
    /** @brief Ensemble de IPass */
    typedef std::list<IPass *> ListPass;

    /** @brief Liste des passes � ex�cuter lors du traitement de l'image
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
     * Ajoute une passe � la liste des passes � ex�cuter
     *
     * @param pass La passe � ajouter
     */
    void add(IPass *pass);

    /**
     * Traite une image en suivant la cha�ne de passes
     * L'image r�sultante doit �tre lib�r�e par l'appelant
     * avec cvReleaseImage
     *
     * @param source L'image � traiter
     * @return L'image trait�e
     */
    virtual IplImage *process(const IplImage *source);
};
