#ifndef __QImagePainter__
#define __QImagePainter__

#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QPainter>

class QImagePainter : public QPainter
{
public:
    QImagePainter( QImage* image = nullptr, QVideoFrame* videoFrame = nullptr, const QVideoSurfaceFormat& surfaceFormat = QVideoSurfaceFormat() );

};

#endif
