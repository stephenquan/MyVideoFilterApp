#ifndef __ShowCornersVideoFilter__
#define __ShowCornersVideoFilter__

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>

class ShowCornersVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY( int videoOutputOrientation MEMBER m_VideoOutputOrientation )
public:
    ShowCornersVideoFilter( QObject* parent = nullptr );
    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

    int m_VideoOutputOrientation;

};

class ShowCornersVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    ShowCornersVideoFilterRunnable( ShowCornersVideoFilter* filter );
    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;

protected:
    ShowCornersVideoFilter* m_Filter;

};

#endif
