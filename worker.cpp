#include "worker.h"

Worker::Worker(QString id, unsigned int time, QObject* parent) :
    QThread(parent),
    m_id(id),
    m_time(time)
{
}

Worker::~Worker()
{
}

void Worker::SetTime(const unsigned int& time)
{
    m_time = time;
}

void Worker::Play()
{
    while(!(QThread::currentThread()->isInterruptionRequested()))
    {
        emit play();
        msleep(m_time);
    }
}
