#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::StartPlaying);
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(Stop()));

    connect(this, &MainWindow::playSignal, m_playingWorker, &Worker::Play);
    connect(m_playingWorker, &Worker::play, this, &MainWindow::Play);
    connect(m_playingWorker, &QThread::finished, m_playingWorker, &QObject::deleteLater);

    m_playingWorker->moveToThread(&m_playingThread);

    m_sound = NULL;
    m_channel = 0;
    m_index = 0;
    m_stepsNum = 8;
    m_steps = new bool[m_stepsNum];
    //m_isPlaying = false;

    if(Init())
    {
        SetFormat();
        if(CreateAudioDevice())
        {
            LoadSounds();
        }
    }
}

MainWindow::~MainWindow()
{
    Clear();
    m_playingThread.requestInterruption();
    m_playingThread.quit();
    delete ui;
}

bool MainWindow::Init()
{
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return false;
    }
    return true;
}

void MainWindow::Clear()
{
    Mix_FreeChunk(m_sound);
    Mix_CloseAudio();
    delete[] m_steps;
    SDL_Quit();
}

void MainWindow::SetFormat()
{
    m_audioFormat.frequency = 44100;
    m_audioFormat.format = AUDIO_S16SYS;
    m_audioFormat.channels = 1;
    m_audioFormat.buffer = 1024;
}

bool MainWindow::CreateAudioDevice()
{
    if(Mix_OpenAudio(m_audioFormat.frequency, m_audioFormat.format, m_audioFormat.channels, m_audioFormat.buffer) < 0)
    {
        return false;
    }
    Mix_AllocateChannels(1);
    return true;
}

void MainWindow::LoadSounds()
{
    m_sound = Mix_LoadWAV("/home/marceltracz/Projekty/audio-programming/SequencerTest/SimpleSequencer/sounds/808-hihat.wav");
    if(!m_sound)
    {
        ui->errorView->setText("Error with loading sound!");
    }
}

void MainWindow::ReadSteps()
{
    m_steps[0] = ui->step01->checkState();
    m_steps[1] = ui->step02->checkState();
    m_steps[2] = ui->step03->checkState();
    m_steps[3] = ui->step04->checkState();
    m_steps[4] = ui->step05->checkState();
    m_steps[5] = ui->step06->checkState();
    m_steps[6] = ui->step07->checkState();
    m_steps[7] = ui->step08->checkState();
}

void MainWindow::PlaySound()
{
    m_channel = Mix_PlayChannel(0, m_sound, 0);
}

void MainWindow::StartPlaying()
{
    emit playSignal();
    m_playingThread.start();
}

void MainWindow::Play()
{
    //m_isPlaying = true;
    if(m_index == m_stepsNum)
    {
        m_index = 0;
    }

    ReadSteps();
    if(m_steps[m_index])
    {
        PlaySound();
    }

    m_index++;
}

void MainWindow::Stop()
{
    //m_isPlaying = false;
    m_playingThread.requestInterruption();
    Mix_HaltChannel(-1);
    //m_playingThread.requestInterruption();
    m_playingThread.quit();
}
