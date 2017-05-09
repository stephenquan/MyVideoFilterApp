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
    QVideoFrame run(const QImage& image, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

protected:
    MyVideoFilter* m_Filter;

};

#endif
