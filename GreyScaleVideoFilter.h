#ifndef __GreyScaleVideoFilter__
#define __GreyScaleVideoFilter__

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>

class GreyScaleVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    GreyScaleVideoFilter( QObject* parent = nullptr );
    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;
};

class GreyScaleVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    GreyScaleVideoFilterRunnable();
    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;
};

#endif
