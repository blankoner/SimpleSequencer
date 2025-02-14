#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "audio.h"
#include "worker.h"
#include <vector>

#include <QMainWindow>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QDial>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QSpinBox>
#include <QSlider>
#include <QResource>
#include <QLabel>

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
    std::vector<QPushButton*> m_soundButtons, m_muteButtons;
    std::vector<QDial*> m_volumeDials, m_panningDials;
    QVBoxLayout* mainLayout;
    QHBoxLayout* sliderLayout, *topLayout;
    QPushButton* m_addButton, *m_delButton, *m_playButton, *m_stopButton, *m_pauseButton;
    QSpinBox* m_bpmBox;
    QSlider* m_posSlider;

    // layout functions
    void SetWindowProperties();
    void AddPosSlider();
    void AddMenuButtons();
    void AddMenuLabels();
    void AddBPMBox();
    void SetProgramBase();
    void AddLayout();
    void DelLayout();
    void ClearLayout(QLayout* layout);
    QDial* CreateDial(int value);

    // audio variables
    Audio m_audioFormat;
    std::vector<Mix_Chunk*> m_sounds;
    bool* m_mutedTracks;
    unsigned int m_playPos;

    // audio functions
    bool InitSDL();
    void SetFormat();
    bool CreateDevice();
    bool InitAudio();
    void LoadBasicSounds();
    void PlayChannel(int channel);
    void MountTrack(unsigned int track);
    void SetTrackName(unsigned int track, const QString& path);
    void Play();
    void Pause();
    void Stop();
    void SetBPM();
    void ChangeVolume(unsigned int track);
    void SetPlayPos();
    void MuteTrack(unsigned int track);
    void SetPanning(unsigned int track);
    Mix_Chunk* LoadWAVFromQRC(const QString& path);

    // workers
    Worker* m_playingWorker;
    QThread m_playingThread;

};
#endif // MAINWINDOW_H
