#include "videoextractor.h"
#include "imageconverter.h"

#include <QObject>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QDate>
#include <QTime>
#include <iostream>
#include <opencv2/opencv.hpp>
//#include <opencv/highgui.h>
#include <QMessageBox>

using namespace std;
using namespace cv;

VideoExtractor::VideoExtractor() : video(0), frame(0), webcam(false), extractSequence(0)
        , framesRetrieved(0)
{
    timeFormat.setPattern("^([0-9]{2}):([0-9]{2}):([0-9]{2})$");
}

VideoExtractor::VideoExtractor(const string &videoPath) : webcam(false), extractSequence(0)
        , framesRetrieved(0)
{
    video = cvCreateFileCapture(videoPath.c_str());
    timeFormat.setPattern("^([0-9]{2}):([0-9]{2}):([0-9]{2})$");
}

VideoExtractor::~VideoExtractor()
{
    cvReleaseCapture(&video);
    free(video);
}

VideoExtractor::operator IplImage*()
{
    return frame;
}

IplImage *VideoExtractor::nextFrame(int delay)
{
    if (!video)
       return 0;

    if (delay >= 0)
        cvWaitKey(delay);

    cvGrabFrame(video);
    frame = cvQueryFrame(video);
    ++framesRetrieved;

    if (webcam)
        cvFlip(frame, frame, 1);

    // Extraction de la séquence
    if (extractSequence)
    {
        saveFrame();
        if (extractSequence > 0)
            if(--extractSequence == 0)
            {
                emit info("Sauvegarde terminée", "La sauvegarde de la séquence est terminée.");
//                QMessageBox::information(0,
//                                    QObject::tr("Sauvegarde terminée"),
//                                    QObject::tr("La sauvegarde de la séquence est terminée."));
            }
    }

    emit newVideoPosition(cvGetCaptureProperty(video, CV_CAP_PROP_POS_AVI_RATIO) * 100);

    return frame;
}

int VideoExtractor::loadVideo(const string &videoPath)
{
    if (video != 0)
        cvReleaseCapture(&video);

    fileName = videoPath.c_str();

    if (!(video = cvCreateFileCapture(videoPath.c_str())))
    {
        qDebug() << "Vidéo ne peux pas être ouverte" << endl;
        return -1;
    }

    webcam = false;

    framesRetrieved = 0;

    return 0;
}

void VideoExtractor::terminate()
{
    cvReleaseCapture(&video);
    free(video);
    video = 0;
    frame = 0;
}

int VideoExtractor::loadDirectStream()
{
    static int attempt = 0;
    int err = 1;

    while (!(video = cvCreateCameraCapture(-1)) && ++attempt < 10)
    {
        #ifdef EBUG
            cout << "No camera found." << endl;
        #endif
    }
    if (attempt >= 10)
        err = 0;
    else
        webcam = true;

    attempt = 0;

    return err;
}

bool VideoExtractor::isOver()
{
    if (video == 0 || !cvGrabFrame(video))
       return true;

    return false;
}

IplImage* VideoExtractor::advance(int frameNumber)
{
    if (video == 0)
       return 0;

    for (int i = 0; i < frameNumber; ++i)
        if (!cvGrabFrame(video))
            break;

    frame = cvQueryFrame(video);

    return frame;
}

void VideoExtractor::goTo(int ms)
{
    if (!video)
        return;

    cvSetCaptureProperty(video, CV_CAP_PROP_POS_MSEC, ms);
}

void VideoExtractor::goTo(string time)
{
    if (!video || timeFormat.indexIn(time.c_str()) == -1)
        return;

    // Milliseconds conversion.
    double h  = timeFormat.cap(1).toInt() * 3600 * 1000,
           m  = timeFormat.cap(2).toInt() * 60   * 1000,
           s  = timeFormat.cap(3).toInt()        * 1000;
    double ms = h + m + s;

    cvSetCaptureProperty(video, CV_CAP_PROP_POS_MSEC, ms);
}

bool VideoExtractor::isWebcam()
{
    return webcam;
}

void VideoExtractor::stopRecording()
{
    webcam = false;
    cvReleaseCapture(&video);
    free(video);
    video = 0;
}

void VideoExtractor::extractImagesFromVideo(int n)
{
    this->extractSequence = n;
}

void VideoExtractor::saveFrame()
{
    if (!frame)
        return;

    QDir dir = QDir::currentPath();
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    stringstream ss;
    QString fileName, videoName, folderName;
    QPixmap tmp;
    static int i = 0;

    videoName = this->fileName.section('/', -1).section('.', 0, 0);

    ss << videoName.toStdString();
    ss << "_";
    ss << date.toString(Qt::TextDate).toStdString();
    folderName = ss.str().c_str();

    ss.clear();
    ss << videoName.toStdString();
    ss << "_";
    ss << time.toString("hh_mm_ss").toStdString();
    ss << "_";
    if (extractSequence > 0)
        ss << extractSequence;
    else
    {
        ss << "continue";
        ss << ++i;
    }
    ss << ".jpg";

    fileName = ss.str().c_str();

    if (!dir.exists("../VideoCapture"))
        dir.mkdir("../VideoCapture");
    dir.cd("../VideoCapture");
    if (!dir.exists("./" + folderName))
        dir.mkdir("./" + folderName);
    dir.cd("./" + folderName);

    if (!(tmp = ImageConverter::IplImageToQPixmap(frame)))
    {
        qDebug() << "Impossible de convertir l'image" << endl;
        return;
    }

    if (!tmp.save(fileName, "JPG", -1))
        qDebug() << "Impossible de sauvegarder l'image" << endl;
    else
        QFile().rename(fileName, dir.path() + "/" + fileName);

}

void VideoExtractor::restartVideo(int method)
{
    if (framesRetrieved < 5 || !video)
        return;
    switch (method)
    {
    case 0:
        terminate();
        loadVideo(fileName.toStdString());
        break;
    case 1:
        goTo(0);
        break;
    }

    framesRetrieved = 0;
}

void VideoExtractor::goToPosition(int val)
{
    cvSetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES, (val/100.0) * cvGetCaptureProperty(video,  CV_CAP_PROP_FRAME_COUNT));
}
