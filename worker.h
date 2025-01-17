#ifndef WORKER_H
#define WORKER_H

#include <QThread>

class Worker : public QThread
{
    Q_OBJECT

public:
    explicit Worker(QString id, unsigned int time, QObject* parent = 0);
    ~Worker();
    void SetTime(const unsigned int& time);

public slots:
    void Play();

signals:
    void play();

private:
    QString m_id;
    unsigned int m_time;
};

#endif // WORKER_H
