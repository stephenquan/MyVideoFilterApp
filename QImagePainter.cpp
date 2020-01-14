#include "QImagePainter.h"

QImagePainter::QImagePainter( QImage* image, QVideoFrame* videoFrame, const QVideoSurfaceFormat& surfaceFormat ) :
    QPainter( image )
{
    bool bottomToTop = false;

    if ( image )
    {
        if ( videoFrame && videoFrame->handleType() == QAbstractVideoBuffer::NoHandle )
        {
            bottomToTop = ( surfaceFormat.scanLineDirection() == QVideoSurfaceFormat::BottomToTop );
        }
    }

    translate( 0, ( !image || !bottomToTop ) ? 0 : image->height() );
    scale( 1, ( !image || !bottomToTop ) ? 1 : -1 );
}
