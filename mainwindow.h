#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <QCheckBox>
#include <QThread>
#include <string>

#include "audio.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void Play();
    void Stop();

signals:
    void playSignal();

private:
    Ui::MainWindow* ui;
    int m_stepsNum;
    int m_channel;
    bool* m_steps;
    Audio m_audioFormat;
    Mix_Chunk* m_sound;
    //bool m_isPlaying;
    QThread m_playingThread;
    int m_index;

    Worker* m_playingWorker = new Worker("1", 100);

    bool Init();
    void SetFormat();
    bool CreateAudioDevice();
    void LoadSounds();
    void ReadSteps();
    void PlaySound();
    void StartPlaying();
    void Play();
    void Clear();
};
#endif // MAINWINDOW_H
