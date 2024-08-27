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
    while(!(QThread::currentThread()->isInterruptionRequested()))
    {
        emit play();
        msleep(time_);
    }
}
