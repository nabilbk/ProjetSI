#include "controller.h"

#include <QDebug>
#include <QDate>
#include <QTime>
#include <fstream>

static bool bLoadImage = false;

void destroy(QGraphicsItem * i)
{
    if (!bLoadImage)
        delete i;
}

void destroyQImages(QImage * i)
{
    if (!bLoadImage)
        delete i;
}

void destroyResult(Result &i)
{
    if (!bLoadImage)
    {
        if (i.result != 0)
        {
            cvReleaseImage(&i.result);
            i.result = 0;
        }
        i.circles.clear();
    }
}


Controller::Controller(const QString &path):
    mode(0),
    image(0),
    qimages(0),
    resultIndex(0),
    currentMethod(0),
    edgeVal(1),
    binaryVal(1),
    circleVal(1),
    streamProcessActivated(false),
    closed(false),
    currentView(-1),
    edgeGroup(0),
    circleGroup(0),
    nanoScale(1),
    video(0)
{
    this->path = path;
    mainWindow = new MainWindow(this);
    mainWindow->showMaximized();
    parameterDock = new ParameterDock(this, mainWindow);
    roi = new ROIPass();
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, parameterDock);
    parameterDock->show();
    redPen.setColor(QColor(255,0,0));
    redPen.setWidth(1);
    bluePen.setColor(QColor(0,0,255));
    bluePen.setWidth(1);

    hough = new HoughCircleDetectionPass();
    custom = new CustomCircleDetectionPass();
    binary = new BinaryConversionPass();
    binaryInverse = new BinaryConversionPass(CV_THRESH_BINARY_INV);
    gaussian = new GaussianBlurPass(7);
    grayLevel = new GrayLevelPass();
    canny = new CannyFilterPass();
    angle = new AngleDetectionPass();
    sobel = new SobelFilterPass();
    meanshift = new MeanShiftFilterPass();

    mutex = new QMutex();
    waitcond = new QWaitCondition();

    imgProcessor = new ImageProcessor(&passChain, 0);
    videoProcessor = new VideoProcessor(0, mutex, waitcond);
    video = new VideoExtractor();

    connect(imgProcessor,   SIGNAL(finished(IplImage*)),              this,           SLOT(imageReady(IplImage*)));
    connect(imgProcessor,   SIGNAL(finished(QList<IplImage *> *)),    this,           SLOT(imagesReady(QList<IplImage *> *)));
    connect(imgProcessor,   SIGNAL(getCircleList()),                  this,           SLOT(getCircleList()));
    connect(videoProcessor, SIGNAL(nextFrame(IplImage*)),             this,           SLOT(nextFrame(IplImage*)));
    connect(mainWindow,     SIGNAL(destroyed()),                      this,           SLOT(onClose()));
    connect(mainWindow,     SIGNAL(changeGraphicMode(int)),           this,           SLOT(changeGraphicMode(int)));
    connect(&scene,         SIGNAL(confirmROI(QRectF*)),              this,           SLOT(confirmROI(QRectF*)));
    connect(&scene,         SIGNAL(scaleFinish()),                    mainWindow,     SLOT(showScale()));
    connect(&scene,         SIGNAL(offset(QPointF)),                  this,           SLOT(onClick(QPointF)));
    connect(this,           SIGNAL(setScale(QString)),                parameterDock,  SLOT(setScale(QString)));
    connect(&scene,         SIGNAL(angle(float)),                     this,           SLOT(receiveAngle(float)));
    connect(&scene,         SIGNAL(needToCleanItems()),               this,           SLOT(cleanItems()));

    connect(meanshift, SIGNAL(error(QString)),          mainWindow, SLOT(displayError(QString)));
    connect(sobel,     SIGNAL(error(QString)),          mainWindow, SLOT(displayError(QString)));
    connect(canny,     SIGNAL(error(QString)),          mainWindow, SLOT(displayError(QString)));
    connect(video,     SIGNAL(info(QString, QString)),  mainWindow, SLOT(displayInfo(QString, QString)));

    connect(this->video,    SIGNAL(newVideoPosition(int)), mainWindow->getSlider(),   SLOT(setValue(int)));
    connect(mainWindow->getSlider(), SIGNAL(sliderPressed()), videoProcessor,  SLOT(pause()));

    methodChanged(PERSONNALISEE);

    // Initialisation de la map de passes
    initIPassMap();
}

Controller::~Controller()
{
    reset();

    delete parameterDock;
    delete mainWindow;

    delete imgProcessor;
    delete videoProcessor;
    delete video;
    delete mutex;
    delete waitcond;

    delete hough;
    delete custom;
    delete meanshift;
    delete binary;
    delete gaussian;
    delete grayLevel;
    delete canny;
    delete angle;
}

void Controller::loadImage(QImage *image)
{
    mainWindow->resetButtons(true, IMAGE);

    bLoadImage = true;

    this->image = ImageConverter::QImageToIplImage(image);

    QGraphicsPixmapItem *imageItem = scene.addPixmap(QPixmap::fromImage(*image));
    mainWindow->sceneChanged(&scene, imageItem);

    currentView = IMAGE;

    //Permet de rappeler le changement de méthode pour la méthode actuel
    //nécessaire pour le rapporteur par exemple
    methodChanged(mode);

    delete image;
}

void Controller::loadImages(QList<QImage *> *qimages)
{
    mainWindow->resetButtons(true, IMAGES);

    bLoadImage = true;

    this->qimages = qimages;

    // on crée autant d'élément dans resultList que d'images à charger.
    for(QList<QImage *>::iterator beg = qimages->begin(); beg != qimages->end() ; ++beg)
    {
        Result res;
        res.result = 0;
        resultList.push_back(res);
        images.push_back(ImageConverter::QImageToIplImage(*beg));
    }
    // On affiche la première image de la liste
    image = images.first();
    QGraphicsPixmapItem *imageItem = scene.addPixmap(QPixmap::fromImage(*qimages->first()));
    mainWindow->sceneChanged(&scene, imageItem);

    currentView = IMAGE;

    //Permet de rappeler le changement de méthode pour la méthode actuel
    //nécessaire pour le rapporteur par exemple
    methodChanged(mode);
}


void Controller::nextFrame(IplImage *image)
{
    if ((currentView == WEBCAM || currentView == VIDEO) && image)
    {
        binaryScene.clear();

        this->image = image;

        binaryScene.addPixmap(ImageConverter::IplImageToQPixmap(image));
        mainWindow->binarySceneChanged(&binaryScene);

        if (!videoProcessor->isPaused())
        {
            videoProcessor->play();
            if (streamProcessActivated)
                process();
        }
    }
}

void Controller::loadVideo(QString &string)
{
    mainWindow->resetButtons(true, VIDEO);

    //video = new VideoExtractor();
    if (video->loadVideo(string.toStdString()) < 0)
        return;

    videoProcessor->setInput(video);
    videoProcessor->start();

    currentView = VIDEO;
}

void Controller::openWebcam()
{
    reset();
    mainWindow->hand();
    mainWindow->resetButtons(true, WEBCAM);

    //video = new VideoExtractor();
    if (!video->loadDirectStream())
    {
        QMessageBox::critical(0,
                             QObject::tr("Erreur"),
                             QObject::tr("Aucune caméra n'est trouvée."));
        mainWindow->resetButtons(false, TOUT);
    }
    else
    {
        videoProcessor->setInput(video);
        videoProcessor->start();

        currentView = WEBCAM;

        play();
    }
}

void Controller::customPassChain()
{
    passChain.clear();

    passChain.add(grayLevel);

    passChain.add(roi);
    passChain.add(binary);
    passChain.add(gaussian);
    passChain.add(binary);

    // Filtrage des contours

//    for (int i = 0; i < 2; ++i)
//    {
//        passChain.add(gaussian);
//        passChain.add(binary);
//    }

    passChain.add(custom);
}

void Controller::dropPassChain()
{
    passChain.clear();
    passChain.add(roi);
    passChain.add(binary);
    passChain.add(binary);
    passChain.add(canny);
    binary->setMode(CV_THRESH_BINARY_INV);
    passChain.add(binary);
    passChain.add(angle);
}

void Controller::meanShiftPassChain()
{
    passChain.clear();
    passChain.add(meanshift);

//    passChain.add(gaussian);
//    passChain.add(binary);
}

void Controller::dynamicPassChain()
{
    passChain.clear();

    for (int i = 0; i < dynamicList.count(); ++i)
    {
        passChain.add(passFactory(dynamicList.at(i)));
    }
}

void Controller::houghPassChain()
{
    passChain.clear();
    passChain.add(roi);
    passChain.add(hough);
}

void Controller::process()
{
    // Vérification de la présence d'une image pour lancer le traitement, qu'on peut prendre le mutex
    // et que la méthode de détection peut être traité (par exemple le rapporteur n'a pas de traitement)
    if (image == 0 || ( (currentView == WEBCAM || currentView == VIDEO) && (mode != PERSONNALISEE && mode != MEANSHIFT && mode != DYNAMIC)) || mutex->tryLock() == false)
        return;
    cleanItems();

    bLoadImage = false;
    imgProcessor->setInput(image);
    imgProcessor->start();

    emit processReady(false);
    mutex->unlock();
}

void Controller::processBatch()
{
    // Vérification de la présence d'une image pour lancer le traitement.
    if (image == 0 || mutex->tryLock() == false)
        return;
    cleanItems();

    emit processReady(false);
    bLoadImage = false;

    // On détruit les anciens résultats s'ils existent déjà
    for_each(resultList.begin(), resultList.end(), destroyResult);

    // On passe l'index à 0 (car on va l'incrémenter dans getCircleList)
    resultIndex = 0;

    // On passe la liste d'images à traiter
    imgProcessor->setInputs(&images);
    imgProcessor->start();



    mutex->unlock();
}


void Controller::nextImages()
{
    int index;
    // On regarde où est l'image actuelle dans la liste, si ce n'est pas la dernière, on exécute
    if ( (index = images.indexOf(image)) < (images.size() - 1))
    {
        ++index;
        image = images.at(index);

        cleanItems();

        scene.clear();
        binaryScene.clear();
        circles.clear();

        QGraphicsPixmapItem *imageItem = scene.addPixmap(QPixmap::fromImage(*qimages->at(index)));
        mainWindow->sceneChanged(&scene, imageItem);

        // Si on a déjà calculé un résultat pour l'image
        if (resultList.at(index).result != 0)
        {
            const Result &res = resultList.at(index);
            binaryScene.addPixmap(ImageConverter::IplImageToQPixmap(res.result));
            mainWindow->binarySceneChanged(&binaryScene);

            circles = res.circles;

            for_each(circles.begin(), circles.end(), bind1st(mem_fun(&Controller::draw), this));

            edgeGroup = scene.createItemGroup(edgeList);
            circleGroup = scene.createItemGroup(circleList);

            edgeGroup->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
            circleGroup->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);

            if (!edgeVal)
                edgeGroup->setVisible(false);
            if (!circleVal)
                circleGroup->setVisible(false);
        }
        updateStats();
    }
}

void Controller::previousImages()
{
    int index;
    // On regarde où est l'image actuelle dans la liste, si ce n'est pas la première, on exécute
    if ( (index = images.indexOf(image)) > 0)
    {
        --index;
        image = images.at(index);

        cleanItems();

        scene.clear();
        binaryScene.clear();
        circles.clear();

        QGraphicsPixmapItem *imageItem = scene.addPixmap(QPixmap::fromImage(*qimages->at(index)));
        mainWindow->sceneChanged(&scene, imageItem);

        // Si on a déjà calculé un résultat pour l'image
        if (resultList.at(index).result != 0)
        {
            const Result &res = resultList.at(index);
            binaryScene.addPixmap(ImageConverter::IplImageToQPixmap(res.result));
            mainWindow->binarySceneChanged(&binaryScene);

            circles = res.circles;

            for_each(circles.begin(), circles.end(), bind1st(mem_fun(&Controller::draw), this));

            edgeGroup = scene.createItemGroup(edgeList);
            circleGroup = scene.createItemGroup(circleList);

            edgeGroup->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
            circleGroup->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);

            if (!edgeVal)
                edgeGroup->setVisible(false);
            if (!circleVal)
                circleGroup->setVisible(false);
        }

        updateStats();
    }
}


void Controller::cleanItems()
{
    if (edgeGroup != 0)
    {
        if (edgeList.size() > 0)
        {
            for_each(edgeList.begin(),edgeList.end(),destroy);
            edgeList.clear();
        }

        scene.destroyItemGroup(edgeGroup);
        edgeGroup = 0;
    }
    if (circleGroup != 0)
    {

        if (circleList.size() > 0)
        {
            for_each(circleList.begin(),circleList.end(),destroy);
            circleList.clear();
        }

        scene.destroyItemGroup(circleGroup);
        circleGroup = 0;
    }

    circles.clear();
    custom->clear();
}

void Controller::play()
{
    if (videoProcessor->videoLoaded())
    {
        videoProcessor->start();
        videoProcessor->play();
    }
    //waitcond->wakeAll();
}

void Controller::pause()
{
    videoProcessor->pause();
}

void Controller::reset()
{
    // On prend le mutex pour éviter les conflits
    QMutexLocker locker(mutex);

    // Si le traitement d'image en direct est activé, on le désactive le temps de la fermeture
    if (streamProcessActivated)
    {
        streamProcessActivated = false;
        mainWindow->processStream(false);
    }

    // On arrête les threads en cours de fonctionnement
    imgProcessor->terminate();
    videoProcessor->stop();
    videoProcessor->terminate();

    // Empéche le traitement simultanée de plusieurs images dans le futur
    imgProcessor->setInputs(0);

    // On enlève les images et les traitements (cercles, contours, etc)
    cleanItems();

    roi->clear();

    scene.clear();
    binaryScene.clear();

    // Si le programme n'est pas fermé, on affiche ou non la deuxième qgraphicsview (l'affichage binaire)
    // en fonction de la valeur dans les options (et de la méthode)
    if (!closed)
    {
        if (currentMethod != hough && binaryVal == true)
            mainWindow->showBinaryImage();
        else
            mainWindow->hideBinaryImage();
    }

    // la vidéo
    if (video != 0)
    {
        video->terminate();
        //delete video;
        //video = 0;
    }

    // Si le traitement groupé était actif
    if (images.size()!= 0)
    {
        for(QList<IplImage *>::iterator beg = images.begin(); beg != images.end(); ++beg)
            cvReleaseImage(&(*beg));
        images.clear();
        image = 0;

        for_each(qimages->begin(), qimages->end(), destroyQImages);
        delete qimages;
        qimages = 0;

        // Supprime les éléments de la liste de résultats
        for_each(resultList.begin(), resultList.end(), destroyResult);
        resultList.clear();

        resultIndex = 0;

        // Passe le output de imgProcessor à 0, car sinon, on appelle cvRelease 2 fois
        // sur le même élément
        imgProcessor->clearOutput();
    }
    else
    {
        // On supprime l'image
        if (currentView != VIDEO)
            if (image != 0)
            {
                cvReleaseImage(&image);
            }
         image = 0;
    }

    // Les cercles
    circles.clear();
    custom->clear();

    // La tengeante


    currentView = -1;

    mainWindow->resetButtons(false, TOUT);

    mainWindow->hand();
    // Le programme est prêt (la souris n'a plus de sablier)
}

void Controller::draw(Circle c)
{ 
    if (currentMethod == hough)
    {
        c.x += roi->getXOffset();
        c.y += roi->getYOffset();
    }

    circleList.push_back(scene.addEllipse(c.x - c.r, c.y - c.r, c.r * 2, c.r * 2, redPen));

    if (currentMethod == custom)
    {
        QPolygon p;

        for (list<pair<int, int> >::iterator beg = c.edge.begin(), end = c.edge.end(); beg != end; ++beg)
            p << QPoint(beg->first, beg->second);

        edgeList.push_back(scene.addPolygon(p, bluePen));
    }
}

void Controller::showParameter(bool check)
{
    if (check)
        parameterDock->show();
    else
        parameterDock->hide();

}

void Controller::tickParameter(bool val)
{
    emit tickShowParameterOption(val);
}

void Controller::imageReady(IplImage *image)
{
    this->filteredImage = image;
    emit processReady(true);
    {
        circles = currentMethod->getCircles();
        if (currentView == WEBCAM || currentView == VIDEO)
        {
            if (mutex->tryLock() == false)
                return;
            scene.clear();
            QGraphicsPixmapItem *imageItem = scene.addPixmap(ImageConverter::IplImageToQPixmap(image));
            mutex->unlock();
            mainWindow->sceneChanged(&scene, imageItem);
            qDebug() << "test";
        }
        else
        {
            binaryScene.clear();
            // S'il y a un traitement groupé, on ajoute le résultat à la liste, au bon endroit
            if (images.size() != 0)
            {
                int index = images.indexOf(this->image);
                resultList[index].result = image;
                resultList[index].circles = circles;
            }
            binaryScene.addPixmap(ImageConverter::IplImageToQPixmap(image));
            mainWindow->binarySceneChanged(&binaryScene);
        }

        if (mode != MEANSHIFT)
            for_each(circles.begin(), circles.end(), bind1st(mem_fun(&Controller::draw), this));
        else
            circles.clear();

        edgeGroup = scene.createItemGroup(edgeList);
        circleGroup = scene.createItemGroup(circleList);

        if (!edgeVal)
            edgeGroup->setVisible(false);
        if (!circleVal)
            circleGroup->setVisible(false);

        if (custom->getAutoDetect())
        {
            mainWindow->hand();
            changeGraphicMode(SCROLLHAND);
        }

        updateStats();

//        if (!videoProcessor->isPaused() && streamProcessActivated && (currentView == VIDEO || currentView == WEBCAM))
//            waitcond->wakeAll();
    }

    if (mode == GOUTTE)
    {
        if (angle->getLineHorizontale() != NULL && angle->getLineTangente()!= NULL)
        {
            if (angle->getLineHorizontale() != NULL)
               scene.addLine(*angle->getLineHorizontale(), bluePen);
            if (angle->getLineTangente() != NULL)
               scene.addLine(*angle->getLineTangente(), bluePen);
        }
    }
}

void Controller::imagesReady(QList<IplImage *> *images)
{
    emit processReady(true);
    {
        // Nécessaire car si on appelle process après, le thread exécute le traitement groupé (car inputs != 0)
        imgProcessor->setInputs(0);

        resultIndex = 0;

        for(QList<IplImage *>::iterator beg = images->begin(); beg != images->end(); ++beg, ++resultIndex)
            resultList[resultIndex].result = *beg;

        binaryScene.clear();
        // Ajoute l'image traitée de l'image actuel dans la binaryscene.
        binaryScene.addPixmap(ImageConverter::IplImageToQPixmap(resultList[this->images.indexOf(image)].result));
        mainWindow->binarySceneChanged(&binaryScene);

        circles.clear();
        // Affiche les cercles de l'image actuelle grâce à son index
        circles = resultList[this->images.indexOf(image)].circles;

        for_each(circles.begin(), circles.end(), bind1st(mem_fun(&Controller::draw), this));

        edgeGroup = scene.createItemGroup(edgeList);
        circleGroup = scene.createItemGroup(circleList);

        edgeGroup->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
        circleGroup->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);

        if (!edgeVal)
            edgeGroup->setVisible(false);
        if (!circleVal)
            circleGroup->setVisible(false);

        if (custom->getAutoDetect())
        {
            mainWindow->hand();
            changeGraphicMode(SCROLLHAND);
        }

        updateStats();
    }

    if (angle->getLineHorizontale() != NULL && angle->getLineTangente()!= NULL)
    {
        if (angle->getLineHorizontale() != NULL)
           scene.addLine(*angle->getLineHorizontale(), bluePen);
        if (angle->getLineTangente() != NULL)
          scene.addLine(*angle->getLineTangente(), bluePen);
    }
}

void Controller::updateStats()
{
    double radius_avg       = 0;
    double radius_sd        = 0;
    double radius_min       = circles.back().r;
    double radius_max       = circles.back().r;
    double irregularity_avg = 0;
    double irregularity_sd  = 0;
    double calculatedAngle = angle->getAngle();
    for (list<Circle>::iterator beg = circles.begin(), end = circles.end(); beg != end; ++beg)
    {
        radius_avg     += beg->r;
        irregularity_avg += beg->i;

        if (beg->r < radius_min)
            radius_min = beg->r;

        if (beg->r > radius_max)
            radius_max = beg->r;
    }

    radius_avg       /= double(circles.size());
    irregularity_avg /= double(circles.size());

    for (list<Circle>::iterator beg = circles.begin(), end = circles.end(); beg != end; ++beg)
    {
        radius_sd       += (beg->r - radius_avg)       * (beg->r - radius_avg);
        irregularity_sd += (beg->i - irregularity_avg) * (beg->i - irregularity_avg);
    }

    radius_sd        /= double(circles.size());
    irregularity_sd  /= double(circles.size());

    parameterDock->setStat_radius_avg      (radius_avg * nanoScale);
    parameterDock->setStat_radius_sd       (radius_sd * nanoScale);
    parameterDock->setStat_radius_min      (radius_min * nanoScale);
    parameterDock->setStat_radius_max      (radius_max * nanoScale);
    parameterDock->setStat_irregularity_avg(irregularity_avg * nanoScale);
    parameterDock->setStat_irregularity_sd (irregularity_sd * nanoScale);
    parameterDock->setStat_angle            (calculatedAngle);
}

int Controller::getMode()
{
    return mode;
}

bool Controller::getStreamProcessActivated()
{
    return streamProcessActivated;
}

void Controller::setParameters()
{
    if (currentMethod == hough)
    {
        hough->setParameters(parameterDock->getParam_H_dp(),
                             parameterDock->getParam_H_minDist(),
                             parameterDock->getParam_H_param1(),
                             parameterDock->getParam_H_param2(),
                             parameterDock->getParam_H_minRadius(),
                             parameterDock->getParam_H_maxRadius());
    }
    if (currentMethod == custom)
    {
        custom->setParameters(parameterDock->getParam_C_minPoints(),
                              parameterDock->getParam_C_minRadius(),
                              parameterDock->getParam_C_maxRadius(),
                              custom->getAutoDetect());
        binary->setThreshold(parameterDock->getParam_C_threshold());
    }
    if (currentMethod == angle)
    {
        canny->setParameters(parameterDock->getParam_Ca_threshold(),
                             parameterDock->getParam_Ca_threshold2(),
                             parameterDock->getParam_Ca_apertureSize());
        binary->setThreshold(parameterDock->getParam_C_threshold());
        angle->setParameters(parameterDock->getParam_Ca_horizontalBand(),
                             parameterDock->getParam_Ca_tangentSensibility());
    }
    if (currentMethod == meanshift)
    {
        meanshift->setParameters(parameterDock->getParam_meanshift_level(),
                              parameterDock->getParam_meanshift_thresh1(),
                              parameterDock->getParam_meanshift_thresh2(),
                              parameterDock->getParam_meanshift_forceProcess());

        meanShiftPassChain();
        if (parameterDock->getParam_meanshift_binary())
            passChain.add(binary);
        if (parameterDock->getParam_meanshift_binaryInverse())
            passChain.add(binaryInverse);

        process();
    }
}

void Controller::changeGraphicMode(int mode)
{
  scene.setGraphicMode(mode);
}

void Controller::methodChanged(int index)
{
   mode = index;
   switch(index)
    {
    case PERSONNALISEE:
        currentMethod = custom;
        parameterDock->setBinaryVal(binaryVal);
        mainWindow->methodChanged(mode);
        if (binaryVal == true)
            mainWindow->showBinaryImage();
        customPassChain();
        break;

    case HOUGH:
        processStream(false);
        currentMethod = hough;
        mainWindow->methodChanged(mode);
        mainWindow->hideBinaryImage();
        parameterDock->setBinaryVal(binaryVal);
        houghPassChain();
        break;

    case GOUTTE:
        processStream(false);
        parameterDock->setBinaryVal(binaryVal);
        mainWindow->methodChanged(mode);
        if (binaryVal == true)
            mainWindow->showBinaryImage();
        dropPassChain();
        break;

    case RAPPORTEUR:
        currentMethod = angle;
        processStream(false);
        mainWindow->methodChanged(mode);
        mainWindow->protractor();
        mainWindow->hideBinaryImage();
        // TO DO
        break;

    case MEANSHIFT:
        currentMethod = meanshift;
        parameterDock->setBinaryVal(binaryVal);
        mainWindow->methodChanged(mode);
        if (binaryVal == true)
            mainWindow->showBinaryImage();
        meanShiftPassChain();
        break;

    case DYNAMIC:
        parameterDock->setBinaryVal(binaryVal);
        mainWindow->methodChanged(mode);
        if (binaryVal == true)
            mainWindow->showBinaryImage();
        dynamicPassChain();
        break;
    }
    imgProcessor->setPassChain(&passChain);
    setParameters();

    // Si le traitement groupé était actif
    if (images.size()!= 0)
    {
        mainWindow->setSelectionActEnabled(false);
    }
}

void Controller::scaleMode()
{
    scene.setMode(1);
}

void Controller::scale(qreal realDist)
{
    ostringstream oss;
    double distance;

    // distance = racine( (Xa - Xb)² + (Ya - Yb)² )
    distance = sqrt( ((scene.getFirst().x() - scene.getSecond().x())
                   * (scene.getFirst().x() - scene.getSecond().x()))
                   + ((scene.getFirst().y() - scene.getSecond().y()))
                   * (scene.getFirst().y() - scene.getSecond().y()));
    nanoScale = realDist / distance;
    oss << realDist << " nm";
    emit setScale(QString(oss.str().c_str()));
    scene.setMode(0);
    if (resultIndex > 0)
        processBatch();
    else
        process();
}

void Controller::drawPoint(pair<int, int> point)
{
    static int i=0;
    if (++i%2 == 1)
        scene.addRect(point.first, point.second, 1, 1,bluePen);
}

void Controller::confirmROI(QRectF *r)
{
    //! Le code ici est incorrect
 if (r->width() == 0 || r->height() == 0)
      roi->setParameters(0,0,scene.width(),scene.height());
  roi->setParameters(r->x(), r->y(), r->width(), r->height());
}

void Controller::edgeBox_checked(bool val)
{
    edgeVal = val;
    if (edgeGroup != 0)
        edgeGroup->setVisible(val);
}

void Controller::binaryBox_checked(bool val)
{
    binaryVal = val;
    if (val)
        mainWindow->showBinaryImage();
    else
        mainWindow->hideBinaryImage();
}

void Controller::circleBox_checked(bool val)
{
    circleVal = val;
    if (circleGroup != 0)
        circleGroup->setVisible(val);
}

void Controller::onClose()
{
    closed = true;

    this->deleteLater();
}

void Controller::fastForward(bool enabled)
{
    videoProcessor->setForward(enabled);
}

void Controller::setAutoMode(bool val)
{
    custom->setAutoDetect(val);
    //S'il y a une zone de selection on la supprime
    if (!val && scene.getRegionOfInterest() != 0)
    {
        scene.clearRegionOfInterest();
        roi->clear();
    }
}

void Controller::onClick(QPointF mouse)
{
    if (!mutex->tryLock())
        return;
    custom->setOffset(mouse.x(), mouse.y());
    // Vérification de la présence d'une image pour lancer le traitement.
    if (image == 0)
        return;

    bLoadImage = false;
    mutex->unlock();

    imgProcessor->setInput(image);
    imgProcessor->start();
    emit processReady(false);
}

void Controller::processStream(bool process)
{
    streamProcessActivated = process;

    if (!process)
        emit processReady(true);

    // Gestion du changement de méthode pendant un traitement
    parameterDock->processStreamEnabled(process);
}

void Controller::getCircleList()
{
    // Met les cercles calculés dans la liste
    if (resultIndex < resultList.size())
        resultList[resultIndex].circles = currentMethod->getCircles();

    // Incrémente l'index pour le prochain appel
    ++resultIndex;
}

void Controller::receiveAngle(float angle)
{
    if(angle > 180)
        angle = 360 - angle;
    parameterDock->setStat_angle (angle);
}

void Controller::openHelp()
{
    QDesktopServices::openUrl(QUrl("file:///" + path + "/manuel_utilisateur.pdf", QUrl::TolerantMode));
}

void Controller::extractSequence(int n)
{
    if (video)
        video->extractImagesFromVideo(n);
}

void Controller::restartVideo()
{
//    reset();
//    if (!mutex->tryLock())
//        return;
//    video->terminate();
//    loadVideo(video->fileName);
//    mutex->unlock();

    mutex->lock();
    video->restartVideo(1);
    videoProcessor->start();
    mutex->unlock();
}

void Controller::sliderMoved()
{
    video->goToPosition(mainWindow->getSlider()->value());
}

void Controller::dynamicMethodSetted(QList<QString> list)
{
    this->dynamicList = list;
    parameterDock->dynamicMethodSetted(list);
    dynamicPassChain();
}

IPass* Controller::passFactory(const QString &str)
{
    return iPassMap[str];
}

void Controller::initIPassMap()
{
    iPassMap.clear();

    iPassMap.insert("Niveaux de gris", grayLevel);
    iPassMap.insert("Flou gaussien", gaussian);
    iPassMap.insert("Binarisation", binary);
    iPassMap.insert("Selection d'une région d'interêt", roi);
    iPassMap.insert("Filtre de Canny", canny);
    iPassMap.insert("Détection des contours", sobel);
    iPassMap.insert("Binarisation inverse", binaryInverse);
    iPassMap.insert("Mean Shift", meanshift);
}

void Controller::saveResultButtonPressed()
{
    if (!filteredImage)
    {
        QMessageBox::critical(0,
                             QObject::tr("Erreur"),
                             QObject::tr("Aucune image à sauvegarder."));
        return;
    }

    QDir dir = QDir::currentPath();
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    stringstream ss;
    QString fileName, folderName;
    QPixmap tmp;
    static int i = 0;

    ss << date.toString(Qt::TextDate).toStdString();
    folderName = ss.str().c_str();

    ss.clear();
    ss << time.toString("hh_mm_ss").toStdString();
    ss << "_";
    ss << i++;
    ss << ".jpg";

    fileName = ss.str().c_str();

    if (!dir.exists("../Results"))
        dir.mkdir("../Results");
    dir.cd("../Results");
    if (!dir.exists("./" + folderName))
        dir.mkdir("./" + folderName);
    dir.cd("./" + folderName);

    if (!(tmp = ImageConverter::IplImageToQPixmap(filteredImage)))
    {
        qDebug() << "Impossible de convertir l'image" << endl;
        return;
    }

    if (!tmp.save(fileName, "JPG", -1))
    {
        QMessageBox::information(0,
                             QObject::tr("Erreur"),
                             QObject::tr("Impossible de sauvegarder l'image traitée."));
        qDebug() << "Impossible de sauvegarder l'image" << endl;
    }
    else
    {
        QFile().rename(fileName, dir.path() + "/" + fileName);
        QMessageBox::information(0,
                             QObject::tr("Image sauvegardée"),
                             QObject::tr("L'image traitée a bien été sauvegardée."));
    }
}


void Controller::exportResultButtonPressed()
{
    if (!filteredImage)
    {
        QMessageBox::critical(0,
                             QObject::tr("Erreur"),
                             QObject::tr("Aucune image à sauvegarder."));
        return;
    }

    QDir dir = QDir::currentPath();
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    stringstream ss;
    QString fileName, folderName;
    QImage tmp;
    int value;
    static int i = 0;

    ss << date.toString(Qt::TextDate).toStdString();
    folderName = ss.str().c_str();

    ss.clear();
    ss << time.toString("hh_mm_ss").toStdString();
    ss << "_";
    ss << i++;
    ss << ".lasmea_save";

    if (!dir.exists("../Results_text"))
        dir.mkdir("../Results_text");
    dir.cd("../Results_text");
    if (!dir.exists("./" + folderName))
        dir.mkdir("./" + folderName);
    dir.cd("./" + folderName);

    fileName = dir.path().toStdString().c_str();
    fileName.append("/");
    fileName.append(ss.str().c_str());

    tmp = ImageConverter::IplImageToQImage(filteredImage);

    ofstream fichier(fileName.toStdString().c_str(), ios::out | ios::trunc);

    if(!fichier)
    {
        QMessageBox::critical(0,
                             QObject::tr("Erreur"),
                             QObject::tr("Création du fichier de sauvegarde impossible."));
        return;
    }
    fichier << "LASMEA_SAVE_BEGIN" << endl;
    fichier << tmp.height() << endl;
    fichier << tmp.width() << endl;
    for (int k = 0; k < tmp.width(); ++k)
    {
        for (int j = 0; j < tmp.height(); ++j)
        {
            value = qGray(tmp.pixel(k, j));
            if (value && value != 255)
            {
                QMessageBox::critical(0,
                                     QObject::tr("Erreur"),
                                     QObject::tr("Votre image doit être binarisée."));
                fichier.close();
                QFile::remove(fileName);
                return;
            }
            else
            {
                if (!value)
                {
                    fichier << k << " " << j << endl;
                }
            }
        }
    }

    fichier.close();

    QMessageBox::information(0,
                         QObject::tr("Image sauvegardée"),
                         QObject::tr("Fichier crée."));
}
