#ifndef __QImageScanLines__
#define __QImageScanLines__

#include <QVideoFrame>
#include <QVideoSurfaceFormat>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class QImageScanLines
{
public:
    QImageScanLines( QImage* image, QVideoFrame* videoFrame, const QVideoSurfaceFormat& surfaceFormat );

    inline int width() const { return m_Image ? m_Image->width() : 0; }
    inline int height() const { return m_Image ? m_Image->height() : 0; }
    uchar* scanLine( int y );
    void setSource( QImage* image, QVideoFrame* videoFrame, const QVideoSurfaceFormat& surfaceFormat );

protected:
    QImage* m_Image;
    bool m_BottomToTop;

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#endif
