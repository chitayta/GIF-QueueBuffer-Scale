//---------------------------------------------------------
//  Name:       Chi Tay Ta
//  File:       gifdecoder.h
//  Project:    Qt-File-Explorer
//  Purpose:    Header file of class GIFDecoder.
//              Read frame image from gif file.
//---------------------------------------------------------

#ifndef GIFDECODER_H
#define GIFDECODER_H

#include <QObject>
#include <QThread>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QMap>
#include <QTimer>
#include <QQueue>

#define MINIMUM_FRAME_BUFFER        1
#define NUMBER_FRAME_BUFFER         2
#define DEFAULT_SCALED_WIDTH        300
#define DEFAULT_SCALED_HEIGHT       200

class GIFDisplayer;
class QElapsedTimer;
class FrameInfo
{
public:
    QImage         imageItem;
    quint64        timeDelay;
    FrameInfo(QImage image, uint time): imageItem(image), timeDelay(time){}
    FrameInfo(){}
};

class GIFDecoder : public QObject
{
    Q_OBJECT

    QString             fileName;
    GIFDisplayer*       displayer;
    QImageReader        reader;
    QSize               frameSize;
    uint                imageCountValue;
    uint                firstFrameReadTime;
    QElapsedTimer*      timer;
    FrameInfo           frameInfo;
public:
    QImage              firstFrameImage;
    uint                firstFrameDelay;
    uint                frameNumber;
    QQueue<FrameInfo>   itemBuffer;

    GIFDecoder(GIFDisplayer* displayer, const QString& fileName, const QSize& size);
    GIFDecoder();
    uint imageCount();
    void resetDecoder();
    void setFileName(const QString& fileName);
    void setScaledSize(const QSize& size);
    void clearBuffer();

public slots:
    void onTimerDelayTimeOut();
};

#endif // GIFDECODER_H
