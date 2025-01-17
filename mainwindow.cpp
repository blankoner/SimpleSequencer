#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mainLayout(new QVBoxLayout()) // Inicjalizacja mainLayout przed użyciem
    , sliderLayout(new QHBoxLayout())
    , topLayout(new QHBoxLayout())
{
    ui->setupUi(this);

    // basic window size and tracks numbers
    m_width = 1100;
    m_height = 100;
    m_expandValue = 50;
    m_trackLength = 32;
    m_tracksInitNum = 4;
    m_tracksNum = 0;
    m_tracksLimit = 8;
    m_playPos = 0;

    m_playingWorker = new Worker("", 333);
    m_playingWorker->moveToThread(&m_playingThread);

    if(!InitAudio())
    {
        qDebug() << "Couldn't load the audio!";
    }

    // Ustawienie właściwości okna, dodanie przycisku
    SetWindowProperties();
    SetProgramBase();

    // Połączenie sygnału kliknięcia przycisku z slotem AddLayout
    connect(addButton, &QPushButton::clicked, this, &MainWindow::AddLayout);
    connect(delButton, &QPushButton::clicked, this, &MainWindow::DelLayout);

    // Polaczenia z workerem
    connect(playButton, &QPushButton::clicked, this, [this](){ emit startPlaying(); m_playingThread.start(); });
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::Stop);
    connect(this, &MainWindow::startPlaying, m_playingWorker, &Worker::Play);
    connect(m_playingWorker, &Worker::play, this, &MainWindow::Play);
    connect(m_playingWorker, &QThread::finished, m_playingWorker, &QObject::deleteLater);
    connect(m_bpmBox, &QSpinBox::textChanged, this, &MainWindow::SetBPM);
}

MainWindow::~MainWindow()
{
    Mix_CloseAudio();
    SDL_Quit();
    delete m_playingWorker;
    m_playingThread.requestInterruption();
    m_playingThread.quit();
    delete ui;
}

void MainWindow::SetWindowProperties()
{
    // Ustawienie głównego układu pionowego na centralnym widgetcie
    ui->centralwidget->setLayout(mainLayout);

    // Ustawienie rozmiaru i tytułu okna
    setFixedSize(m_width, m_height);
    setWindowTitle("Sequencer");
}

void MainWindow::AddPosSlider()
{
    m_posSlider = new QSlider(Qt::Horizontal, this);
    m_posSlider->setValue(0);
    m_posSlider->setMaximum(m_trackLength);
    m_posSlider->setMinimum(0);
    m_posSlider->setFixedWidth(840);
    sliderLayout->addWidget(m_posSlider);
    connect(m_posSlider, &QSlider::valueChanged, [this](){ SetPlayPos(); });
}

void MainWindow::SetProgramBase()
{
    // Tworzenie przyciskow z odpowiednim tekstem
    addButton = new QPushButton("+", this);
    addButton->setFixedSize(30, 20);

    delButton = new QPushButton("-", this);
    delButton->setFixedSize(30, 20);

    playButton = new QPushButton("PLAY", this);
    playButton->setFixedSize(50, 20);

    stopButton = new QPushButton("STOP", this);
    stopButton->setFixedSize(50, 20);

    // Tworzenie BPMBoxa
    m_bpmBox = new QSpinBox(this);
    m_bpmBox->setMinimum(1);
    m_bpmBox->setMaximum(360);
    m_bpmBox->setValue(90);
    m_bpmBox->setFixedSize(70, 20);

    // Dodadnie posSlidera
    AddPosSlider();
    mainLayout->addLayout(sliderLayout);

    // Dodanie przyciskow do głównego layoutu
    topLayout->addWidget(addButton);
    topLayout->addWidget(delButton);
    topLayout->addWidget(playButton);
    topLayout->addWidget(stopButton);
    topLayout->addWidget(m_bpmBox);
    mainLayout->addLayout(topLayout);

    // Dodanie podstawowych trackow i wczytanie podstawowych dzwiekow
    for(unsigned int i = 0; i < m_tracksInitNum; i++)
    {
        AddLayout();
    }
    LoadBasicSounds();
}

void MainWindow::LoadBasicSounds()
{
    const char* path = "/home/marceltracz/music/ft2/SAMPLES/9th_wonder_drum_kit/Kicks/Bck_K1.wav";
    m_sounds[0] = Mix_LoadWAV(path);
    SetTrackName(0, path);

    path = "/home/marceltracz/music/ft2/SAMPLES/9th_wonder_drum_kit/Hi-Hats/Bck_H.wav";
    m_sounds[1] = Mix_LoadWAV(path);
    SetTrackName(1, path);

    path = "/home/marceltracz/music/ft2/SAMPLES/9th_wonder_drum_kit/Snares/Bck_Snr.wav";
    m_sounds[2] = Mix_LoadWAV(path);
    SetTrackName(2, path);

    path = "/home/marceltracz/music/ft2/SAMPLES/Blu Mar Ten - Jungle Jungle/Pads/God Chord Pad.wav";
    m_sounds[3] = Mix_LoadWAV(path);
    SetTrackName(3, path);
}

void MainWindow::AddLayout()
{
    if(m_tracksNum < m_tracksLimit)
    {
        // Tworzenie nowego układu poziomego
        QHBoxLayout* newTrack = new QHBoxLayout();
        m_tracks.push_back(newTrack);

        // Tworzenie przycisku z dzwiekiem na poczatku tracku
        QPushButton* newSoundButton = new QPushButton();
        newSoundButton->setText("emptyTrack");
        newSoundButton->setMaximumWidth(80);
        newTrack->addWidget(newSoundButton);
        m_soundButtons.push_back(newSoundButton);

        // Polaczenie przycisku i jego indeksu z funkcja MountTrack
        const int trackIndex = m_soundButtons.size() - 1;
        connect(newSoundButton, &QPushButton::clicked, [this, trackIndex](){ this->MountTrack(trackIndex); });

        // Tworzenie pojemnika na wczytanie dzwieku
        m_sounds.push_back(NULL);

        // Tworzenie krokow
        for(unsigned int i = 0; i < m_trackLength; i++)
        {
            QCheckBox* step = new QCheckBox("", this);
            m_steps.push_back(step);
            newTrack->addWidget(step);
        }

        // Tworzenie pokretla glosnosci na koncu tracku
        QDial* newVolumeDial = new QDial();
        newVolumeDial->setFixedSize(40, 40);
        newVolumeDial->setMaximum(100);
        newVolumeDial->setMinimum(0);
        newVolumeDial->setValue(80);
        m_volumeDials.push_back(newVolumeDial);
        newTrack->addWidget(newVolumeDial);

        // Polaczenie pokretla i jego indexu z opcja zmiany glosnosci tracku
        connect(newVolumeDial, &QDial::valueChanged, [trackIndex, newVolumeDial](){ Mix_Volume(trackIndex, newVolumeDial->value()); });

        // Dodanie wewnętrznego układu do głównego layoutu
        mainLayout->addLayout(newTrack);
        m_tracksNum++;

        // Rozszerzenie okna, by zmiescilo dodatkowy track
        m_height += m_expandValue;
        setFixedSize(m_width, m_height);
    }
}

void MainWindow::ClearLayout(QLayout* layout)
{
    // getting the last layout index to clear
    const int layoutIndex = mainLayout->indexOf(layout);

    // deleting layout's button
    delete m_soundButtons.back();
    m_soundButtons.pop_back();

    // deleting layout's step
    for(unsigned int i = 0; i < m_trackLength; i++)
    {
        delete m_steps.back();
        m_steps.pop_back();
    }

    // deleting layout's vol dial
    delete m_volumeDials.back();
    m_volumeDials.pop_back();

    // deleting the layout from mainLayout and making the memory free
    QLayoutItem* item = mainLayout->takeAt(layoutIndex);
    delete item;

    layout = nullptr;
}

void MainWindow::DelLayout()
{
    // works only if there are tracks added to mainLayout
    if(m_tracksNum > 0)
    {
        ClearLayout(m_tracks.back());
        Mix_FreeChunk(m_sounds.back());
        m_sounds.pop_back();

        // repair the window size
        m_height -= m_expandValue;
        setFixedSize(m_width, m_height);
        m_tracksNum--;
    }
}

bool MainWindow::InitSDL()
{
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        return false;
    }
    return true;
}

void MainWindow::SetFormat()
{
    m_audioFormat.frequency = 44100;
    m_audioFormat.format = AUDIO_S16SYS;
    m_audioFormat.channels = 2;
    m_audioFormat.buffer = 1024;
}

bool MainWindow::CreateDevice()
{
    if(Mix_OpenAudio(m_audioFormat.frequency, m_audioFormat.format, m_audioFormat.channels, m_audioFormat.buffer) < 0)
    {
        return false;
    }
    Mix_AllocateChannels(m_tracksLimit);
    return true;
}

bool MainWindow::InitAudio()
{
    if(InitSDL())
    {
        SetFormat();
        if(CreateDevice())
        {
            return true;
        }
    }
    return false;
}

void MainWindow::PlayChannel(int channel)
{
    // plays only if there is any sound loaded and if the channel is in the range of vector with sounds
    if(m_sounds[channel] && (channel >= 0))
    {
        int playingChannel = 0;
        playingChannel = Mix_PlayChannel(channel, m_sounds[channel], 0);
    }
}

void MainWindow::MountTrack(unsigned int track)
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("WAV (*.wav)"));

    if(filePath.isEmpty())
    {
        qDebug() << "Empty filePath!";
        return;
    }

    m_sounds[track] = Mix_LoadWAV(filePath.toStdString().c_str());

    SetTrackName(track, filePath);
}

void MainWindow::SetTrackName(unsigned int track, QString path)
{
    QFileInfo fileInfo(path);
    QString name = fileInfo.baseName();
    m_soundButtons[track]->setText(name.toStdString().c_str());
}

void MainWindow::Play()
{
    if(m_playPos == m_trackLength)
    {
        m_playPos = 0;
    }

    m_posSlider->setValue(m_playPos);

    for(unsigned int i = 0; i < m_tracksNum; i++)
    {
        if(m_tracks[i])
        {
            int playIndex = m_playPos + (i * m_trackLength);
            if((playIndex < m_steps.size()) && (m_steps[playIndex]->isChecked()))
            {
                PlayChannel(i);
            }
        }
    }

    m_playPos++;
}

void MainWindow::Stop()
{
    m_playingThread.requestInterruption();
    m_playingThread.quit();
    Mix_HaltChannel(-1);
    m_playPos = 0;
    m_posSlider->setValue(m_playPos);
}

void MainWindow::SetBPM()
{
    // Setting BPM with "ms to BPM" mathematical formula
    int bpm = m_bpmBox->value();
    m_playingWorker->SetTime(((double)60000/(double)bpm)/(double)2);
}

void MainWindow::SetPlayPos()
{
    m_playPos = m_posSlider->value();
}
