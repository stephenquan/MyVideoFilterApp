#include "GreyScaleVideoFilter.h"
#include "VideoFrame.h"

GreyScaleVideoFilterRunnable::GreyScaleVideoFilterRunnable()
{
}

QVideoFrame GreyScaleVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( surfaceFormat )
    Q_UNUSED( flags )
    if ( !input ) return QVideoFrame();
    VideoFrame videoFrame( input );
    QImage* image = videoFrame.startEditing();
    if ( !image ) return *input;
    int width = image->width();
    int height = image->height();
    for ( int y = 0; y < height; y++ )
    {
        uchar* pixel = image->scanLine( y );
        for ( int x = 0; x < width; x++ )
        {
            uchar& B = pixel[ 0 ];
            uchar& G = pixel[ 1 ];
            uchar& R = pixel[ 2 ];
            B = G = R = static_cast< uchar >( qGray( R, G, B ) );
            pixel += 4;
        }
    }
    return videoFrame.finish();
}

GreyScaleVideoFilter::GreyScaleVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* GreyScaleVideoFilter::createFilterRunnable()
{
    return new GreyScaleVideoFilterRunnable();
}
