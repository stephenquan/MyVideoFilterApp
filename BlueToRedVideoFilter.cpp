#include "BlueToRedVideoFilter.h"
#include "VideoFrame.h"
#include "QVideoFrameToQImage.h"
#include "QImageScanLines.h"

BlueToRedVideoFilterRunnable::BlueToRedVideoFilterRunnable()
{
}

QVideoFrame BlueToRedVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
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
            if ( B >= R && B >= G )
            {
                uchar t = R;
                R = B;
                B = t;
            }
            pixel += 4;
        }
    }

    return image;
}

BlueToRedVideoFilter::BlueToRedVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* BlueToRedVideoFilter::createFilterRunnable()
{
    return new BlueToRedVideoFilterRunnable();
}
