#ifndef __ShowCornersVideoFilter__
#define __ShowCornersVideoFilter__

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>

class ShowCornersVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    ShowCornersVideoFilterRunnable();
    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;
};

class ShowCornersVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    ShowCornersVideoFilter( QObject* parent = nullptr );
    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;
};

#endif
