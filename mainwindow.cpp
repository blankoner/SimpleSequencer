#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::StartPlaying);
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(Stop()));
    connect(ui->tempoBox, &QSpinBox::textChanged, this, &MainWindow::SetTempo);

    connect(this, &MainWindow::PlaySignal, m_playingWorker, &Worker::Play);
    connect(m_playingWorker, &Worker::play, this, &MainWindow::Play);
    connect(m_playingWorker, &QThread::finished, m_playingWorker, &QObject::deleteLater);

    m_playingWorker->moveToThread(&m_playingThread);

    m_channels = 3;
    m_sounds.reserve(m_channels);
    m_index = 0;
    m_stepsNum = 16;
    m_steps0 = new bool[m_stepsNum];
    m_steps1 = new bool[m_stepsNum];
    m_steps2 = new bool[m_stepsNum];

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
    for(int i = 0; i < 3; i++)
    {
        Mix_FreeChunk(m_sounds[i]);
    }
    Mix_CloseAudio();
    SDL_Quit();
    delete[] m_steps0;
    delete[] m_steps1;
    delete[] m_steps2;
}

void MainWindow::SetFormat()
{
    m_audioFormat.frequency = 44100;
    m_audioFormat.format = AUDIO_S16SYS;
    m_audioFormat.channels = 1;
    m_audioFormat.buffer = 512;
}

bool MainWindow::CreateAudioDevice()
{
    if(Mix_OpenAudio(m_audioFormat.frequency, m_audioFormat.format, m_audioFormat.channels, m_audioFormat.buffer) < 0)
    {
        return false;
    }
    Mix_AllocateChannels(m_channels);
    return true;
}

void MainWindow::LoadSounds()
{
    m_sounds[0] = Mix_LoadWAV("/home/marceltracz/Projekty/audio-programming/SequencerTest/SimpleSequencer/sounds/808-bassdrum.wav");
    if(!m_sounds[0])
    {
        ui->errorView->setText("Error with loading sound!");
    }

    m_sounds[1] = Mix_LoadWAV("/home/marceltracz/Projekty/audio-programming/SequencerTest/SimpleSequencer/sounds/808-hihat.wav");
    if(!m_sounds[1])
    {
        ui->errorView->setText("Error with loading sound!");
    }

    m_sounds[2] = Mix_LoadWAV("/home/marceltracz/Projekty/audio-programming/SequencerTest/SimpleSequencer/sounds/808-clap.wav");
    if(!m_sounds[2])
    {
        ui->errorView->setText("Error with loading sound!");
    }
}

void MainWindow::ReadSteps()
{
    m_steps0[0] = ui->step01->checkState();
    m_steps0[1] = ui->step02->checkState();
    m_steps0[2] = ui->step03->checkState();
    m_steps0[3] = ui->step04->checkState();
    m_steps0[4] = ui->step05->checkState();
    m_steps0[5] = ui->step06->checkState();
    m_steps0[6] = ui->step07->checkState();
    m_steps0[7] = ui->step08->checkState();
    m_steps0[8] = ui->step09->checkState();
    m_steps0[9] = ui->step0A->checkState();
    m_steps0[10] = ui->step0B->checkState();
    m_steps0[11] = ui->step0C->checkState();
    m_steps0[12] = ui->step0D->checkState();
    m_steps0[13] = ui->step0E->checkState();
    m_steps0[14] = ui->step0F->checkState();
    m_steps0[15] = ui->step0G->checkState();

    m_steps1[0] = ui->step11->checkState();
    m_steps1[1] = ui->step12->checkState();
    m_steps1[2] = ui->step13->checkState();
    m_steps1[3] = ui->step14->checkState();
    m_steps1[4] = ui->step15->checkState();
    m_steps1[5] = ui->step16->checkState();
    m_steps1[6] = ui->step17->checkState();
    m_steps1[7] = ui->step18->checkState();
    m_steps1[8] = ui->step19->checkState();
    m_steps1[9] = ui->step1A->checkState();
    m_steps1[10] = ui->step1B->checkState();
    m_steps1[11] = ui->step1C->checkState();
    m_steps1[12] = ui->step1D->checkState();
    m_steps1[13] = ui->step1E->checkState();
    m_steps1[14] = ui->step1F->checkState();
    m_steps1[15] = ui->step1G->checkState();

    m_steps2[0] = ui->step21->checkState();
    m_steps2[1] = ui->step22->checkState();
    m_steps2[2] = ui->step23->checkState();
    m_steps2[3] = ui->step24->checkState();
    m_steps2[4] = ui->step25->checkState();
    m_steps2[5] = ui->step26->checkState();
    m_steps2[6] = ui->step27->checkState();
    m_steps2[7] = ui->step28->checkState();
    m_steps2[8] = ui->step29->checkState();
    m_steps2[9] = ui->step2A->checkState();
    m_steps2[10] = ui->step2B->checkState();
    m_steps2[11] = ui->step2C->checkState();
    m_steps2[12] = ui->step2D->checkState();
    m_steps2[13] = ui->step2E->checkState();
    m_steps2[14] = ui->step2F->checkState();
    m_steps2[15] = ui->step2G->checkState();
}

void MainWindow::PlaySound(int channel)
{
    int playingChannel = 0;
    playingChannel = Mix_PlayChannel(channel, m_sounds[channel], 0);
}

void MainWindow::SetTempo()
{
    int tempo = ui->tempoBox->value();
    m_playingWorker->SetTime(((double)60000/(double)tempo)/(double)2);
}

void MainWindow::StartPlaying()
{
    emit PlaySignal();
    m_playingThread.start();
}

void MainWindow::Play()
{
    if(m_index == m_stepsNum)
    {
        m_index = 0;
    }

    ReadSteps();
    if(m_steps0[m_index])
    {
        PlaySound(0);
    }

    if(m_steps1[m_index])
    {
        PlaySound(1);
    }

    if(m_steps2[m_index])
    {
        PlaySound(2);
    }

    m_index++;
}

void MainWindow::Stop()
{
    //m_isPlaying = false;
    m_playingThread.requestInterruption();
    m_playingThread.quit();
    Mix_HaltChannel(-1);
}
