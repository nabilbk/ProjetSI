#ifndef EXTRACTSEQUENCEDIALOG_H
#define EXTRACTSEQUENCEDIALOG_H

#include <QDialog>

namespace Ui {
    class ExtractSequenceDialog;
}

/**
 * @class ExtractSequenceDialog
 * @brief Boite de dialogue permettant la sauvegarde des frames d'une vidéo
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
      * Réagi a la configuration de l'extraction de séquence de l'utilisateur
      */
    void extract();

signals:
    /**
      * Met à jour le nombre d'images à extraire dans l'extracteur de vidéo
      */
    void notifieExtractSequence(int);
};

#endif // EXTRACTSEQUENCEDIALOG_H
