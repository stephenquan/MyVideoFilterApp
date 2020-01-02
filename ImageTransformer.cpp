#include "ImageTransformer.h"
#include <QDebug>

ImageTransformer::ImageTransformer( QImage& image ) :
    m_SourceBits( nullptr ),
    m_SourceSize( 0, 0 ),
    m_BytesPerPixel( 4 ),
    m_BytesPerLine( 0 ),
    m_ClipRect( 0, 0, 0, 0 ),
    m_Rotate( 0 ),
    m_Mirror( false ),
    m_Invert( false ),
    m_TransformBits( nullptr ),
    m_TransformSize( 0, 0 ),
    m_TransformDelta( 4, 0 )
{
    setSource( image );
}

ImageTransformer::ImageTransformer( uchar *bits, int width, int height ) :
    m_SourceBits( nullptr ),
    m_SourceSize( width, height ),
    m_BytesPerPixel( 4 ),
    m_BytesPerLine( width * 4 ),
    m_ClipRect( 0, 0, width, height ),
    m_Rotate( 0 ),
    m_Mirror( false ),
    m_Invert( false ),
    m_TransformBits( nullptr ),
    m_TransformSize( width, height ),
    m_TransformDelta( 4, width * 4 )
{
    setSource( bits, QSize( width, height ), width * 4, 4 );
}

void ImageTransformer::setSource( uchar* bits, const QSize& sourceSize, int bytesPerLine, int bytesPerPixel )
{
    m_SourceBits = bits;
    m_SourceSize = sourceSize;
    m_BytesPerLine = bytesPerLine;
    m_BytesPerPixel = bytesPerPixel;
    update();
}

void ImageTransformer::setSource( QImage& image )
{
    setSource(
                image.bits(),
                QSize( image.width(), image.height() ),
                image.bytesPerLine(),
                image.bytesPerLine() / image.width() );
}

void ImageTransformer::setClipRect( const QRect& clipRect )
{
    m_ClipRect = clipRect;
    update();
}

void ImageTransformer::setRotate( int rotate )
{
    m_Rotate = rotate;
    update();
}

void ImageTransformer::setMirror( bool mirror )
{
    m_Mirror = mirror;
    update();
}

void ImageTransformer::setInvert( bool invert )
{
    m_Invert = invert;
}

void ImageTransformer::update()
{
    if ( !m_SourceBits )
    {
        m_TransformSize = QSize( 0, 0 );
        m_TransformBits = nullptr;
        m_TransformDelta = QPoint( 0, 0 );
        return;
    }

    const bool& mirror = m_Mirror;
    const int clipX = !m_ClipRect.isEmpty() ? m_ClipRect.x() : 0;
    const int clipY = !m_ClipRect.isEmpty() ? m_ClipRect.y() : 0;
    const int clipWidth = !m_ClipRect.isEmpty() ? m_ClipRect.width() : m_SourceSize.width();
    const int clipHeight = !m_ClipRect.isEmpty() ? m_ClipRect.height() : m_SourceSize.height();
    const int& bytesPerPixel = m_BytesPerPixel;
    const int& bytesPerLine = m_BytesPerLine;

    switch ( m_Rotate )
    {
    case 0:
    default:
        m_TransformSize = QSize( clipWidth, clipHeight );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX : clipX + clipWidth - 1 ) * m_BytesPerPixel + ( clipY ) * m_BytesPerLine;
        m_TransformDelta = QPoint( !mirror ? bytesPerPixel : -bytesPerPixel, bytesPerLine );
        break;
    case 90:
        m_TransformSize = QSize( clipHeight, clipWidth );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX + clipWidth - 1 : clipX ) * m_BytesPerPixel + ( clipY ) * m_BytesPerLine;
        m_TransformDelta = QPoint( bytesPerLine, !mirror ? -bytesPerPixel : bytesPerPixel );
        break;
    case 180:
        m_TransformSize = QSize( clipWidth, clipHeight );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX + clipWidth - 1 : clipX ) * m_BytesPerPixel + ( clipY + clipHeight - 1 ) * m_BytesPerLine;
        m_TransformDelta = QPoint( !mirror ? -bytesPerPixel : bytesPerPixel, -bytesPerLine );
        break;
    case 270:
        m_TransformSize = QSize( clipHeight, clipWidth );
        m_TransformBits = m_SourceBits + ( !mirror ? clipX : clipX + clipWidth - 1 ) * m_BytesPerPixel + ( clipY + clipHeight - 1 ) * m_BytesPerLine;
        m_TransformDelta = QPoint( -bytesPerLine, !mirror ? bytesPerPixel : -bytesPerPixel );
        break;
    }
}

void ImageTransformer::getBits( int y, uchar* bits )
{
    if ( !m_TransformBits )
    {
        return;
    }

    uchar* src = m_TransformBits + y * m_TransformDelta.y();
    if ( !m_Invert )
    {
        for ( int x = 0; x < m_TransformSize.width(); x++ )
        {
            memcpy( bits, src, 4 );
            bits += 4;
            src += m_TransformDelta.x();
        }
    }
    else
    {
        for ( int x = 0; x < m_TransformSize.width(); x++ )
        {
            const uchar& b = src[ 0 ];
            const uchar& g = src[ 1 ];
            const uchar& r = src[ 2 ];
            const uchar& a = src[ 3 ];
            //uchar _y = ( ( 66 * r + 129 * g + 25 * b ) >> 8 ) + 16;
            //_y = 255 - _y;
            *bits++ = 255 - b;
            *bits++ = 255 - g;
            *bits++ = 255 - r;
            *bits++ = a;
            src += m_TransformDelta.x();
        }
    }
}
