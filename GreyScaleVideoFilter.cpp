#include "GreyScaleVideoFilter.h"
#include "VideoFrame.h"
#include "QVideoFrameToQImage.h"
#include "QImageScanLines.h"

GreyScaleVideoFilterRunnable::GreyScaleVideoFilterRunnable()
{
}

QVideoFrame GreyScaleVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( flags )

    if ( !input )
    {
        return QVideoFrame();
    }

    QImage image = QVideoFrameToQImage( *input );

    QImageScanLines scanLines( &image, input, surfaceFormat );

    int width = scanLines.width();
    int height = scanLines.height();

    for ( int y = 0; y < height; y++ )
    {
        uchar* pixel = scanLines.scanLine( y );
        for ( int x = 0; x < width; x++ )
        {
            uchar& B = pixel[ 0 ];
            uchar& G = pixel[ 1 ];
            uchar& R = pixel[ 2 ];
            B = G = R = static_cast< uchar >( qGray( R, G, B ) );
            pixel += 4;
        }
    }

    return image;
}

GreyScaleVideoFilter::GreyScaleVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* GreyScaleVideoFilter::createFilterRunnable()
{
    return new GreyScaleVideoFilterRunnable();
}
