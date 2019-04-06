#include "MyVideoFilterRunnable.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include <QDateTime>
#include "MyVideoFilter.h"

QImage qt_imageFromVideoFrame(const QVideoFrame& f);

MyVideoFilterRunnable::MyVideoFilterRunnable(MyVideoFilter* parent) :
    m_Filter(parent),
    m_Orientation(0),
    m_Flip(0)
{
}

QVideoFrame MyVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED(flags)

    if (!input->isValid())
    {
        qWarning("Invalid input format");
        return *input;
    }

    static qint64 lastDraw = 0;

    if (QDateTime::currentDateTime().currentMSecsSinceEpoch() < lastDraw + 3000)
    {
    }
    else if (QDateTime::currentDateTime().currentMSecsSinceEpoch() < lastDraw + 6000)
    {
        return *input;
    }
    else
    {
        lastDraw = QDateTime::currentDateTime().currentMSecsSinceEpoch();
    }

    m_Orientation = m_Filter ? m_Filter->property("orientation").toInt() : 0;

#ifdef Q_OS_ANDROID
    m_Flip = true;
#else
    m_Flip = surfaceFormat.scanLineDirection() == QVideoSurfaceFormat::BottomToTop;
#endif

    QImage image = QVideoFrameToQImage(input);
    if (image.isNull())
    {
        return *input;
    }

    if (image.format() != QImage::Format_ARGB32)
    {
        image = image.convertToFormat(QImage::Format_ARGB32);
    }

    drawRedGreenPixels(image);

    return QVideoFrame(image);
}

QImage MyVideoFilterRunnable::QVideoFrameToQImage(QVideoFrame* input)
{
    switch (input->handleType())
    {
    case QAbstractVideoBuffer::NoHandle:
        return QVideoFrameToQImage_using_Qt_internals(input);

    case QAbstractVideoBuffer::GLTextureHandle:
        return QVideoFrameToQImage_using_GLTextureHandle(input);

    case QAbstractVideoBuffer::XvShmImageHandle:
    case QAbstractVideoBuffer::CoreImageHandle:
    case QAbstractVideoBuffer::QPixmapHandle:
    case QAbstractVideoBuffer::EGLImageHandle:
    case QAbstractVideoBuffer::UserHandle:
        break;
    }

    return QImage();
}

QImage MyVideoFilterRunnable::QVideoFrameToQImage_using_Qt_internals(QVideoFrame* input)
{
    return qt_imageFromVideoFrame(*input);
}

QImage MyVideoFilterRunnable::QVideoFrameToQImage_using_GLTextureHandle(QVideoFrame* input)
{
    QImage image(input->width(), input->height(), QImage::Format_ARGB32);
    GLuint textureId = static_cast<GLuint>(input->handle().toInt());
    QOpenGLContext* ctx = QOpenGLContext::currentContext();
    QOpenGLFunctions* f = ctx->functions();
    GLuint fbo;
    f->glGenFramebuffers(1, &fbo);
    GLint prevFbo;
    f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
    f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    f->glReadPixels(0, 0, input->width(), input->height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    f->glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(prevFbo));
    return image;
}

void MyVideoFilterRunnable::drawRedGreenPixels(QImage& image)
{
    auto bits = image.bits();
    int bytesPerLine = image.bytesPerLine();
    auto bytesPerPixel = bytesPerLine / image.width();
    for (int y = 0; y < image.height() && y < 32; y++)
    {
        unsigned char* line = bits + y * bytesPerLine;
        auto leftPixel = line;
        auto rightPixel = line + bytesPerLine - bytesPerPixel;
        for (int x = 0; x < image.width() && x < 32; x++)
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
}
