#pragma once

#include <QGraphicsView>
#include <QDragMoveEvent>
#include <customgraphicsscene.h>

/**
 * @class CustomGraphicsView
 *
 * GraphicsView notifiant des évènements de scroll
 */
class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
protected:
    /**
     * Indique si la CustomGraphicsView est jumelée
     * Si true, onScroll sera généré à chaque défilement des scrollbars
     */
    bool dualMode;

    /**
     * Permet de savoir si la touche contrôle est pressé (pour le zoom avec molette)
     */
    bool ctrlPressed;

    /**
     * Hérité
     */
    void scrollContentsBy(int dx, int dy);

    /**
     * Surcharge des méthodes de détection de touches
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
     * @param val La touche Contrôle est pressé ou non
     */
    void setCtrlPressed(bool val);

signals:
    /**
     * Déclenché lorsque l'utilisateur fait défiler les scrollbars
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
