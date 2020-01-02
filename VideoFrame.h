#ifndef __VideoFrame__
#define __VideoFrame__

#include <QVideoFrame>
#include <QImage>

class VideoFrame : public QObject
{
    Q_OBJECT

public:
    VideoFrame( QObject* parent = nullptr );
    VideoFrame( QVideoFrame* input, QObject* parent = nullptr );
    ~VideoFrame();

    QImage* startReading();
    QImage* startEditing();
    QVideoFrame finish();

protected:
    QVideoFrame* m_Input;
    QImage* m_MappedImage;
    QImage m_Image;
    QAbstractVideoBuffer::MapMode m_MapMode;

    QImage* start( QAbstractVideoBuffer::MapMode mode );

};

#endif
