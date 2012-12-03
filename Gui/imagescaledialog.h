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
      * R�agi a la confirmation de la mise � l'�chelle de l'utilisateur
      */
    void scale();

signals:
    /**
      * Met � jour la mise � l'�chelle dans le controleur apr�s la d�cision de l'utilisateur
      */
    void scaleSet(qreal);
};
