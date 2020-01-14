#include "ShowCornersVideoFilter.h"
#include <QPainter>
#include <QDebug>
#include "VideoFrame.h"
#include "QVideoFrameToQImage.h"
#include "QImagePainter.h"

ShowCornersVideoFilterRunnable::ShowCornersVideoFilterRunnable()
{
}

QVideoFrame ShowCornersVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( flags )

    if ( !input )
    {
        return QVideoFrame();
    }

    QImage image = QVideoFrameToQImage( *input );

    QImagePainter painter( &image, input, surfaceFormat );
    painter.fillRect( 0, 0, 16, 16, QColor( Qt::GlobalColor::red ) );
    painter.fillRect( image.width() - 16, 0, 16, 16, QColor( Qt::GlobalColor::green ) );
    painter.fillRect( 0, image.height() - 16, 16, 16, QColor( Qt::GlobalColor::green ) );
    painter.fillRect( image.width() - 16, image.height() - 16, 16, 16, QColor( Qt::GlobalColor::green ) );
    painter.drawText( 32, 32, QStringLiteral( "Origin" ) );

    return image;
}

ShowCornersVideoFilter::ShowCornersVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* ShowCornersVideoFilter::createFilterRunnable()
{
    return new ShowCornersVideoFilterRunnable();
}
