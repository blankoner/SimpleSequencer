#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QDial>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "audio.h"
#include "worker.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QSpinBox>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startPlaying();

private:
    Ui::MainWindow *ui;

    // variables making the layout
    unsigned int m_width, m_height, m_expandValue, m_trackLength, m_tracksInitNum, m_tracksNum, m_tracksLimit;
    std::vector<QCheckBox*> m_steps;
    std::vector<QHBoxLayout*> m_tracks;
    std::vector<QPushButton*> m_soundButtons;
    std::vector<QDial*> m_volumeDials;
    QVBoxLayout* mainLayout;
    QHBoxLayout* sliderLayout, *topLayout;
    QPushButton* addButton, *delButton, *playButton, *stopButton;
    QSpinBox* m_bpmBox;
    QSlider* m_posSlider;

    // layout functions
    void SetWindowProperties();
    void AddPosSlider();
    void SetProgramBase();
    void AddLayout();
    void DelLayout();
    void ClearLayout(QLayout* layout);

    // audio variables
    Audio m_audioFormat;
    std::vector<Mix_Chunk*> m_sounds;
    unsigned int m_playPos;

    // audio functions
    bool InitSDL();
    void SetFormat();
    bool CreateDevice();
    bool InitAudio();
    void LoadBasicSounds();
    void PlayChannel(int channel);
    void MountTrack(unsigned int track);
    void SetTrackName(unsigned int track, QString path);
    void Play();
    void Stop();
    void SetBPM();
    void ChangeVolume(unsigned int track);
    void SetPlayPos();

    // workers
    Worker* m_playingWorker;
    QThread m_playingThread;

};
#endif // MAINWINDOW_H
