#include "MyVideoFilterRunnable.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include "MyVideoFilter.h"

extern QImage qt_imageFromVideoFrame(const QVideoFrame& f);

MyVideoFilterRunnable::MyVideoFilterRunnable(MyVideoFilter* parent) :
    m_Filter(parent),
    m_Orientation(0),
    m_Flip(0)
{
}

QVideoFrame MyVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    if (!input->isValid())
    {
        qWarning("Invalid input format");
        return *input;
    }

    m_Orientation = m_Filter ? m_Filter->property("orientation").toInt() : 0;

#ifdef Q_OS_ANDROID
    m_Flip = true;
#else
    m_Flip = surfaceFormat.scanLineDirection() == QVideoSurfaceFormat::BottomToTop;
#endif

    switch (input->pixelFormat())
    {
    case QVideoFrame::Format_BGR24:
    case QVideoFrame::Format_BGR32:
        if (input->map(QAbstractVideoBuffer::ReadWrite))
        {
            return run(input, surfaceFormat, flags, QImage());
        }
        break;
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
        m_Flip = false;
        return run(input, surfaceFormat, flags, img.convertToFormat(QImage::Format_ARGB32));
    }

    qDebug() << Q_FUNC_INFO << "Unsupported handle type " << input->handleType();
    return *input;
}

QVideoFrame MyVideoFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags, QImage image)
{
    Q_UNUSED(surfaceFormat)
    Q_UNUSED(flags)

    auto bits = !image.isNull() ? image.bits() : input->bits();
    int bytesPerLine = !image.isNull() ? image.bytesPerLine() : input->bytesPerLine();
    auto bytesPerPixel = bytesPerLine / input->width();
    QVideoFrame::PixelFormat pixelFormat = !image.isNull() ? QVideoFrame::pixelFormatFromImageFormat(image.format()) : input->pixelFormat();

    // qDebug() << Q_FUNC_INFO << "Orientation: " << m_Orientation << "Flip: " << m_Flip;

    for (int y = 0; y < input->height() && y < 32; y++)
    {
        unsigned char* line = bits + y * bytesPerLine;
        auto leftPixel = line;
        auto rightPixel = line + bytesPerLine - bytesPerPixel;
        for (int x = 0; x < input->width() && x < 32; x++)
        {
            leftPixel[0] = 0;
            leftPixel[1] = 255;
            leftPixel[2] = 0;
            leftPixel += bytesPerPixel;
            rightPixel[0] = 0;
            rightPixel[1] = 0;
            rightPixel[2] = 255;
            rightPixel -= bytesPerPixel;
        }
    }

    if (input->isMapped())
    {
        input->unmap();
    }

    return !image.isNull() ? QVideoFrame(image) : *input;
}
