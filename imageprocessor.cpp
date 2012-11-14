#include "imageprocessor.h"

ImageProcessor::ImageProcessor(PassChain *passChain, IplImage *input)
    : passChain(passChain), input(input), output(0), inputs(0), outputs(0)
{
}

ImageProcessor::~ImageProcessor()
{
}

PassChain *ImageProcessor::getPassChain() const
{
    return passChain;
}

void ImageProcessor::setPassChain(PassChain *passChain)
{
    this->passChain = passChain;
}

IplImage *ImageProcessor::getInput() const
{
    return input;
}

void ImageProcessor::setInput(IplImage *input)
{
    this->input = input;
}

void ImageProcessor::setInputs(QList<IplImage *> *inputs)
{
    this->inputs = inputs;
}

IplImage *ImageProcessor::getOutput() const
{
    return output;
}

QList<IplImage *> *ImageProcessor::getOutputs() const
{
    return outputs;
}

void ImageProcessor::clearOutput()
{
    output = 0;
}

void ImageProcessor::run()
{
    if (inputs != 0)
    {
        outputs = new QList<IplImage *>();
        for(QList<IplImage *>::iterator beg = inputs->begin() ; beg != inputs->end(); ++beg)
        {
            outputs->push_back(passChain->process(*beg));
            emit getCircleList();
        }

        emit finished(outputs);
    }
    else
    {
        if (output != 0)
            cvReleaseImage(&output);

        if (input != 0) {
            output = passChain->process(input);
            emit finished(output);
        }
    }
}
