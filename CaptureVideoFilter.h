#ifndef __CaptureVideoFilter__
#define __CaptureVideoFilter__

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>
#include <QPainter>
#include <QUrl>

class CaptureVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY( int videoOutputOrientation MEMBER m_VideoOutputOrientation )

signals:
    void captured( const QUrl& imageUrl );

public:
    CaptureVideoFilter( QObject* parent = nullptr );
    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

    Q_INVOKABLE void capture();

public:
    bool capturing() const { return m_Capturing; }
    void setCapturing( bool capturing ) { m_Capturing = capturing; }
    int videoOutputOrientation() const { return m_VideoOutputOrientation; }
    void emitCaptured( const QUrl& imageUrl );

protected:
    int m_VideoOutputOrientation;
    bool m_Capturing;

};

class CaptureVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    CaptureVideoFilterRunnable( CaptureVideoFilter* filter );
    QVideoFrame run( QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags ) Q_DECL_OVERRIDE;

protected:
    CaptureVideoFilter* m_Filter;

};

#endif
