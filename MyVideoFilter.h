#ifndef MyVideoFilter_H
#define MyVideoFilter_H

#include <QAbstractVideoFilter>

class MyVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY(int orientation MEMBER m_Orientation)

public:
    MyVideoFilter(QObject* parent = nullptr);

    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

protected:
    int m_Orientation;

};

#endif
