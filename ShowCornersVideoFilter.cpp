#include "ShowCornersVideoFilter.h"
#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include <QGuiApplication>
#include <QScreen>
#include "VideoFrame.h"
#include "QVideoFrameToQImage.h"
#include "QImagePainter.h"

ShowCornersVideoFilter::ShowCornersVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* ShowCornersVideoFilter::createFilterRunnable()
{
    return new ShowCornersVideoFilterRunnable( this );
}

ShowCornersVideoFilterRunnable::ShowCornersVideoFilterRunnable( ShowCornersVideoFilter* filter ) :
    m_Filter( filter )
{
}

QVideoFrame ShowCornersVideoFilterRunnable::run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags )
{
    Q_UNUSED( flags )

    if ( !input )
    {
        return QVideoFrame();
    }

    int videoOutputOrientation = m_Filter->m_VideoOutputOrientation;

    QImage image = QVideoFrameToQImage( *input );
    int w = ( videoOutputOrientation == 0 || videoOutputOrientation == 180 ) ? image.width() : image.height();
    int h = ( videoOutputOrientation == 0 || videoOutputOrientation == 180 ) ? image.height() : image.width();

    QImagePainter painter( &image, input, surfaceFormat, videoOutputOrientation );

    painter.setFont( QFont("Arial", 10 ) );
    painter.setPen( Qt::white );

    painter.fillRect( QRect( 0, 0, 24, 24 ), QColor( Qt::GlobalColor::red ) );
    painter.drawText( QRect( 0, 0, 24, 24 ), Qt::AlignCenter, QStringLiteral( "1" ) );

    painter.fillRect( QRect( w-1 - 24, 0, 24, 24 ), QColor( Qt::GlobalColor::darkGreen ) );
    painter.drawText( QRect( w-1 - 24, 0, 24, 24 ), Qt::AlignCenter, QStringLiteral( "2" ) );

    painter.fillRect( QRect( w-1 - 24, h-1 - 24, 24, 24 ), QColor( Qt::GlobalColor::darkGreen ) );
    painter.drawText( QRect( w-1 - 24, h-1 - 24, 24, 24 ), Qt::AlignCenter, QStringLiteral( "3" ) );

    painter.fillRect( QRect( 0, h-1 - 24, 24, 24 ), QColor( Qt::GlobalColor::blue ) );
    painter.drawText( QRect( 0, h-1 - 24, 24, 24 ), Qt::AlignCenter, QStringLiteral( "4" ) );

    return image;
}
