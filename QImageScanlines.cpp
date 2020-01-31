#include "QImageScanLines.h"

//----------------------------------------------------------------------

QImageScanLines::QImageScanLines( QImage* image, QVideoFrame* videoFrame, const QVideoSurfaceFormat& surfaceFormat ) :
    m_Image( nullptr ),
    m_BottomToTop( false )
{
    setSource( image, videoFrame, surfaceFormat );
}

//----------------------------------------------------------------------

uchar* QImageScanLines::scanLine( int y )
{
    if ( !m_Image )
    {
        return nullptr;
    }

    if ( m_BottomToTop )
    {
        return m_Image->scanLine( m_Image->height() - 1 - y );
    }

    return m_Image->scanLine( y );
}

//----------------------------------------------------------------------

void QImageScanLines::setSource( QImage* image, QVideoFrame* videoFrame, const QVideoSurfaceFormat& surfaceFormat )
{
    m_Image = image;
    m_BottomToTop = false;

    if ( videoFrame && videoFrame->handleType() == QAbstractVideoBuffer::NoHandle )
    {
        m_BottomToTop = ( surfaceFormat.scanLineDirection() == QVideoSurfaceFormat::Direction::BottomToTop );
    }
}

//----------------------------------------------------------------------
