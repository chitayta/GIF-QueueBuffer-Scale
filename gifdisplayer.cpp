//---------------------------------------------------------
//  Name:       Ta Chi Tay
//  File:       gifdisplayer.cpp
//  Project:    Qt-File-Explorer
//  Purpose:    Implementation of class GIFDisplayer.
//---------------------------------------------------------



#include "gifdisplayer.h"
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QStyle>
#include <QEvent>
#include <QElapsedTimer>


GIFDisplayer::GIFDisplayer(const QString& fileName, const QSize& size ,QWidget *parent) : QWidget(parent), fileName(fileName), size(size)
{
    decoder = new GIFDecoder(this, fileName, size);
    decoder->moveToThread(&decodeThread);
    decodeThread.start();

    firstFramePixmap = QPixmap(QPixmap::fromImage(decoder->firstFrameImage).scaled(size));
    imageLabel = new QLabel;
    imageLabel->setPixmap(firstFramePixmap);
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageLabel);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    timerDelay = new QTimer;
    QObject::connect(timerDelay, SIGNAL(timeout()), this, SLOT(replyTimeOut()));
    QObject::connect(timerDelay, SIGNAL(timeout()), decoder, SLOT(onTimerDelayTimeOut()));
    gifState = Start;

    timer = new QElapsedTimer;
}

GIFDisplayer::GIFDisplayer(QWidget *parent): QWidget(parent)
{
    decoder = new GIFDecoder();
    decoder->moveToThread(&decodeThread);
    decodeThread.start();

    imageLabel = new QLabel;
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(imageLabel);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    timerDelay = new QTimer;
    QObject::connect(timerDelay, SIGNAL(timeout()), this, SLOT(replyTimeOut()));
    QObject::connect(timerDelay, SIGNAL(timeout()), decoder, SLOT(onTimerDelayTimeOut()));
    gifState = Stop;

    timer = new QElapsedTimer;
}

void GIFDisplayer::setScaledSize(const QSize& size)
{
    this->size = size;
    decoder->setScaledSize(size);
    if (!this->fileName.isNull())
    {
        firstFramePixmap = QPixmap(QPixmap::fromImage(decoder->firstFrameImage).scaled(size));
        imageLabel->setPixmap(firstFramePixmap);
        gifState = Start;
    }
}

void GIFDisplayer::setFileName(const QString &fileName)
{
    this->fileName = fileName;
    decoder->setFileName(fileName);
    firstFramePixmap = QPixmap(QPixmap::fromImage(decoder->firstFrameImage).scaled(size));
    imageLabel->setPixmap(firstFramePixmap);
    gifState = Start;
}

void GIFDisplayer::play()
{
    //gifState = Start;
}

void GIFDisplayer::start()
{
    timer->start();
    decoder->clearBuffer();
    decoder->resetDecoder();
    currentFrame = 0;
    nextTimeDelay = decoder->firstFrameDelay;
    currentFrame = currentFrame + 1;
    timerDelay->start(nextTimeDelay);
}

void GIFDisplayer::stop()
{
    timerDelay->stop();
    decoder->clearBuffer();
    imageLabel->setPixmap(firstFramePixmap);
}

void GIFDisplayer::replyTimeOut()
{
    while(decoder->itemBuffer.count() <= MINIMUM_FRAME_BUFFER);
    item = decoder->itemBuffer.dequeue();
    timerDelay->start(item.timeDelay);
    currentPixmap = QPixmap(QPixmap::fromImage(item.imageItem).scaled(size));
    imageLabel->setPixmap(currentPixmap);
    nextTimeDelay = item.timeDelay;
    currentFrame = currentFrame + 1;
    if (currentFrame == decoder->imageCount())
    {
        qDebug() << timer->elapsed();
        currentFrame = 0;
    }

}

void GIFDisplayer::enterEvent(QEvent*)
{
    if (gifState == Start)
        this->start();
}

void GIFDisplayer::leaveEvent(QEvent*)
{
    if (gifState == Start)
        this->stop();
}

GIFDisplayer::~GIFDisplayer()
{
}
