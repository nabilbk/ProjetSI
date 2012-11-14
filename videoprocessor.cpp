#include "videoprocessor.h"
#include <QDebug>

VideoProcessor::VideoProcessor(VideoExtractor *input, QMutex *mutex, QWaitCondition *waitcond)
    : input(input), output(0), paused(false), firstTime(true),
    mutex(mutex), waitcond(waitcond), isStopped(false), forward(170)
{
}

VideoProcessor::~VideoProcessor()
{
}

VideoExtractor *VideoProcessor::getInput() const
{
    return input;
}

void VideoProcessor::setInput(VideoExtractor *input)
{
    this->input = input;
}

IplImage *VideoProcessor::getOutput() const
{
    return output;
}

void VideoProcessor::run()
{
    mutex->lock();
    while (!input->isOver())
    {
        mutex->unlock();

        mutex->lock();
        output = input->nextFrame(30);
        mutex->unlock();

        if (input->isWebcam())
            msleep(90);
        else
            msleep(forward);

        emit nextFrame(output);

        if (isPaused())
        {
            mutexPause.lock();
            waitcond->wait(&mutexPause);
            mutexPause.unlock();
        }

        mutex->lock();
    }
    mutex->unlock();
}

bool VideoProcessor::videoLoaded()
{
    return (input != 0);
}

void VideoProcessor::pause()
{
    paused = true;
}

void VideoProcessor::play()
{
    paused = false;
    waitcond->wakeAll();
}

bool VideoProcessor::isPaused()
{
    return paused;
}

void VideoProcessor::stop()
{
    output = 0;
    input = 0;
    isStopped = true;
}

void VideoProcessor::setForward(bool enabled)
{
    enabled?this->forward=80:this->forward=170;
}

