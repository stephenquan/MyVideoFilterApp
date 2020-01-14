#include "EditCenterVideoFilter.h"
#include "VideoFrame.h"
#include "ImageTransformer.h"

EditCenterVideoFilterRunnable::EditCenterVideoFilterRunnable( EditCenterVideoFilter* filter ) :
    m_Filter( filter )
{
}

QVideoFrame EditCenterVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( surfaceFormat )
    Q_UNUSED( flags )

    if ( !input )
    {
        return QVideoFrame();
    }

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
}

EditCenterVideoFilter::EditCenterVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent ),
      m_angle( 0 ),
      m_invert( false ),
      m_mirror( false )
{
}

QVideoFilterRunnable* EditCenterVideoFilter::createFilterRunnable()
{
    return new EditCenterVideoFilterRunnable( this );
}
