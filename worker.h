#ifndef WORKER_H
#define WORKER_H

#include <QThread>

class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(QString _id, int _time, QObject* parent=0);
    ~Worker();

public slots:
    void Play();

signals:
    void add(QString s);

private:
    QString id_;
    int time_;

//friend class MainWindow;
};


#endif // WORKER_H
