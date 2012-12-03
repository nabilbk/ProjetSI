#pragma once

#include <QGraphicsView>
#include <QDragMoveEvent>
#include "customgraphicsscene.h"

/**
 * @class CustomGraphicsView
 *
 * GraphicsView notifiant des �v�nements de scroll
 */
class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
protected:
    /**
     * Indique si la CustomGraphicsView est jumel�e
     * Si true, onScroll sera g�n�r� � chaque d�filement des scrollbars
     */
    bool dualMode;

    /**
     * Permet de savoir si la touche contr�le est press� (pour le zoom avec molette)
     */
    bool ctrlPressed;

    /**
     * H�rit�
     */
    void scrollContentsBy(int dx, int dy);

    /**
     * Surcharge des m�thodes de d�tection de touches
     * pour permettre l'utilisation de maj+molette
     * pour le zoom
     */
    void wheelEvent(QWheelEvent * event);

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

public:
    /**
     * Constructeur
     *
     * @param parent Le QWidget parent
     */
    explicit CustomGraphicsView(QWidget *parent = 0);

    /**
     * Destructeur
     */
    virtual ~CustomGraphicsView();

    /**
     * Accesseur : dualMode
     *
     * @param dualMode Le mode de jumelage
     */
    void setDualMode(bool dualMode);

    /**
     * Accesseur : ctrlPressed
     */
    bool getCtrlPressed();

    /**
     * Accesseur : ctrlPressed
     * @param val La touche Contr�le est press� ou non
     */
    void setCtrlPressed(bool val);

signals:
    /**
     * D�clench� lorsque l'utilisateur fait d�filer les scrollbars
     */
    void onScroll();

    /**
     * Ctrl + Molette haut
     */
    void zoomIn();

    /**
     * Ctrl + Molette bas
     */
    void zoomOut();
};
