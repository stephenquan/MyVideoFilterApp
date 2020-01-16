#include "QVideoFrameBits.h"
#include <QDebug>

QVideoFrameBits::QVideoFrameBits( QImage* image, QVideoFrame* videoFrame, const QVideoSurfaceFormat& surfaceFormat, int videoOutputOrientation  ) :
    m_SourceBits( nullptr ),
    m_SourceSize( 0, 0 ),
    m_SourceDelta( QPoint( 4, 0 ) ),
    m_SourceClipRect( 0, 0, 0, 0 ),
    m_VideoOutputOrientation( 0 ),
    m_Mirror( false ),
    m_TransformBits( nullptr ),
    m_TransformSize( 0, 0 ),
    m_TransformDelta( 4, 0 )
{
    Q_UNUSED( surfaceFormat )

    if ( image )
    {
        setSource( *image );
    }

    setVideoOutputOrientation( videoOutputOrientation );
}

void QVideoFrameBits::setSource( uchar* bits, const QSize& sourceSize, const QPoint& sourceDelta )
{
    m_SourceBits = bits;
    m_SourceSize = sourceSize;
    m_SourceDelta = sourceDelta;
    update();
}

void QVideoFrameBits::setSource( QImage& image )
{
    setSource(
                image.bits(),
                QSize( image.width(), image.height() ),
                QPoint( image.bytesPerLine() / image.width(), image.bytesPerLine() )
                );
}

void QVideoFrameBits::setSourceClipRect( const QRect& sourceClipRect )
{
    m_SourceClipRect = sourceClipRect;
    update();
}

void QVideoFrameBits::setVideoOutputOrientation( int videoOutputOrientation )
{
    m_VideoOutputOrientation = videoOutputOrientation;
    update();
}

void QVideoFrameBits::setMirror( bool mirror )
{
    m_Mirror = mirror;
    update();
}

void QVideoFrameBits::update()
{
    if ( !m_SourceBits )
    {
        m_TransformSize = QSize( 0, 0 );
        m_TransformBits = nullptr;
        m_TransformDelta = QPoint( 0, 0 );
        return;
    }

    const bool& mirror = m_Mirror;
    const int clipX = !m_SourceClipRect.isEmpty() ? m_SourceClipRect.x() : 0;
    const int clipY = !m_SourceClipRect.isEmpty() ? m_SourceClipRect.y() : 0;
    const int clipWidth = !m_SourceClipRect.isEmpty() ? m_SourceClipRect.width() : m_SourceSize.width();
    const int clipHeight = !m_SourceClipRect.isEmpty() ? m_SourceClipRect.height() : m_SourceSize.height();
    const int& sourceDeltaX = m_SourceDelta.x();
    const int& sourceDeltaY = m_SourceDelta.y();

    switch ( m_VideoOutputOrientation )
    {
    case 0:
    default:
        m_TransformSize = QSize( clipWidth, clipHeight );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX : clipX + clipWidth - 1 ) * sourceDeltaX + ( clipY ) * sourceDeltaY;
        m_TransformDelta = QPoint( !mirror ? sourceDeltaX : -sourceDeltaX, sourceDeltaY );
        break;
    case 90:
        m_TransformSize = QSize( clipHeight, clipWidth );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX + clipWidth - 1 : clipX ) * sourceDeltaX + ( clipY ) * sourceDeltaY;
        m_TransformDelta = QPoint( sourceDeltaY, !mirror ? -sourceDeltaX : sourceDeltaX );
        break;
    case 180:
        m_TransformSize = QSize( clipWidth, clipHeight );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX + clipWidth - 1 : clipX ) * sourceDeltaX + ( clipY + clipHeight - 1 ) * sourceDeltaY;
        m_TransformDelta = QPoint( !mirror ? -sourceDeltaX : sourceDeltaX, -sourceDeltaY );
        break;
    case 270:
        m_TransformSize = QSize( clipHeight, clipWidth );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX : clipX + clipWidth - 1 ) * sourceDeltaX + ( clipY + clipHeight - 1 ) * sourceDeltaY;
        m_TransformDelta = QPoint( -sourceDeltaY, !mirror ? sourceDeltaX : -sourceDeltaX );
        break;
    }
}

void QVideoFrameBits::getScanLine( int y, uchar* bits )
{
    if ( !m_TransformBits )
    {
        return;
    }

    uchar* src = m_TransformBits + y * m_TransformDelta.y();
    for ( int x = 0; x < m_TransformSize.width(); x++ )
    {
        memcpy( bits, src, 4 );
        bits += 4;
        src += m_TransformDelta.x();
    }
}

uchar* QVideoFrameBits::getPixelAddress( int x, int y )
{
    if ( !m_TransformBits )
    {
        return nullptr;
    }

    return m_TransformBits + y * m_TransformDelta.y() + x * m_TransformDelta.x();
}
