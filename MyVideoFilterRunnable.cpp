#include "MyVideoFilterRunnable.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include "MyVideoFilter.h"

extern QImage qt_imageFromVideoFrame(const QVideoFrame& f);

MyVideoFilterRunnable::MyVideoFilterRunnable(MyVideoFilter* parent) :
    m_Filter(parent)
{
}

QVideoFrame MyVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    if (!input->isValid())
    {
        qWarning("Invalid input format");
        return *input;
    }

    if (input->handleType() == QAbstractVideoBuffer::NoHandle)
    {
        QImage img = qt_imageFromVideoFrame(*input);

        if (img.format() == QImage::Format_ARGB32)
        {
            return run(input, surfaceFormat, flags, img);
        }

        return run(input, surfaceFormat, flags, img.convertToFormat(QImage::Format_ARGB32));
    }

    if (input->handleType() == QAbstractVideoBuffer::GLTextureHandle)
    {
        QImage img(input->width(), input->height(), QImage::Format_RGBA8888);
        GLuint textureId = input->handle().toInt();
        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions* f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLuint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, input->width(), input->height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        return run(input, surfaceFormat, flags, img.convertToFormat(QImage::Format_ARGB32));
    }

    qDebug() << Q_FUNC_INFO << "Unsupported handle type " << input->handleType();
    return *input;
}

QVideoFrame MyVideoFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags, QImage& image)
{
    if (image.format() != QImage::Format_ARGB32)
    {
        return *input;
    }

    int orientation = m_Filter ? m_Filter->property("orientation").toInt() : 0;

#ifdef Q_OS_ANDROID
    bool flip = true;
#else
    bool flip = surfaceFormat.scanLineDirection() == QVideoSurfaceFormat::BottomToTop;
#endif

    // qDebug() << Q_FUNC_INFO << "Orientation: " << orientation << "Flip: " << flip;

    for (int y = 0; y < image.height() && y < 10; y++)
    {
        auto line = image.bits() + y * image.bytesPerLine();
        auto leftPixel = line;
        auto rightPixel = line + image.bytesPerLine() - 4;
        for (int x = 0; x < image.width() && x < 10; x++)
        {
            leftPixel[0] = 0;
            leftPixel[1] = 255;
            leftPixel[2] = 0;
            leftPixel[3] = 0;
            leftPixel += 4;
            rightPixel[0] = 0;
            rightPixel[1] = 0;
            rightPixel[2] = 255;
            rightPixel[3] = 0;
            rightPixel -= 4;
        }
    }

    return QVideoFrame(image);
}
