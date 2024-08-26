#include "worker.h"

Worker::Worker(QString _id, int _time, QObject* parent):
    QThread(parent),
    id_(_id),
    time_(_time)
{
}

Worker::~Worker()
{
}

void Worker::Play()
{
    while(true)
    {
        if(QThread::currentThread()->isInterruptionRequested())
        {
            break;
        }
        emit play();
        msleep(time_);
    }
}
