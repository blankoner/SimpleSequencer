#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(StartPlaying()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(Stop()));
    connect(ui->tempoBox, SIGNAL(textChanged(QString)), this, SLOT(SetTempo()));
    connect(ui->volDial0, SIGNAL(valueChanged(int)), this, SLOT(SetVolume(int)));
    connect(ui->volDial1, SIGNAL(valueChanged(int)), this, SLOT(SetVolume(int)));
    connect(ui->volDial2, SIGNAL(valueChanged(int)), this, SLOT(SetVolume(int)));
    connect(ui->volDial3, SIGNAL(valueChanged(int)), this, SLOT(SetVolume(int)));
    connect(ui->trackButton00, &QPushButton::clicked, this, [this](){ MountTrack(0); });
    connect(ui->trackButton01, &QPushButton::clicked, this, [this](){ MountTrack(1); });
    connect(ui->trackButton02, &QPushButton::clicked, this, [this](){ MountTrack(2); });
    connect(ui->trackButton03, &QPushButton::clicked, this, [this](){ MountTrack(3); });
    connect(ui->positionSlider, &QSlider::sliderReleased, this, [this](){ m_index = ui->positionSlider->value(); });

    connect(this, &MainWindow::PlaySignal, m_playingWorker, &Worker::Play);
    connect(m_playingWorker, &Worker::play, this, &MainWindow::Play);
    connect(m_playingWorker, &QThread::finished, m_playingWorker, &QObject::deleteLater);

    m_playingWorker->moveToThread(&m_playingThread);

    m_channels = 4;
    m_sounds.reserve(m_channels);
    m_index = 0;
    m_stepsNum = 16;
    m_steps0 = new bool[m_stepsNum];
    m_steps1 = new bool[m_stepsNum];
    m_steps2 = new bool[m_stepsNum];
    m_steps3 = new bool[m_stepsNum];

    if(Init())
    {
        SetFormat();
        if(CreateAudioDevice())
        {
            LoadBasicSounds();
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
    for(int i = 0; i < m_channels; i++)
    {
        Mix_FreeChunk(m_sounds[i]);
    }
    Mix_CloseAudio();
    SDL_Quit();
    delete[] m_steps0;
    delete[] m_steps1;
    delete[] m_steps2;
    delete[] m_steps3;
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

void MainWindow::LoadBasicSounds()
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

    m_sounds[3] = Mix_LoadWAV("/home/marceltracz/Projekty/audio-programming/SequencerTest/SimpleSequencer/sounds/pad.wav");
    if(!m_sounds[3])
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

    m_steps3[0] = ui->step31->checkState();
    m_steps3[1] = ui->step32->checkState();
    m_steps3[2] = ui->step33->checkState();
    m_steps3[3] = ui->step34->checkState();
    m_steps3[4] = ui->step35->checkState();
    m_steps3[5] = ui->step36->checkState();
    m_steps3[6] = ui->step37->checkState();
    m_steps3[7] = ui->step38->checkState();
    m_steps3[8] = ui->step39->checkState();
    m_steps3[9] = ui->step3A->checkState();
    m_steps3[10] = ui->step3B->checkState();
    m_steps3[11] = ui->step3C->checkState();
    m_steps3[12] = ui->step3D->checkState();
    m_steps3[13] = ui->step3E->checkState();
    m_steps3[14] = ui->step3F->checkState();
    m_steps3[15] = ui->step3G->checkState();
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

void MainWindow::SetVolume(int value)
{
    QObject* dial = sender();
    int channel = -1;

    if(dial == ui->volDial0)
    {
        channel = 0;
    }
    else if(dial == ui->volDial1)
    {
        channel = 1;
    }
    else if(dial == ui->volDial2)
    {
        channel = 2;
    }
    else if(dial == ui->volDial3)
    {
        channel = 3;
    }

    if(channel != -1)
    {
        Mix_Volume(channel, value);
    }
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

    ui->positionSlider->setValue(m_index);
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

    if(m_steps3[m_index])
    {
        PlaySound(3);
    }

    m_index++;
}

void MainWindow::Stop()
{
    //m_isPlaying = false;
    m_playingThread.requestInterruption();
    m_playingThread.quit();
    Mix_HaltChannel(-1);
    //m_index = 0;
    //ui->positionSlider->setValue(m_index);
}

void MainWindow::MountTrack(unsigned int track)
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("WAV (*.wav)"));

    m_sounds[track] = Mix_LoadWAV(filePath.toStdString().c_str());

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.baseName();
    SetTrackName(track, fileName.toStdString().c_str());

    if(!m_sounds[track])
    {
        ui->errorView->setText("Error with loading sound!");
    }
}

void MainWindow::SetTrackName(unsigned int track, const char* name)
{
    switch(track)
    {
    case 0:
        ui->trackButton00->setText(name);
        break;
    case 1:
        ui->trackButton01->setText(name);
        break;
    case 2:
        ui->trackButton02->setText(name);
        break;
    case 3:
        ui->trackButton03->setText(name);
        break;
    default:
        return;
    }
}
