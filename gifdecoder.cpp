//---------------------------------------------------------
//  Name:       Chi Tay Ta
//  File:       gifdecoder.cpp
//  Project:    Qt-File-Explorer
//  Purpose:    Implementation of class GIFDecoder.
//---------------------------------------------------------

#include "gifdecoder.h"
#include "gifdisplayer.h"
#include <QDebug>
#include <QElapsedTimer>

GIFDecoder::GIFDecoder()
{
    timer = new QElapsedTimer;
}

GIFDecoder::GIFDecoder(GIFDisplayer* displayer, const QString& fileName, const QSize& size) : displayer(displayer), fileName(fileName), frameSize(size)
{
    timer = new QElapsedTimer;

    reader.setFileName(fileName);
    imageCountValue = reader.imageCount();
//    if (frameSize.isValid())
//    {
//        this->reader.setScaledSize(QSize(frameSize.width(), frameSize.height()));
//    }
    firstFrameDelay = reader.nextImageDelay();
    timer->start();
//    firstFramePixmap = QPixmap(QPixmap::fromImage(reader.read()));
    firstFrameImage = reader.read();
    firstFrameReadTime = timer->elapsed();
}

void GIFDecoder::setScaledSize(const QSize &size)
{
    frameSize = size;
//    if (frameSize.isValid())
//    {
//        reader.setScaledSize(QSize(frameSize.width(), frameSize.height()));
//    }
    if (!fileName.isNull())
    {
        reader.setFileName(fileName);
        firstFrameDelay = reader.nextImageDelay();
        timer->start();
//        firstFramePixmap = QPixmap(QPixmap::fromImage(reader.read()));
        firstFrameImage = reader.read();
        firstFrameReadTime = timer->elapsed();
    }
}

void GIFDecoder::setFileName(const QString& fileName)
{
    this->fileName = fileName;
    reader.setFileName(fileName);
    imageCountValue = reader.imageCount();
//    if (frameSize.isValid())
//    {
//        reader.setScaledSize(QSize(frameSize.width(), frameSize.height()));
//    }
    firstFrameDelay = reader.nextImageDelay();
    timer->start();
//    firstFramePixmap = QPixmap(QPixmap::fromImage(reader.read()));
    firstFrameImage = reader.read();
    firstFrameReadTime = timer->elapsed();
}

void GIFDecoder::clearBuffer()
{
    itemBuffer.clear();
    frameNumber = 0;
}

void GIFDecoder::resetDecoder()
{
    reader.setFileName(fileName);
//    if (frameSize.isValid())
//    {
//        reader.setScaledSize(QSize(frameSize.width(), frameSize.height()));
//    }
    firstFrameDelay = reader.nextImageDelay();
//    firstFramePixmap = QPixmap(QPixmap::fromImage(reader.read()));
    firstFrameImage = reader.read();
    for(int index = 0; index < 2; index++)
    {
        frameNumber = frameNumber + 1;
        itemBuffer.enqueue(FrameInfo(reader.read(), reader.nextImageDelay()));
    }
}

void GIFDecoder::onTimerDelayTimeOut()
{
    frameNumber = frameNumber + 1;
    if (frameNumber < imageCountValue)
    {
        itemBuffer.enqueue(FrameInfo(reader.read(), reader.nextImageDelay()));
    }
    else
    {
        reader.setFileName(fileName);
        frameNumber = 0;
        itemBuffer.enqueue(FrameInfo(reader.read(), reader.nextImageDelay()));
    }
}

uint GIFDecoder::imageCount()
{
    return this->imageCountValue;
}
