#include "NullVideoFilter.h"
#include <QJSEngine>
#include "QVideoFrameToQImage.h"
#include "VideoFrame.h"

NullVideoFilterRunnable::NullVideoFilterRunnable()
{
}

QVideoFrame NullVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED( surfaceFormat )
    Q_UNUSED( flags )

    if ( !input )
    {
        return QVideoFrame();
    }

    QImage image = QVideoFrameToQImage( *input );

    return image;
};

NullVideoFilter::NullVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* NullVideoFilter::createFilterRunnable()
{
    return new NullVideoFilterRunnable();
};
