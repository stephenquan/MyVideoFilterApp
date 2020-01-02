#include "NullVideoFilter.h"

NullVideoFilterRunnable::NullVideoFilterRunnable()
{
}

QVideoFrame NullVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED( surfaceFormat )
    Q_UNUSED( flags )
    if ( !input ) return QVideoFrame();
    return *input;
};

NullVideoFilter::NullVideoFilter( QObject* parent )
    : QAbstractVideoFilter( parent )
{
}

QVideoFilterRunnable* NullVideoFilter::createFilterRunnable()
{
    return new NullVideoFilterRunnable();
};
