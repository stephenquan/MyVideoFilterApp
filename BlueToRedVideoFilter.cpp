#include "BlueToRedVideoFilter.h"
#include "VideoFrame.h"

BlueToRedVideoFilterRunnable::BlueToRedVideoFilterRunnable()
{
}

QVideoFrame BlueToRedVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
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
            if ( B >= R && B >= G )
            {
                uchar t = R;
                R = B;
                B = t;
            }
            pixel += 4;
        }
    }
    return videoFrame.finish();
}

BlueToRedVideoFilter::BlueToRedVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* BlueToRedVideoFilter::createFilterRunnable()
{
    return new BlueToRedVideoFilterRunnable();
}
