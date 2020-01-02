#ifndef __ImageTransformer__
#define __ImageTransformer__

#include <QImage>
#include <QRect>
#include <QSize>
#include <QPoint>

class ImageTransformer
{
public:
    ImageTransformer( QImage& image );
    ImageTransformer( uchar *bits = nullptr, int width = 0, int height = 0 );

    void setSource( uchar* bits, const QSize& sourceSize, int bytesPerLine, int bytesPerPixel );
    void setSource( QImage& image );

    const QRect& clipRect() const { return m_ClipRect; }
    void setClipRect( const QRect& clipRect );

    int rotate() { return m_Rotate; }
    void setRotate( int rotate );

    bool mirror() const { return m_Mirror; }
    void setMirror( bool mirror );

    bool invert() const { return m_Invert; }
    void setInvert( bool invert );

    void getBits( int y, uchar* bits );

    inline QSize transformSize() const { return m_TransformSize; }

protected:
    uchar* m_SourceBits;
    QSize m_SourceSize;
    int m_BytesPerPixel;
    int m_BytesPerLine;
    QRect m_ClipRect;
    int m_Rotate;
    bool m_Mirror;
    bool m_Invert;
    uchar* m_TransformBits;
    QSize m_TransformSize;
    QPoint m_TransformDelta;

    void update();
};

#endif
