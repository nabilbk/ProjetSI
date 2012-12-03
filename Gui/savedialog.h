#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>

namespace Ui {
    class SaveDialog;
}

/**
 * @class SaveDialog
 * @brief Boite de dialogue donnant acc�s aux possibilit�s de sauvegarde
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
     * Evenement: Bouton de sauvegarde de s�quences relach�
     */
    void saveSequencesButtonPressed();

    /**
     * Evenement: Bouton de sauvegarde de l'image trait�e relach�
     */
    void saveResultButtonPressed();

    /**
     * Evenement: Bouton d'exportation de l'image trait�e relach�
     */
    void exportResultButtonPressed();

    /**
     * Evenement: Bouton d'importation d'un fichier relach�
     */
    void importFileButtonPressed();

signals:
    /**
     * Ev�nement : Emet un signal pour que Mainwindows ouvre la fen�tre de sauvegarde des s�quences
     */
    void saveSequences();

    /**
     * Ev�nement : Emet un signal pour que Mainwindows demande au controller de sauvegarder le r�sultat
     */
    void saveResult();

    /**
     * Ev�nement : Emet un signal pour que Mainwindows demande au controller d'exporter le r�sultat
     */
    void exportResult();

    /**
     * Ev�nement : Emet un signal pour que Mainwindows demande au controller d'importer un fichier texte
     */
    void importFile();
};

#endif // SAVEDIALOG_H
