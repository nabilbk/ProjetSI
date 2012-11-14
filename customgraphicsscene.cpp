#include "customgraphicsscene.h"

#include <cmath>

#include <QDebug>
#include <QPainter>
#include <QColor>

CustomGraphicsScene::CustomGraphicsScene(QObject *parent)
    : QGraphicsScene(parent), first(0,0), second(0,0), FirstLine(0), SecondLine(0), scaleMode(0), angleMode(1),
    imageLoaded(false)
{
    graphicMode = SCROLLHAND;
    regionOfInterest = 0;
    FirstLine = NULL;
    SecondLine = NULL;
    firstRec = true;
    pen.setColor(QColor(255,150,0));
    pen.setWidth(2);
    angleMode = 1;
    movePen = new QPen(QColor(255,0,0));
    releasePen = new QPen(QColor(0,255, 0));
    horizontalPen = new QPen(QColor(0,150,0));
}

CustomGraphicsScene::~CustomGraphicsScene()
{
    clear();
    delete movePen;
    delete releasePen;
    delete horizontalPen;
}

void CustomGraphicsScene::clear()
{
    if (!regionOfInterest)
        delete regionOfInterest;
    if (!FirstLine)
    {
        delete FirstLine;
        FirstLine = 0;
    }
    if (!SecondLine)
    {
        SecondLine = 0;
        delete SecondLine;
    }
    QGraphicsScene::clear();
    first = QPointF(0, 0);
    second = QPointF(0, 0);
    scaleMode = 0;
    angleMode = 1;
    graphicMode = SCROLLHAND;
    regionOfInterest = NULL;
    FirstLine = NULL;
    SecondLine = NULL;
    firstRec = true;
    pen.setColor(QColor(255,150,0));
    pen.setWidth(2);
    imageLoaded = false;
}

QPointF CustomGraphicsScene::getFirst()
{
    return first;
}

QPointF CustomGraphicsScene::getSecond()
{
    return second;
}

bool CustomGraphicsScene::isImageLoaded()
{
    return imageLoaded;
}

QGraphicsRectItem* CustomGraphicsScene::getRegionOfInterest()
{
    return regionOfInterest;
}

void CustomGraphicsScene::setMode(int nmode)
{
    scaleMode = nmode;
    if (scaleMode == 0)
    {
        first = QPointF(0,0);
        second = QPointF(0,0);
    }
}

void CustomGraphicsScene::setGraphicMode(int mode)
{
    graphicMode = mode;
}

void CustomGraphicsScene::clearRegionOfInterest()
{
    if (regionOfInterest != 0)
    {
        removeItem(regionOfInterest);
        delete regionOfInterest;
        regionOfInterest = 0;
        firstRec = true;
    }
}

QGraphicsPixmapItem* CustomGraphicsScene::addPixmap (const QPixmap &pixmap)
{
    imageLoaded = true;

    return QGraphicsScene::addPixmap(pixmap);
}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (graphicMode == ANGLEDETECTION )
    {
        if (mouseEvent->button() == Qt::RightButton)
        {
            if (FirstLine != 0)
            {
                delete FirstLine;
                FirstLine = 0;
            }
            if(SecondLine != 0)
            {
                delete SecondLine;
                SecondLine = 0;
            }
            angleMode = 1;
            update();
        }
        else
        {
            switch (angleMode)
            {
            case 1:
                if (itemsBoundingRect().contains(mouseEvent->scenePos()))
                {
                    first = mouseEvent->scenePos();
                    angleMode = 2;
                }
                break;
            case 2:
                break;
            case 3:
                if (itemsBoundingRect().contains(mouseEvent->scenePos()))
                {
                    if (SecondLine != 0)
                    {
                        delete SecondLine;
                        SecondLine = 0;
                    }
                    first = mouseEvent->scenePos();
                    angleMode = 4;
                }
                break;
            case 4:
                angleMode = 0;
                break;
            }
        }
    }
    else
    {
        if (mouseEvent->button() == Qt::RightButton)
        {
            emit(needToCleanItems());
        }
        if (graphicMode == RUBBERBAND )
        {
            origin = mouseEvent->scenePos();
            if (origin.x() <=0)
                origin.setX(0);
            if (origin.y() <=0)
                origin.setY(0);
            if (origin.x() > width())
                origin.setX(width());
            if (origin.y() > height())
                origin.setY(height());
        }
        else
        {
            switch(scaleMode)
            {
                case 1:
                    first = mouseEvent->scenePos();
                    scaleMode = 2;
                    break;
                case 2:
                    second = mouseEvent->scenePos();
                    scaleMode = 3;
                    emit scaleFinish();
                    break;
            }
        }
    }


    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (graphicMode == ANGLEDETECTION)
    {
        switch (angleMode)
        {
        case 1:
            break;
        case 2:
            second = mouseEvent->scenePos();
            if (FirstLine == NULL)
            {
                if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                {
                    FirstLine = addLine(first.x(), first.y(), second.x(), second.y(), *movePen);
                }
                else
                {
                    if(first.x() < itemsBoundingRect().x())
                        FirstLine = addLine(itemsBoundingRect().x(), first.y(), second.x(), second.y(), *movePen);
                    if(first.x() > itemsBoundingRect().width())
                        FirstLine = addLine(itemsBoundingRect().width(), first.y(), second.x(), second.y(), *movePen);
                    if(first.y() < itemsBoundingRect().y())
                        FirstLine = addLine(first.x(), itemsBoundingRect().y(), second.x(), second.y(), *movePen);
                    if(first.y() > itemsBoundingRect().height())
                        FirstLine = addLine(first.x(), itemsBoundingRect().height(), second.x(), second.y(), *movePen);

                    if(second.x() < itemsBoundingRect().x())
                        FirstLine = addLine(first.x(), first.y(), itemsBoundingRect().x(), second.y(), *movePen);
                    if(second.x() > itemsBoundingRect().width())
                        FirstLine = addLine(first.x(), first.y(), itemsBoundingRect().width(), second.y(), *movePen);
                    if(second.y() < itemsBoundingRect().y())
                        FirstLine = addLine(first.x(), first.y(), second.x(), itemsBoundingRect().y(), *movePen);
                    if(second.y() > itemsBoundingRect().height())
                        FirstLine = addLine(first.x(), first.y(), second.x(), itemsBoundingRect().height(), *movePen);

                    if(second.x() > itemsBoundingRect().width() && second.y() > itemsBoundingRect().height())
                        FirstLine = addLine(first.x(), first.y(), itemsBoundingRect().width(), itemsBoundingRect().height(), *movePen);
                }
            }
            else
            {
                if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                {
                    FirstLine->setLine(first.x(),first.y(),second.x(),second.y());
                }
                else
                {
                    if(second.x() > itemsBoundingRect().width() && second.y() > itemsBoundingRect().height())
                        FirstLine->setLine(first.x(), first.y(), itemsBoundingRect().width(), itemsBoundingRect().height());

                    if(first.x() < itemsBoundingRect().x())
                        FirstLine->setLine(itemsBoundingRect().x(), first.y(), second.x(), second.y());
                    if(first.x() > itemsBoundingRect().width())
                        FirstLine->setLine(itemsBoundingRect().width(), first.y(), second.x(), second.y());
                    if(first.y() < itemsBoundingRect().y())
                        FirstLine->setLine(first.x(), itemsBoundingRect().y(), second.x(), second.y());
                    if(first.y() > itemsBoundingRect().height())
                        FirstLine->setLine(first.x(), itemsBoundingRect().height(), second.x(), second.y());

                    if(second.x() < itemsBoundingRect().x())
                        FirstLine->setLine(first.x(), first.y(), itemsBoundingRect().x(), second.y());
                    if(second.x() > itemsBoundingRect().width())
                        FirstLine->setLine(first.x(), first.y(), itemsBoundingRect().width(), second.y());
                    if(second.y() < itemsBoundingRect().y())
                        FirstLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().y());
                    if(second.y() > itemsBoundingRect().height())
                        FirstLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().height());


                }
            }
            break;
        case 3:
            break;
        case 4:
            second = mouseEvent->scenePos();
            if (SecondLine == NULL)
            {
                if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                {
                    SecondLine = addLine(first.x(), first.y(), second.x(), second.y(), *movePen);
                }
                else
                {
                    if(first.x() < itemsBoundingRect().x())
                        SecondLine = addLine(itemsBoundingRect().x(), first.y(), second.x(), second.y(), *movePen);
                    if(first.x() > itemsBoundingRect().width())
                        SecondLine = addLine(itemsBoundingRect().width(), first.y(), second.x(), second.y(), *movePen);
                    if(first.y() < itemsBoundingRect().y())
                        SecondLine = addLine(first.x(), itemsBoundingRect().y(), second.x(), second.y(), *movePen);
                    if(first.y() > itemsBoundingRect().height())
                        SecondLine = addLine(first.x(), itemsBoundingRect().height(), second.x(), second.y(), *movePen);

                    if(second.x() < itemsBoundingRect().x())
                        SecondLine = addLine(first.x(), first.y(), itemsBoundingRect().x(), second.y(), *movePen);
                    if(second.x() > itemsBoundingRect().width())
                        SecondLine = addLine(first.x(), first.y(), itemsBoundingRect().width(), second.y(), *movePen);
                    if(second.y() < itemsBoundingRect().y())
                        SecondLine = addLine(first.x(), first.y(), second.x(), itemsBoundingRect().y(), *movePen);
                    if(second.y() > itemsBoundingRect().height())
                        SecondLine = addLine(first.x(), first.y(), second.x(), itemsBoundingRect().height(), *movePen);
                }
            }
            else
            {
                if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                {
                    SecondLine->setLine(first.x(),first.y(),second.x(),second.y());
                }
                else
                {
                    if(first.x() < itemsBoundingRect().x())
                        SecondLine->setLine(itemsBoundingRect().x(), first.y(), second.x(), second.y());
                    if(first.x() > itemsBoundingRect().width())
                        SecondLine->setLine(itemsBoundingRect().width(), first.y(), second.x(), second.y());
                    if(first.y() < itemsBoundingRect().y())
                        SecondLine->setLine(first.x(), itemsBoundingRect().y(), second.x(), second.y());
                    if(first.y() > itemsBoundingRect().height())
                        SecondLine->setLine(first.x(), itemsBoundingRect().height(), second.x(), second.y());

                    if(second.x() < itemsBoundingRect().x())
                        SecondLine->setLine(first.x(), first.y(), itemsBoundingRect().x(), second.y());
                    if(second.x() > itemsBoundingRect().width())
                        SecondLine->setLine(first.x(), first.y(), itemsBoundingRect().width(), second.y());
                    if(second.y() < itemsBoundingRect().y())
                        SecondLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().y());
                    if(second.y() > itemsBoundingRect().height())
                        SecondLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().height());
                }
            }

            if (FirstLine != 0 && SecondLine != 0)
                emit angle(FirstLine->line().angleTo(SecondLine->line()));
            break;
        }
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CustomGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (graphicMode == RUBBERBAND)
    {
        double x, y, w, h;
        QPointF releasePos = mouseEvent->scenePos();

        if (releasePos.x() == origin.x() && releasePos.y() == origin.y())
        {
            x = 0;
            y = 0;
            w = width();
            h = height();
        }
        else
        {
            if (releasePos.x() <=0)
                releasePos.setX(0);
            if (releasePos.y() <=0)
                releasePos.setY(0);
            if (releasePos.x() > width())
                releasePos.setX(width());
            if (releasePos.y() > height())
                releasePos.setY(height());

            if (origin.x() <= releasePos.x())
                x = origin.x();
            else
                x = releasePos.x();
            if (origin.y() <= releasePos.y())
                y = origin.y();
            else
                y = releasePos.y();

            w = std::abs(origin.x() - releasePos.x());
            h = std::abs(origin.y() - releasePos.y());
        }
        if (firstRec)
        {
            regionOfInterest = addRect(x, y, w, h);
            regionOfInterest->setPen(pen);;
            firstRec = false;
        }
        else
        {
            regionOfInterest->setRect(x, y, w, h);
        }
        confirmedROI = new QRectF(regionOfInterest->rect());
        emit confirmROI(confirmedROI);
        delete confirmedROI;
        origin = QPointF(0,0);
        //        setGraphicMode(SCROLLHAND);
    }

    if (graphicMode == MANUALSELECTION)
    {
        QPointF releasePos = mouseEvent->scenePos();
        emit offset(releasePos);

    }

    if (graphicMode == ANGLEDETECTION)
    {
        switch (angleMode)
        {
        case 1:
            break;
        case 2:
            second = mouseEvent->scenePos();
            if (FirstLine != NULL)
            {
                    if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                    {
                        FirstLine->setLine(first.x(),first.y(),second.x(),second.y());
                        FirstLine->setPen(*horizontalPen);
                        angleMode = 3;
                    }
                    else
                    {
                        if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                        {
                            FirstLine->setLine(first.x(),first.y(),second.x(),second.y());
                        }
                        else
                        {
                            if(first.x() < itemsBoundingRect().x())
                                FirstLine->setLine(itemsBoundingRect().x(), first.y(), second.x(), second.y());
                            if(first.x() > itemsBoundingRect().width())
                                FirstLine->setLine(itemsBoundingRect().width(), first.y(), second.x(), second.y());
                            if(first.y() < itemsBoundingRect().y())
                                FirstLine->setLine(first.x(), itemsBoundingRect().y(), second.x(), second.y());
                            if(first.y() > itemsBoundingRect().height())
                                FirstLine->setLine(first.x(), itemsBoundingRect().height(), second.x(), second.y());

                            if(second.x() < itemsBoundingRect().x())
                                FirstLine->setLine(first.x(), first.y(), itemsBoundingRect().x(), second.y());
                            if(second.x() > itemsBoundingRect().width())
                                FirstLine->setLine(first.x(), first.y(), itemsBoundingRect().width(), second.y());
                            if(second.y() < itemsBoundingRect().y())
                                FirstLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().y());
                            if(second.y() > itemsBoundingRect().height())
                                FirstLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().height());
                        }
                        FirstLine->setPen(*horizontalPen);
                        angleMode = 3;
                    }

            }
            break;
        case 3:
            break;
        case 4:
            second = mouseEvent->scenePos();
            if (SecondLine != NULL)
            {
                    if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                    {
                        SecondLine->setLine(first.x(),first.y(),second.x(),second.y());
                        SecondLine->setPen(*releasePen);
                        angleMode = 3;
                    }
                    else
                    {
                        if(itemsBoundingRect().contains(first) && itemsBoundingRect().contains(second))
                        {
                            SecondLine->setLine(first.x(),first.y(),second.x(),second.y());
                        }
                        else
                        {
                            if(first.x() < itemsBoundingRect().x())
                                SecondLine->setLine(itemsBoundingRect().x(), first.y(), second.x(), second.y());
                            if(first.x() > itemsBoundingRect().width())
                                SecondLine->setLine(itemsBoundingRect().width(), first.y(), second.x(), second.y());
                            if(first.y() < itemsBoundingRect().y())
                                SecondLine->setLine(first.x(), itemsBoundingRect().y(), second.x(), second.y());
                            if(first.y() > itemsBoundingRect().height())
                                SecondLine->setLine(first.x(), itemsBoundingRect().height(), second.x(), second.y());

                            if(second.x() < itemsBoundingRect().x())
                                SecondLine->setLine(first.x(), first.y(), itemsBoundingRect().x(), second.y());
                            if(second.x() > itemsBoundingRect().width())
                                SecondLine->setLine(first.x(), first.y(), itemsBoundingRect().width(), second.y());
                            if(second.y() < itemsBoundingRect().y())
                                SecondLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().y());
                            if(second.y() > itemsBoundingRect().height())
                                SecondLine->setLine(first.x(), first.y(), second.x(), itemsBoundingRect().height());
                        }
                        SecondLine->setPen(*releasePen);
                        angleMode = 3;
                    }

            }
            if (FirstLine != 0 && SecondLine != 0)
                emit angle(FirstLine->line().angleTo(SecondLine->line()));
            angleMode = 3;
            break;
        }

    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
