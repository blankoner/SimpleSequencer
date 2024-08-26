#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <QCheckBox>
#include <QThread>
#include <string>

#include "audio.h"

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
    void Play();
    void Stop();

private:
    Ui::MainWindow* ui;
    int m_stepsNum;
    int m_channel;
    bool* m_steps;
    Audio m_audioFormat;
    Mix_Chunk* m_sound;
    //bool m_isPlaying;
    QThread playingThread;
    int m_index;

    bool Init();
    void SetFormat();
    bool CreateAudioDevice();
    void LoadSounds();
    void ReadSteps();
    void PlaySound();
    void Clear();
};
#endif // MAINWINDOW_H
