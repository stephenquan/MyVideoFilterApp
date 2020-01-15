#include "QImagePainter.h"
#include <QDateTime>
#include <QDebug>

QImagePainter::QImagePainter( QImage* image, QVideoFrame* videoFrame, const QVideoSurfaceFormat& surfaceFormat , int videoOutputOrientation) :
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

    switch ( videoOutputOrientation )
    {
    case 270:
        rotate( 270 );
        translate( -(image->height()-1), !bottomToTop ? 0 : (image->width()-1) );
        scale( 1, !bottomToTop ? 1 : -1 );
        break;

    case 180:
        rotate( 180 );
        translate( -(image->width()-1), !bottomToTop ? -(image->height()-1) : 0 );
        scale( 1, !bottomToTop ? 1 : -1 );
        break;

    case 90:
        rotate( 90 );
        translate( 0, !bottomToTop ? -(image->width()-1) : 0 );
        scale( 1, !bottomToTop ? 1 : -1 );
        break;

    case 0:
    default:
        translate( 0, !bottomToTop ? 0 : (image->height()-1) );
        scale( 1, !bottomToTop ? 1 : -1 );
        break;

    }
}
