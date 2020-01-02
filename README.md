This is a sample video filter application.

This applications paints a red square into the top left at ( 0, 0 ) and a green square into the top right at ( Width, 0 ).

![Screenshot.png](Screenshot.png)

The QML code shows how to use custom video filters, for example:

```qml
    Camera {
        id: camera
    }
    
    VideoOutput {
        source: camera
        filters: [ showCornersVideoFilter ]
    }
    
    ShowCornersVideoFilter {
        id: showCornersVideoFilter
    }
```

The C++ code shows how to implement custom video filters by subclassing QAbstractVideoFilter, for example:

```c++
    class ShowCornersVideoFilter: public QAbstractVideoFilterRunnable
    {
        Q_OBJECT
    public:
        QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;
    };
    
    class ShowCornersVideoFilterRunnable : public QVideoFilterRunnable
    {
    public:
        QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;
    };
```

# QVideoFrameToQImage helper function

To make things easier, I included QVideoFrameToQImage helper function.

 - For non-Android, using Qt's qt_imageFromVideoFrame() to convert a `QVideoFrame` to a `QImage`.
 - For Android, using the GLTextureHandle to obtain a `QImage`.

# Sample VideoFilters

I've implemented a number of image filters for you to try.
The sample is hardcoded for ShowCornersVideoFilter.
Edit `main.qml` to try all the other video filters.

## ShowCornersVideoFilter

This is the default video filter for this application.
It shows a red square in the left corner at ( 0, 0 ).
It shows a green square in the right corner at ( Width, 0 ).

## BlueToRedVideoFilter

This is a fun 'joke' video filter which recolors only blue objects into red.

## GreyScaleVideoFilter

This is a video filter turns everything into grey.

## NullVideoFilter

This video filter acts as a 'passthru', i.e. it does nothing to the image.

## EditCenterVideoFilter

This video filter demonstrates how to pass properties from QML to the video filter.

 - angle can be 0, 90, 180, 270 causing the center of the image to rotate.
 - mirror will do a mirror flip of an image, useful for working with front facing cameras.
 - invert will do an x-ray like color inversion. This is useful for scanning white barcodes on a black background.

# References

 - http://doc.qt.io/qt-5/qabstractvideobuffer.html
