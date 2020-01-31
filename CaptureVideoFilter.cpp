#include "CaptureVideoFilter.h"
#include "QVideoFrameToQImage.h"
#include "QVideoFrameBits.h"
#include <QBuffer>
#include <QByteArray>

CaptureVideoFilter::CaptureVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent ),
      m_VideoOutputOrientation( 0 ),
      m_Capturing( false )
{
}

QVideoFilterRunnable* CaptureVideoFilter::createFilterRunnable()
{
    return new CaptureVideoFilterRunnable( this );
}

void CaptureVideoFilter::capture()
{
    setCapturing( true );
}

void CaptureVideoFilter::emitCaptured( const QUrl &imageUrl )
{
    emit captured( imageUrl );
}

CaptureVideoFilterRunnable::CaptureVideoFilterRunnable( CaptureVideoFilter* filter ) :
    QVideoFilterRunnable(),
    m_Filter( filter )
{
}

QVideoFrame CaptureVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( surfaceFormat )
    Q_UNUSED( flags )

    if ( !input )
    {
        return QVideoFrame();
    }

    if ( !m_Filter || !m_Filter->capturing() )
    {
        return *input;
    }

    QImage image = QVideoFrameToQImage( *input );
    QVideoFrameBits frameBits( &image, input, surfaceFormat, m_Filter->videoOutputOrientation() );

    QImage newImage( frameBits.width(), frameBits.height(), QImage::Format_ARGB32 );
    for ( int y = 0; y < frameBits.height(); y++ )
    {
        frameBits.getScanLine( y, newImage.scanLine( y ) );
    }

    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    newImage.save( &buffer, "PNG" );
    QString dataURI = QString( "data:image/png;base64," ) + QString::fromUtf8( byteArray.toBase64() );

    m_Filter->emitCaptured( QUrl( dataURI ) );
    m_Filter->setCapturing( false );

    return *input;
}

void CaptureVideoFilter::setCapturing( bool capturing )
{
    if ( m_Capturing == capturing )
    {
        return;
    }

    m_Capturing = capturing;

    emit capturingChanged();
}
