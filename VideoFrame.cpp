#include "VideoFrame.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include "QVideoFrameToQImage.h"

//----------------------------------------------------------------------

VideoFrame::VideoFrame( QObject* parent ) :
    QObject( parent ),
    m_Input( nullptr ),
    m_MappedImage( nullptr ),
    m_MapMode( QAbstractVideoBuffer::MapMode::NotMapped )
{
}

//----------------------------------------------------------------------

VideoFrame::VideoFrame( QVideoFrame* input, QObject* parent ) :
    QObject( parent ),
    m_Input( input ),
    m_MappedImage( nullptr ),
    m_MapMode( QAbstractVideoBuffer::NotMapped )
{
}

//----------------------------------------------------------------------

VideoFrame::~VideoFrame()
{
    if ( m_MapMode != QAbstractVideoBuffer::NotMapped )
    {
        finish();
    }
}

//----------------------------------------------------------------------

QImage* VideoFrame::startReading()
{
    return start( QAbstractVideoBuffer::ReadOnly );
}

//----------------------------------------------------------------------

QImage* VideoFrame::startEditing()
{
    return start( QAbstractVideoBuffer::ReadWrite );
}

//----------------------------------------------------------------------

QVideoFrame VideoFrame::finish()
{
    if ( !m_Input )
    {
        return QVideoFrame();
    }

    if ( m_MappedImage )
    {
        delete m_MappedImage;
        m_MappedImage = nullptr;
        m_Input->unmap();
        m_MapMode = QAbstractVideoBuffer::NotMapped;
        return *m_Input;
    }

    if ( ( m_MapMode & QAbstractVideoBuffer::WriteOnly) == 0 )
    {
        m_MapMode = QAbstractVideoBuffer::NotMapped;
        return *m_Input;
    }

    m_MapMode = QAbstractVideoBuffer::NotMapped;
    return QVideoFrame( m_Image );
}

//----------------------------------------------------------------------

QImage* VideoFrame::start( QAbstractVideoBuffer::MapMode mode )
{
    if ( !m_Input )
    {
        return nullptr;
    }

    m_MapMode = mode;

    if ( m_Input->handleType() == QAbstractVideoBuffer::NoHandle
         && m_Input->pixelFormat() == QVideoFrame::PixelFormat::Format_ARGB32 )
    {
        if ( m_Input->map( mode ) )
        {
            m_MappedImage = new QImage(
                        m_Input->bits(),
                        m_Input->width(),
                        m_Input->height(),
                        m_Input->bytesPerLine(),
                        QVideoFrame::imageFormatFromPixelFormat( m_Input->pixelFormat() )
                        );
            return m_MappedImage;
        }
    }

    m_Image = QVideoFrameToQImage( *m_Input );
    return &m_Image;
}

//----------------------------------------------------------------------
