#ifndef MyVideoFilterRunnable_H
#define MyVideoFilterRunnable_H

#include <QVideoFilterRunnable>
#include <QVideoFrame>

class MyVideoFilter;

class MyVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    MyVideoFilterRunnable(MyVideoFilter* parent = nullptr);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags, QImage& image);

protected:
    MyVideoFilter* m_Filter;
    int m_Orientation;
    int m_Flip;

};

#endif
