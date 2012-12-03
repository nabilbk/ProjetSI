#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"

#include <fstream>
#include <QDebug>
#include <QGLWidget>

MainWindow::MainWindow(Controller *control,QWidget *parent) :
    QMainWindow(parent),
    controller(control),
    imageItem(0),
    normalFactor(1),
    imageScaleDialog(this),
    extractSequenceDialog(this),
    aboutDialog(this),
    seekBar(new QSlider(Qt::Horizontal, this)),
    separator(0),
    ui(new Ui::MainWindow)
{
    // Interface
    ui->setupUi(this);

    // Enregistrement des event handlers
    connect(ui->imageOpenAct,       SIGNAL(triggered()),        this,       SLOT(openImage()));
    connect(ui->imagesOpenAct,      SIGNAL(triggered()),        this,       SLOT(openImages()));
    connect(ui->videoOpenAct,       SIGNAL(triggered()),        this,       SLOT(openVideo()));
    connect(ui->webcamOpenAct,      SIGNAL(triggered()),        this,       SLOT(openWebcam()));
    connect(ui->closeAct,           SIGNAL(triggered()),        this,       SLOT(closeCurrent()));
    connect(ui->exitAct,            SIGNAL(triggered()),        this,       SLOT(close()));
    connect(ui->zoomInAct,          SIGNAL(triggered()),        this,       SLOT(zoomIn()));
    connect(ui->zoomOutAct,         SIGNAL(triggered()),        this,       SLOT(zoomOut()));
    connect(ui->normalSizeAct,      SIGNAL(triggered()),        this,       SLOT(normalSize()));
    connect(ui->processAct,         SIGNAL(triggered()),        controller, SLOT(process()));
    connect(ui->processBatchAct,    SIGNAL(triggered()),        controller, SLOT(processBatch()));
    connect(ui->fitToWindowAct,     SIGNAL(triggered(bool)),    this,       SLOT(fitToWindow(bool)));
    connect(ui->playAct,            SIGNAL(triggered()),        controller, SLOT(play()));
    connect(ui->pauseAct,           SIGNAL(triggered()),        controller, SLOT(pause()));
    connect(ui->showParameterAct,   SIGNAL(triggered(bool)),    controller, SLOT(showParameter(bool)));
    connect(ui->selectionAct,       SIGNAL(triggered()),        this,       SLOT(clickZoneInteret()));
    connect(ui->handAct,            SIGNAL(triggered()),        this,       SLOT(hand()));
    connect(ui->manualSelectionAct, SIGNAL(triggered()),        this,       SLOT(manualSelection()));
    connect(ui->scaleAct,           SIGNAL(triggered()),        this,       SLOT(imageScale()));
    connect(ui->fastAct,            SIGNAL(triggered(bool)),    controller, SLOT(fastForward(bool)));
    connect(ui->backBegin,          SIGNAL(triggered()),        controller, SLOT(restartVideo()));
    connect(ui->processStreamAct,   SIGNAL(triggered(bool)),    this,       SLOT(processStream(bool)));
    connect(ui->nextAct,            SIGNAL(triggered()),        controller, SLOT(nextImages()));
    connect(ui->previousAct,        SIGNAL(triggered()),        controller, SLOT(previousImages()));
    connect(ui->indexAct,           SIGNAL(triggered()),        controller, SLOT(openHelp()));
    connect(seekBar,                SIGNAL(sliderReleased()),   controller, SLOT(sliderMoved()));

    // Permet de modifier l'affichage pendant un traitement
    connect(controller,             SIGNAL(processReady(bool)), this,       SLOT(enableProcess(bool)));

    // Pour la définition de l'échelle
    connect(&imageScaleDialog,      SIGNAL(scaleSet(qreal)),    this,       SLOT(imageScale(qreal)));

    // Sauvegardes
    connect(&extractSequenceDialog, SIGNAL(notifieExtractSequence(int)), controller, SLOT(extractSequence(int)));
    connect(&saveDialog,            SIGNAL(saveSequences()),             this,       SLOT(saveSequencesButtonPressed()));
    connect(&saveDialog,            SIGNAL(saveResult()),                controller, SLOT(saveResultButtonPressed()));
    connect(&saveDialog,            SIGNAL(exportResult()),              controller, SLOT(exportResultButtonPressed()));
    connect(&saveDialog,            SIGNAL(importFile()),                this,       SLOT(importFileButtonPressed()));


    // Pour synchronizer les QGraphicsView ainsi que le zoom à la molette
    connect(ui->imageView,          SIGNAL(onScroll()),         this,       SLOT(secondDrag()));
    connect(ui->secondImage,        SIGNAL(onScroll()),         this,       SLOT(firstDrag()));
    connect(ui->imageView,          SIGNAL(zoomIn()),           this,       SLOT(zoomIn()));
    connect(ui->imageView,          SIGNAL(zoomOut()),          this,       SLOT(zoomOut()));

    // En cas de changement de média
    connect(this,                   SIGNAL(imageChanged(QImage*)),          controller, SLOT(loadImage(QImage*)));
    connect(this,                   SIGNAL(imagesChanged(QList<QImage*>*)), controller, SLOT(loadImages(QList<QImage*>*)));
    connect(this,                   SIGNAL(videoChanged(QString&)),         controller, SLOT(loadVideo(QString&)));
    connect(this,                   SIGNAL(mediaClosed()),                  controller, SLOT(reset()));
    connect(this,                   SIGNAL(changeGraphicMode(int)),         controller, SLOT(changeGraphicMode(int)));

    // Affichage de la fenêtre à propos
    connect(ui->aboutAct,           SIGNAL(triggered()), &aboutDialog, SLOT(show()));

    // Gestion de la fenêtre de création dynamique de passes
    connect(ui->createFilterAct,     SIGNAL(triggered()),              &dynamicPassChainDialog, SLOT(show()));
    connect(&dynamicPassChainDialog, SIGNAL(emitList(QList<QString>)), controller,              SLOT(dynamicMethodSetted(QList<QString>)));

    // Décochement des certains paramètres
    connect(controller, SIGNAL(tickShowParameterOption(bool)), this, SLOT(tickParameter(bool)));

    // Mise en place de la seekBar pour le curseur de la vidéo
    seekBar->setMaximumWidth(300);
    ui->toolBar->addWidget(seekBar);
    ui->toolBar->addSeparator();

    // Etats initaux des menus et autres
    resetButtons(false, TOUT);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete seekBar;
}

void MainWindow::resetButtons(bool val, int type)
{
    mode = type;

    switch(type)
    {
    case TOUT:
            ui->processAct->setEnabled(val);
            ui->processStreamAct->setEnabled(val);
            ui->scaleAct->setEnabled(val);
            ui->fastAct->setEnabled(val);
            ui->playAct->setEnabled(val);
            ui->backBegin->setEnabled(val);
            ui->pauseAct->setEnabled(val);
            seekBar->setEnabled(val);
            ui->selectionAct->setEnabled(val);
            ui->manualSelectionAct->setEnabled(val);
            break;
    case IMAGE:
            ui->processAct->setEnabled(val);
            ui->processStreamAct->setEnabled(!val);
            ui->scaleAct->setEnabled(val);
            ui->fastAct->setEnabled(!val);
            ui->playAct->setEnabled(!val);
            ui->backBegin->setEnabled(!val);
            ui->pauseAct->setEnabled(!val);
            seekBar->setEnabled(!val);
            ui->selectionAct->setEnabled(val);
            ui->manualSelectionAct->setEnabled(val);
            break;
    case IMAGES:
            ui->processAct->setEnabled(val);
            ui->processStreamAct->setEnabled(!val);
            ui->scaleAct->setEnabled(val);
            ui->fastAct->setEnabled(!val);
            ui->playAct->setEnabled(!val);
            ui->backBegin->setEnabled(!val);
            ui->pauseAct->setEnabled(!val);
            seekBar->setEnabled(!val);
            ui->selectionAct->setEnabled(!val);
            ui->manualSelectionAct->setEnabled(val);
            break;
    case VIDEO:
            ui->processAct->setEnabled(val);
            ui->processStreamAct->setEnabled(val);
            ui->scaleAct->setEnabled(!val);
            ui->fastAct->setEnabled(val);
            ui->playAct->setEnabled(val);
            ui->backBegin->setEnabled(val);
            ui->pauseAct->setEnabled(val);
            seekBar->setEnabled(val);
            ui->selectionAct->setEnabled(!val);
            ui->manualSelectionAct->setEnabled(!val);
            break;
    case WEBCAM:
            ui->processAct->setEnabled(val);
            ui->processStreamAct->setEnabled(val);
            ui->scaleAct->setEnabled(!val);
            ui->fastAct->setEnabled(!val);
            ui->playAct->setEnabled(!val);
            ui->backBegin->setEnabled(!val);
            ui->pauseAct->setEnabled(!val);
            seekBar->setEnabled(!val);
            ui->selectionAct->setEnabled(!val);
            ui->manualSelectionAct->setEnabled(!val);
            break;
    }
}

void MainWindow::methodChanged(int method)
{
    if (mode != TOUT)
    {
        switch(method)
        {
        case DYNAMIC:
        case MEANSHIFT:
        case PERSONNALISEE:
            ui->manualSelectionAct->setEnabled(true);
            ui->selectionAct->setEnabled(true);
            ui->processAct->setEnabled(true);

            // Si ce bouton est visible, alors le logiciel est en mode traitement groupée
            if (ui->nextAct->isVisible())
                ui->processBatchAct->setEnabled(true);
            if (mode == WEBCAM || mode == VIDEO)
                ui->processStreamAct->setEnabled(true);
            break;
        case HOUGH:
        case GOUTTE:
            ui->manualSelectionAct->setEnabled(false);
            ui->selectionAct->setEnabled(true);
            ui->processAct->setEnabled(true);

            // Si ce bouton est visible, alors le logiciel est en mode traitement groupée
            if (ui->nextAct->isVisible())
                ui->processBatchAct->setEnabled(true);

            ui->processStreamAct->setChecked(false);
            ui->processStreamAct->setEnabled(false);
            break;
        case RAPPORTEUR:
            ui->manualSelectionAct->setEnabled(true);
            ui->selectionAct->setEnabled(false);
            ui->processAct->setEnabled(false);
            ui->processBatchAct->setEnabled(false);
            ui->processStreamAct->setChecked(false);
            ui->processStreamAct->setEnabled(false);
            break;
        }
    }
}

void MainWindow::openImage()
{ 
    QDir dir = QDir::currentPath();
    if (dir.exists("../images"))
        dir.cd("../images");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"),
                                                    dir.absolutePath(),
                                                    tr("Images (*.png *.gif *.jpg *.tif *.bmp)"));

    if (!fileName.isEmpty())
    {
        QImage *image = new QImage(fileName);

        if (image->isNull())
        {
            QMessageBox::information(this, tr("Erreur"),
                                     tr("Impossible de charger %1.").arg(fileName));
            delete image;

            return;
        }
        closeCurrent();
        emit imageChanged(image);
    }
    else
    {
        if (!controller->scene.isImageLoaded())
            resetButtons(false, TOUT);
    }
}

void MainWindow::openImages()
{
    QDir dir = QDir::currentPath();
    if (dir.exists("../images"))
        dir.cd("../images");

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Ouvrir un fichier"),
                                                    dir.absolutePath(),
                                                    tr("Images (*.png *.gif *.jpg *.tif *.bmp)"));
    if (!fileNames.isEmpty())
    {
        QList<QImage *> *images = new QList<QImage *>;
        for(QStringList::iterator beg = fileNames.begin(); beg != fileNames.end(); ++beg)
            images->push_back(new QImage(*beg));

        if (images->empty())
        {
            QMessageBox::information(this, tr("Erreur"),
                                     tr("Impossible de charger les fichiers."));
            delete images;
            return;
        }
        closeCurrent();
        ui->processBatchAct->setVisible(true);
        ui->processBatchAct->setEnabled(true);
        separator = ui->toolBar->insertSeparator(ui->previousAct);
        ui->nextAct->setVisible(true);
        ui->previousAct->setVisible(true);
        emit imagesChanged(images);
    }
    else
    {
        if (!controller->scene.isImageLoaded())
            resetButtons(false, TOUT);
    }


}

void MainWindow::openVideo()
{
    QDir dir = QDir::currentPath();
    if (dir.exists("../images"))
        dir.cd("../images");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"),
                                                    dir.absolutePath(),
                                                    tr("Vidéos (*.avi)"));

    if (!fileName.isEmpty())
    {
        closeCurrent();
        emit videoChanged(fileName);
    }

}

void MainWindow::openWebcam()
{
    closeCurrent();
    controller->openWebcam();
}

void MainWindow::closeCurrent()
{
    ui->processStreamAct->setChecked(false);
    enableProcess(true);

    normalSize();

    // Enlève un séparateur qui apparait seulement en traitement groupé
    if (separator != 0)
    {
        ui->toolBar->removeAction(separator);
        separator = 0;
    }
    // Fait disparaitre les boutons du traitement groupé
    ui->nextAct->setVisible(false);
    ui->previousAct->setVisible(false);
    ui->processBatchAct->setVisible(false);

    resetButtons(false, TOUT);

    emit mediaClosed();
}

void MainWindow::sceneChanged(CustomGraphicsScene *cScene, QGraphicsPixmapItem *cImageItem)
{
    if (cImageItem != 0)
        imageItem = cImageItem;

    //Nécessaire pour adapter le rectangle de l'affichage aux items actuels
    cScene->setSceneRect(cScene->itemsBoundingRect());
    ui->imageView->setScene(cScene);

    // Si l'image est à l'échelle de la fenetre, la taille est ajusté au démarrage
    fitToWindow();
}

void MainWindow::binarySceneChanged(CustomGraphicsScene *binaryScene)
{
    //Nécessaire pour adapter le rectangle de l'affichage aux items actuels
    binaryScene->setSceneRect(binaryScene->itemsBoundingRect());
    ui->secondImage->setScene(binaryScene);

    // Si l'image est à l'échelle de la fenetre, la taille est ajusté au démarrage
    fitToWindow();
}


void MainWindow::fitToWindow()
{
    if (imageItem != 0)
    {
        secondDrag();

        if (ui->fitToWindowAct->isChecked())
        {
            // Si on a décoché l'image à l'échelle, on la remet à sa taille initiale
            normalSize();

            ui->imageView->fitInView(QRectF(0,0,(qreal)imageItem->pixmap().width(),(qreal)imageItem->pixmap().height()),Qt::IgnoreAspectRatio);
            if (controller->getMode() != 1)
                ui->secondImage->fitInView(QRectF(0,0,(qreal)imageItem->pixmap().width(),(qreal)imageItem->pixmap().height()),Qt::IgnoreAspectRatio);
        }

        updateActions();
    }
}

void MainWindow::fitToWindow(bool val)
{
    if (imageItem != 0)
    {
        secondDrag();

        // Si on a décoché l'image à l'échelle, on la remet à sa taille initiale
        normalSize();

        if (val)
        {
            ui->imageView->fitInView(QRectF(0,0,(qreal)imageItem->pixmap().width(),(qreal)imageItem->pixmap().height()),Qt::IgnoreAspectRatio);
            if (controller->getMode() == 0)
                ui->secondImage->fitInView(QRectF(0,0,(qreal)imageItem->pixmap().width(),(qreal)imageItem->pixmap().height()),Qt::IgnoreAspectRatio);
        }

        updateActions();
    }
}

void MainWindow::zoomIn()
{
    ui->imageView->setDualMode(false);
    ui->secondImage->setDualMode(false);
    scaleImage(1.20);
    ui->imageView->setDualMode(true);
    ui->secondImage->setDualMode(true);
}

void MainWindow::zoomOut()
{
    ui->imageView->setDualMode(false);
    ui->secondImage->setDualMode(false);
    scaleImage(1 / 1.20);
    ui->imageView->setDualMode(true);
    ui->secondImage->setDualMode(true);
}

void MainWindow::normalSize()
{
    ui->imageView->resetMatrix();
    ui->secondImage->resetMatrix();
    normalFactor = 1;
}

void MainWindow::updateActions()
{
    // On désactive certains menus mutuellement exclusifs
    ui->zoomInAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->zoomOutAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->normalSizeAct->setEnabled(!ui->fitToWindowAct->isChecked());
}

void MainWindow::scaleImage(qreal factor)
{
    normalFactor /= factor;

    ui->imageView->scale(factor, factor);
    if (controller->getMode() == 0)
    {
        ui->secondImage->scale(factor, factor);
    }

    // On empêche le zoom ou dézoom si les valeurs sont trop fortes
    ui->zoomInAct->setEnabled(normalFactor  > 0.1);
    ui->zoomOutAct->setEnabled(normalFactor < 5);
}

void MainWindow::resizeEvent(QResizeEvent *event )
{
    if (imageItem != NULL)
    {
        // Si l'image est initialisée
        if (ui->fitToWindowAct->isChecked())
        {
            ui->imageView->fitInView(QRectF(0,0,(qreal)imageItem->pixmap().width(),(qreal)imageItem->pixmap().height()),Qt::IgnoreAspectRatio);
            if (controller->getMode() == 0)
            {
                ui->secondImage->fitInView(QRectF(0,0,(qreal)imageItem->pixmap().width(),(qreal)imageItem->pixmap().height()),Qt::IgnoreAspectRatio);
            }
        }
    }

    // On redimensionne la fenêtre
    QMainWindow::resizeEvent(event);
}

void MainWindow::tickParameter(bool val)
{
    ui->showParameterAct->setChecked(val);
}

void MainWindow::enableProcess(bool enabled)
{
    // Nécessaire à chaque appel, car pour remettre le curseur initial,
    // il faut appeler la fonction restore autant de fois qu'on a appelé set*
    // avant
    QApplication::restoreOverrideCursor();
    if (!controller->getStreamProcessActivated())
    {
        if (!enabled)
            QApplication::setOverrideCursor(Qt::WaitCursor);

        if (controller->scene.isImageLoaded())
            ui->processAct->setEnabled(enabled);

        ui->imageOpenAct->setEnabled(enabled);
        ui->imagesOpenAct->setEnabled(enabled);
        ui->videoOpenAct->setEnabled(enabled);
        ui->webcamOpenAct->setEnabled(enabled);
    }
    else
    {
        if (!enabled)
            QApplication::setOverrideCursor(Qt::WaitCursor);
    }
}

void MainWindow::imageScale()
{
    controller->scaleMode();

    // Nécessaire à chaque appel, car pour remettre le curseur initial,
    // il faut appeler la fonction restore autant de fois qu'on a appelé set*
    // avant
    QApplication::restoreOverrideCursor();

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::imageScale(qreal realDist)
{
    controller->scale(realDist);
}

void MainWindow::showScale()
{
    imageScaleDialog.show();
    QApplication::restoreOverrideCursor();
}

void MainWindow::showExtract()
{
    extractSequenceDialog.show();
    QApplication::restoreOverrideCursor();
}

void MainWindow::showSaveDialog()
{
    saveDialog.show();
    QApplication::restoreOverrideCursor();
}

void MainWindow::clickZoneInteret()
{
    ui->manualSelectionAct->setChecked(false);
    ui->handAct->setChecked(false);
    ui->selectionAct->setChecked(true);
    ui->imageView->setDragMode(QGraphicsView::RubberBandDrag);
    controller->setAutoMode(true);
    if (imageItem != 0)
        emit changeGraphicMode(RUBBERBAND);
}

void MainWindow::hand()
{
    ui->manualSelectionAct->setChecked(false);
    ui->handAct->setChecked(true);
    ui->selectionAct->setChecked(false);
    ui->imageView->setDragMode(QGraphicsView::ScrollHandDrag);
    controller->setAutoMode(true);
    emit changeGraphicMode(SCROLLHAND);
}

void MainWindow::manualSelection()
{
    ui->manualSelectionAct->setChecked(true);
    ui->handAct->setChecked(false);
    ui->selectionAct->setChecked(false);
    ui->imageView->setDragMode(QGraphicsView::NoDrag);
    if (controller->getMode() == RAPPORTEUR)
    {
        ui->imageView->setDragMode(QGraphicsView::NoDrag);
        if (imageItem != 0)
            emit changeGraphicMode(ANGLEDETECTION);
    }
    else
    {
        controller->setAutoMode(false);
        if (imageItem != 0)
            emit changeGraphicMode(MANUALSELECTION);
    }
}

void MainWindow::protractor()
{
    if (mode != TOUT)
    {
        ui->selectionAct->setEnabled(false);
        ui->manualSelectionAct->setChecked(true);
        ui->handAct->setChecked(false);
        ui->selectionAct->setChecked(false);
        ui->imageView->setDragMode(QGraphicsView::NoDrag);
        if (imageItem != 0)
            emit changeGraphicMode(ANGLEDETECTION);
    }

}

void MainWindow::showBinaryImage()
{
    ui->imageView->setDualMode(true);
    ui->secondImage->setDualMode(true);
    if (mode == IMAGE)
    {
        ui->secondImage->setVisible(true);
        ui->secondImage->setSizeIncrement(0,0);
        ui->imageView->setVisible(true);
    }
    else
    {
        ui->imageView->setVisible(true);
        ui->imageView->setSizeIncrement(0,0);
        ui->secondImage->setVisible(true);
    }
}

void MainWindow::hideBinaryImage()
{
    ui->imageView->setDualMode(false);
    ui->secondImage->setDualMode(false);
    if (mode == IMAGE)
    {
        ui->secondImage->setVisible(false);
        ui->imageView->setVisible(true);
    }
    else
    {
        ui->imageView->setVisible(false);
        ui->secondImage->setVisible(true);
    }
}

void MainWindow::manualSelectionSetEnabled(bool val)
{
    ui->manualSelectionAct->setEnabled(val);
}

void MainWindow::rectangleSelectionSetEnabled(bool val)
{
    ui->selectionAct->setEnabled(val);
}

void MainWindow::firstDrag()
{
    if (!dualScrollBarMutex.tryLock())
        return;
    ui->imageView->horizontalScrollBar()->setSliderPosition(ui->secondImage->horizontalScrollBar()->sliderPosition());
    ui->imageView->verticalScrollBar()->setSliderPosition(ui->secondImage->verticalScrollBar()->sliderPosition());
    dualScrollBarMutex.unlock();
}

void MainWindow::secondDrag()
{
    if (!dualScrollBarMutex.tryLock())
        return;
    ui->secondImage->horizontalScrollBar()->setSliderPosition(ui->imageView->horizontalScrollBar()->sliderPosition());
    ui->secondImage->verticalScrollBar()->setSliderPosition(ui->imageView->verticalScrollBar()->sliderPosition());
    dualScrollBarMutex.unlock();
}

void MainWindow::fast(bool val)
{

}

void MainWindow::processStream(bool val)
{
    ui->processAct->setEnabled(!val);
    ui->imageOpenAct->setEnabled(!val);
    ui->imagesOpenAct->setEnabled(!val);
    ui->videoOpenAct->setEnabled(!val);
    ui->webcamOpenAct->setEnabled(!val);

    controller->processStream(val);
    if (!val)
        QApplication::restoreOverrideCursor();
}

void MainWindow::on_extractAct_triggered()
{
    showSaveDialog();
}

QSlider* MainWindow::getSlider()
{
    return this->seekBar;
}


void MainWindow::saveSequencesButtonPressed()
{
    showExtract();
}

void MainWindow::importFileButtonPressed()
{
    int height;
    int width;
    int x, y;
    QImage *image;
    string tmp;

    QDir dir = QDir::currentPath();
    if (dir.exists("../results_text"))
        dir.cd("../results_text");

    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier de sauvegarde"),
                                                    dir.absolutePath(),
                                                    tr("Sauvegardes (*.lasmea_save)"));

    if (!fileName.isEmpty())
    {
        // File parser
        ifstream fichier(fileName.toStdString().c_str(), ios::in);

        // Test sur la validité du fichier
        fichier >> tmp;
        if (tmp != "LASMEA_SAVE_BEGIN")
        {
            QMessageBox::critical(0,
                                 QObject::tr("Erreur"),
                                 QObject::tr("Le fichier est corrumpu."));
        }

        // Lecture de la taille de l'image
        fichier >> height >> width;

        image = new QImage(width, height, QImage::Format_Mono);

        for (int i = 0; i < width; ++i)
            for (int j = 0; j < height; ++j)
                image->setPixel(i, j, 1);

        // Lecture des coordonnées
        while (!fichier.eof())
        {
            fichier >> x >> y;
            image->setPixel(x, y, 0);
        }

        if (image->isNull())
        {
            QMessageBox::information(this, tr("Erreur"),
                                     tr("Impossible de charger %1.").arg(fileName));
            delete image;

            return;
        }
        closeCurrent();
        emit imageChanged(image);
    }
    else
    {
        if (!controller->scene.isImageLoaded())
            resetButtons(false, TOUT);
    }
}

void MainWindow::setSelectionActEnabled(bool act)
{
    ui->selectionAct->setEnabled(act);
}

void MainWindow::displayError(QString err)
{
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(0,
                         QObject::tr("Erreur"),
                         err);
}

void MainWindow::displayInfo(QString title, QString info)
{
    QMessageBox::information(0,
        title,
        info);
}
