#ifndef __EditCenterVideoFilter__
#define __EditCenterVideoFilter__

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>
#include <QPainter>

class EditCenterVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY( int angle MEMBER m_angle )
    Q_PROPERTY( bool invert MEMBER m_invert )
    Q_PROPERTY( bool mirror MEMBER m_mirror )

public:
    EditCenterVideoFilter( QObject* parent = nullptr );
    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

    int m_angle;
    bool m_invert;
    bool m_mirror;

};

class EditCenterVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    EditCenterVideoFilterRunnable( EditCenterVideoFilter* filter );
    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;

protected:
    EditCenterVideoFilter* m_Filter;

};

#endif
