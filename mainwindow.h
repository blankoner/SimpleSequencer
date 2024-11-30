#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <QThread>
#include <vector>
#include <QFileDialog>
#include <QFileInfo>
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
    void SetVolume(int);

signals:
    void PlaySignal();

private:
    Ui::MainWindow* ui;
    int m_stepsNum;
    int m_channels;
    bool* m_steps0, *m_steps1, *m_steps2, *m_steps3;
    Audio m_audioFormat;
    std::vector<Mix_Chunk*> m_sounds;
    QThread m_playingThread;
    unsigned int m_index;

    Worker* m_playingWorker = new Worker("1", 333);

    bool Init();
    void SetFormat();
    bool CreateAudioDevice();
    void LoadBasicSounds();
    void ReadSteps();
    void PlaySound(int channel);
    void SetTempo();
    void StartPlaying();
    void Play();
    void Clear();
    void MountTrack(unsigned int track);
    void SetTrackName(unsigned int track, const char* name);
};
#endif // MAINWINDOW_H
