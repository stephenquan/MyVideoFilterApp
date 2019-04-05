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

    static QImage QVideoFrameToQImage(QVideoFrame* input);
    static QImage QVideoFrameToQImage_using_Qt_internals(QVideoFrame* input);
    static QImage QVideoFrameToQImage_using_GLTextureHandle(QVideoFrame* input);
    static void drawRedGreenPixels(QImage& image);

protected:
    MyVideoFilter* m_Filter;
    int m_Orientation;
    int m_Flip;

};

#endif
