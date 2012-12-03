#pragma once

#include <QDialog>

namespace Ui
{
    class ImageScaleDialog;
}

class ImageScaleDialog : public QDialog
{
    Q_OBJECT

protected:
    /**
     * Interface graphique
     */
    Ui::ImageScaleDialog *ui;

public:
    /**
     * Constructeur
     *
     * @param parent Le QWidget parent
     */
    explicit ImageScaleDialog(QWidget *parent = 0);

    /**
     * Destructeur
     */
    virtual ~ImageScaleDialog();

public slots:
    /**
      * Réagi a la confirmation de la mise à l'échelle de l'utilisateur
      */
    void scale();

signals:
    /**
      * Met à jour la mise à l'échelle dans le controleur après la décision de l'utilisateur
      */
    void scaleSet(qreal);
};
