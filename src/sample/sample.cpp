#include "sample.h"

#include <QRandomGenerator>

Sample::Sample(QObject *parent) :
    QObject(parent)
{
    const int SampleCount = 2;
    const int MaxDepth = 5;
    setObjectName("Hidden Root Object");
    populate(this, SampleCount, 0, MaxDepth);
}


void Sample::populate(QObject *parent, int childCount, int depth, int maxDepth)
{
    for(int i=0;i<childCount;++i)
    {
        auto *obj = createRandom(parent);
        obj->setObjectName(QString("%1_%2").arg(depth).arg(i));
        if(depth < maxDepth)
            populate(obj, childCount, depth+1, maxDepth);
    }
}


Sample::~Sample()
{

}

QObject *Sample::createRandom(QObject *parent)
{
    double random = QRandomGenerator::global()->bounded(1.0);
    if(random < 0.25)
    {
        return new Cat(parent);
    }
    if(random < 0.5)
    {
        return new Dog(parent);
    }
    if(random < 0.75)
    {
        return new Animal(parent);
    }
    return new QObject(parent);
}
