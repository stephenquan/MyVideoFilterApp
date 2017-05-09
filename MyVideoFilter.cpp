#include "MyVideoFilter.h"
#include "MyVideoFilterRunnable.h"

MyVideoFilter::MyVideoFilter(QObject* parent) :
    QAbstractVideoFilter(parent),
    m_Orientation(0)
{
}

QVideoFilterRunnable* MyVideoFilter::createFilterRunnable()
{
    return new MyVideoFilterRunnable();
}
