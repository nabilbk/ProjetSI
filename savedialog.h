#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>

namespace Ui {
    class SaveDialog;
}

/**
 * @class SaveDialog
 * @brief Boite de dialogue donnant accès aux possibilités de sauvegarde
 * @see ExtractSequenceDialog
 */
class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructeur
     */
    explicit SaveDialog(QWidget *parent = 0);

    /**
     * Destructeur
     */
    ~SaveDialog();

private:
    /**
     * Interface graphique
     */
    Ui::SaveDialog *ui;

public slots:
    /**
     * Evenement: Bouton de sauvegarde de séquences relaché
     */
    void saveSequencesButtonPressed();

    /**
     * Evenement: Bouton de sauvegarde de l'image traitée relaché
     */
    void saveResultButtonPressed();

    /**
     * Evenement: Bouton d'exportation de l'image traitée relaché
     */
    void exportResultButtonPressed();

    /**
     * Evenement: Bouton d'importation d'un fichier relaché
     */
    void importFileButtonPressed();

signals:
    /**
     * Evènement : Emet un signal pour que Mainwindows ouvre la fenêtre de sauvegarde des séquences
     */
    void saveSequences();

    /**
     * Evènement : Emet un signal pour que Mainwindows demande au controller de sauvegarder le résultat
     */
    void saveResult();

    /**
     * Evènement : Emet un signal pour que Mainwindows demande au controller d'exporter le résultat
     */
    void exportResult();

    /**
     * Evènement : Emet un signal pour que Mainwindows demande au controller d'importer un fichier texte
     */
    void importFile();
};

#endif // SAVEDIALOG_H
