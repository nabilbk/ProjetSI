#ifndef EXTRACTSEQUENCEDIALOG_H
#define EXTRACTSEQUENCEDIALOG_H

#include <QDialog>

namespace Ui {
    class ExtractSequenceDialog;
}

/**
 * @class ExtractSequenceDialog
 * @brief Boite de dialogue permettant la sauvegarde des frames d'une vid�o
 */
class ExtractSequenceDialog : public QDialog
{
    Q_OBJECT

public:
    /**
      * Constructeur
      */
    explicit ExtractSequenceDialog(QWidget *parent = 0);

    /**
      * Destructeur
      */
    ~ExtractSequenceDialog();

private:
    /**
      * Interface graphique
      */
    Ui::ExtractSequenceDialog *ui;

public slots:
    /**
      * R�agi a la configuration de l'extraction de s�quence de l'utilisateur
      */
    void extract();

signals:
    /**
      * Met � jour le nombre d'images � extraire dans l'extracteur de vid�o
      */
    void notifieExtractSequence(int);
};

#endif // EXTRACTSEQUENCEDIALOG_H
