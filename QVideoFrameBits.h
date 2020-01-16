#ifndef __QVideoFrameBits__
#define __QVideoFrameBits__

#include <QImage>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>

class QVideoFrameBits
{
public:
    QVideoFrameBits( QImage* image = nullptr, QVideoFrame* videoFrame = nullptr, const QVideoSurfaceFormat& surfaceFormat = QVideoSurfaceFormat(), int videoOutputOrientation = 0 );

    void setSource( uchar* bits, const QSize& sourceSize, const QPoint& sourceDelta );
    void setSource( QImage& image );

    const QRect& sourceClipRect() const { return m_SourceClipRect; }
    void setSourceClipRect( const QRect& sourceClipRect );

    int videoOutputOrientation() { return m_VideoOutputOrientation; }
    void setVideoOutputOrientation( int videoOutputOrientation );

    bool mirror() const { return m_Mirror; }
    void setMirror( bool mirror );

    void getScanLine( int y, uchar* bits );
    uchar* getPixelAddress( int x, int y );

    inline int width() const { return m_TransformSize.width(); }
    inline int height() const { return m_TransformSize.height(); }
    inline QPoint transformDelta() const { return m_TransformDelta; }

protected:
    uchar* m_SourceBits;
    QSize m_SourceSize;
    QPoint m_SourceDelta;
    QRect m_SourceClipRect;
    int m_VideoOutputOrientation;
    bool m_Mirror;
    uchar* m_TransformBits;
    QSize m_TransformSize;
    QPoint m_TransformDelta;

    void update();
};

#endif
