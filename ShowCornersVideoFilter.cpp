#include "ShowCornersVideoFilter.h"
#include <QPainter>
#include "VideoFrame.h"

ShowCornersVideoFilterRunnable::ShowCornersVideoFilterRunnable()
{
}

QVideoFrame ShowCornersVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( surfaceFormat )
    Q_UNUSED( flags )
    if ( !input ) return QVideoFrame();
    VideoFrame videoFrame( input );
    QImage* image = videoFrame.startEditing();
    if ( !image ) return *input;
    paintCorners( image );
    return videoFrame.finish();
}

void ShowCornersVideoFilterRunnable::paintCorners( QImage* image )
{
    QPainter painter( image );
    painter.fillRect( 0, 0, 32, 32, QColor( Qt::GlobalColor::red ) );
    painter.fillRect( image->width() - 32, 0, 32, 32, QColor( Qt::GlobalColor::green ) );
}

ShowCornersVideoFilter::ShowCornersVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* ShowCornersVideoFilter::createFilterRunnable()
{
    return new ShowCornersVideoFilterRunnable();
}
