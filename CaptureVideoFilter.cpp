#include "CaptureVideoFilter.h"
#include "QVideoFrameToQImage.h"
#include "QVideoFrameBits.h"
#include <QBuffer>
#include <QByteArray>
#include <QDebug>

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
    qDebug() << Q_FUNC_INFO << imageUrl;
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

    //qDebug() << Q_FUNC_INFO << __LINE__;

    if ( !input )
    {
        //qDebug() << Q_FUNC_INFO << __LINE__;
        return QVideoFrame();
    }

    //qDebug() << Q_FUNC_INFO << __LINE__;
    if ( !m_Filter || !m_Filter->capturing() )
    {
        //qDebug() << Q_FUNC_INFO << __LINE__;
        return *input;
    }

    qDebug() << Q_FUNC_INFO << __LINE__;
    QImage image = QVideoFrameToQImage( *input );
    QVideoFrameBits frameBits( &image, input, surfaceFormat, m_Filter->videoOutputOrientation() );

    QImage newImage( frameBits.width(), frameBits.height(), QImage::Format_ARGB32 );
    for ( int y = 0; y < frameBits.height(); y++ )
    {
        frameBits.getScanLine( y, newImage.scanLine( y ) );
    }

    qDebug() << Q_FUNC_INFO << __LINE__;
    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    newImage.save( &buffer, "PNG" );
    QString dataURI = QString( "data:image/png;base64," ) + QString::fromUtf8( byteArray.toBase64() );

    m_Filter->emitCaptured( QUrl( dataURI ) );
    m_Filter->setCapturing( false );

    return *input;

    /*

    VideoFrame videoFrame( input );
    QImage* image = videoFrame.startEditing();
    ImageTransformer imageTransformer( *image);
    int d = ( image->width() < image->height() ? image->width() : image->height() ) * 9 / 10;
    int x0 = image->width() / 2 - d / 2;
    int y0 = image->height() / 2 - d / 2;
    imageTransformer.setClipRect( QRect( x0, y0, d, d ) );
    imageTransformer.setRotate( m_Filter ? m_Filter->m_angle : 0 );
    imageTransformer.setInvert( m_Filter ? m_Filter->m_invert : false );
    imageTransformer.setMirror( m_Filter ? m_Filter->m_mirror : false );
    QSize size = imageTransformer.transformSize();
    QImage newImage( size, QImage::Format_ARGB32 );
    for ( int y = 0; y < size.height(); y++ )
    {
        imageTransformer.getBits( y, newImage.scanLine( y ) );
    }
    {
        QPainter painter( image );
        painter.drawImage( x0, y0, newImage );
    }
    return videoFrame.finish();
    */
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
