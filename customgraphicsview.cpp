#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent), dualMode(false), ctrlPressed(false)
{
}

CustomGraphicsView::~CustomGraphicsView()
{
}

void CustomGraphicsView::setDualMode(bool dualMode)
{
    this->dualMode = dualMode;
}

void CustomGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlPressed = true;
    QWidget::keyPressEvent(event);
}

void CustomGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlPressed = false;
    QWidget::keyReleaseEvent(event);
}

void CustomGraphicsView::wheelEvent(QWheelEvent * event)
{
    if (!ctrlPressed)
        QGraphicsView::wheelEvent(event);
    else
    {
        if (event->delta() > 0)
        {
            emit(zoomIn());
        }
        else
        {
            emit(zoomOut());
        }
    }
}

bool CustomGraphicsView::getCtrlPressed()
{
    return ctrlPressed;
}

void CustomGraphicsView::setCtrlPressed(bool val)
{
    ctrlPressed = val;
}

void CustomGraphicsView::scrollContentsBy(int dx, int dy)
{
    if (dualMode)
        emit onScroll();

    QGraphicsView::scrollContentsBy(dx, dy);
}


