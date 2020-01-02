#ifndef __BlueToRedVideoFilter__
#define __BlueToRedVideoFilter__

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>

class BlueToRedVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    BlueToRedVideoFilterRunnable();
    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;
};

class BlueToRedVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    BlueToRedVideoFilter( QObject* parent = nullptr );
    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;
};

#endif
