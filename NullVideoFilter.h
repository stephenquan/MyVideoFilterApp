#ifndef __NullVideoFilter__
#define __NullVideoFilter__

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>

class NullVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    NullVideoFilterRunnable();
    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;
};

class NullVideoFilterRunnableProxy : public QObject
{
    Q_OBJECT
public:
    NullVideoFilterRunnableProxy( QObject* parent = nullptr );
    NullVideoFilterRunnableProxy( NullVideoFilterRunnable* runnable, QObject* parent = nullptr );

protected:
    NullVideoFilterRunnable* m_Runnable;

};

class NullVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    NullVideoFilter( QObject* parent = nullptr );
    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;
};

#endif
