#include "GUI/VideoPlayer/VideoPlayer.h"
#include "Core/ProcessingEngine/Demo/MonoChromeProc.h"
#include "Core/ProcessingEngine/SiftFeatures.h"
#include "Core/OptionsManager.h"

VideoPlayer::VideoPlayer(QWidget* parent)
    : backgroundWidget(parent)
    , videoWidget(parent)
    , coverWidget(parent)
{
    videoWidget.hide();
    state = Idle;
    backgroundWidget.SwitchBgrMode(Background::NoSignal);
    player.setVideoOutput(videoWidget.videoSurface());
    QObject::connect(&player, SIGNAL(positionChanged(qint64)), this, SLOT(OnPositionChange(qint64)));
    QObject::connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(OnDurationChange(qint64)));
    // TODO: Remove these when resize funcionallity will be present
    backgroundWidget.resize(QSize(640, 480));
    videoWidget.resize(QSize(640, 480));

	registerImageProcessingJobs();
	videoWidget.SetVideoFrameProcessor(&videoProcessor);
	videoWidget.SetCoverWidget(&coverWidget);
}
void VideoPlayer::SwitchInput(VideoStreamFrom source) {
    videoSource = source;
}
void VideoPlayer::SetVideoFile(const QString& filePath) {
    videoFilePath = QUrl::fromLocalFile(filePath);
}
void VideoPlayer::FocusIn(){

}
void VideoPlayer::FocusOut(){

}
void VideoPlayer::Pause() {
    if(state == Active) {
        state = Paused;
        if(videoSource == VideoFile) {
            player.pause();
        }
    }
}
void VideoPlayer::Resume() {
    if(state == Paused ) {
        state = Active;
        if(videoSource == VideoFile) {
            player.play();
        }
    }
}
void VideoPlayer::Start() {
    state = Active;
    backgroundWidget.SwitchBgrMode(Background::Black);
    videoWidget.show();
    if(videoSource == VideoFile) {
        player.setMedia(videoFilePath);
        player.play();
    }
}
void VideoPlayer::Stop() {
    videoWidget.hide();
    state = Idle;
    backgroundWidget.SwitchBgrMode(Background::NoSignal);
    if(videoSource == VideoFile) {
        player.stop();
    }
}
size_t VideoPlayer::FPS() const {
    return 0;
}
size_t VideoPlayer::PFPS() const {
    return 0;
}
VideoPlayer::VideoPlayerState VideoPlayer::State() const {
    return state;
}
void VideoPlayer::OnPositionChange(qint64 pos) {
    emit playerPositionChanged((int) pos);
}
void VideoPlayer::OnDurationChange(qint64 dur) {
    emit playerDurationChanged((int) dur);
}
void VideoPlayer::ConnectProgressBar(QProgressBar* pb) {
    if( state == Active ) {
        pb->setMinimum(0);
        QObject::connect(this, SIGNAL(playerPositionChanged(int)), pb, SLOT(setValue(int)));
        QObject::connect(this, SIGNAL(playerDurationChanged(int)), pb, SLOT(setMaximum(int)));
    }
}
void VideoPlayer::registerImageProcessingJobs() {
	if(0) {	// TODO: Here will be control checkbox
		registerJobDemo();
	}
	if(1) {
		registerJobSiftFeatures();
	}
}
void VideoPlayer::registerJobDemo() {
	ThreadRunnableTask taskDemo;
	taskDemo.AddJob(new MonoChromeProc());
	taskDemo.SetTaskName("Demo");
	videoProcessor.AddRunnableTask(taskDemo);

	ConfigCollection taskDemoConfig;
	QString optionValue;
    optionValue = OptionsManager::OptionValue("DemoKernelSize").c_str(); {
        if(optionValue.isEmpty()) {
        } else {
			taskDemoConfig.AddConfig<size_t>("KernelSize", optionValue.toInt());
        }
    }
    optionValue = OptionsManager::OptionValue("DemoThreshold").c_str(); {
        if(optionValue.isEmpty()) {
        } else {
			taskDemoConfig.AddConfig<size_t>("Threshold", optionValue.toInt());
        }
    }
    optionValue = OptionsManager::OptionValue("DemoMode").c_str(); {
        if(optionValue.isEmpty()) {
        } else {
			MonoChromeProc::Mode mode;
			if(optionValue == "Red") {
				mode = MonoChromeProc::Red;
			} else if(optionValue == "Green") {
				mode = MonoChromeProc::Green;
			} else if(optionValue == "Blue") {
				mode = MonoChromeProc::Blue;
			} else if(optionValue == "White") {
				mode = MonoChromeProc::White;
			}
			taskDemoConfig.AddConfig<MonoChromeProc::Mode>("ChromeMode", mode);
        }
    }
    optionValue = OptionsManager::OptionValue("DemoMakeTransformation").c_str(); {
        if(optionValue.isEmpty()) {
        } else {
			taskDemoConfig.AddConfig<bool>("MakeTransformation", optionValue.toInt());
        }
    }
    optionValue = OptionsManager::OptionValue("DemoPixelIntervalStart").c_str(); {
        if(optionValue.isEmpty()) {
        } else {
			taskDemoConfig.AddConfig<size_t>("PixelIntervalStart", optionValue.toInt());
        }
    }
    optionValue = OptionsManager::OptionValue("DemoPixelIntervalEnd").c_str(); {
        if(optionValue.isEmpty()) {
        } else {
			taskDemoConfig.AddConfig<size_t>("PixelIntervalEnd", optionValue.toInt());
        }
    }
	videoProcessor.ConfigureJob("Demo", "Monochrome processing", taskDemoConfig);
}
void VideoPlayer::registerJobSiftFeatures() {
	bool configNeeded = false;	// TODO: HARDCODED
	ThreadRunnableTask task;
	task.AddJob(new SiftFeatures(configNeeded));
	task.SetTaskName("Sift");
	videoProcessor.AddRunnableTask(task);

	if(configNeeded) {
		ConfigCollection taskConfig;
		QString optionValue;
		// TODO: Replace options names startin with __ later for more understandable names
		optionValue = OptionsManager::OptionValue("__SiftIntvls").c_str(); {
			if(optionValue.isEmpty()) {
			} else {
				taskConfig.AddConfig<int>("Intvls", optionValue.toInt());
			}
		}
		optionValue = OptionsManager::OptionValue("SiftSigma").c_str(); {
			if(optionValue.isEmpty()) {
			} else {
				taskConfig.AddConfig<double>("Sigma", optionValue.toDouble());
			}
		}
		optionValue = OptionsManager::OptionValue("__SiftContrThr").c_str(); {
			if(optionValue.isEmpty()) {
			} else {
				taskConfig.AddConfig<double>("ContrThr", optionValue.toDouble());
			}
		}
		optionValue = OptionsManager::OptionValue("__SiftCurvThr").c_str(); {
			if(optionValue.isEmpty()) {
			} else {
				taskConfig.AddConfig<int>("CurvThr", optionValue.toInt());
			}
		}
		optionValue = OptionsManager::OptionValue("__SiftImgDbl").c_str(); {
			if(optionValue.isEmpty()) {
			} else {
				taskConfig.AddConfig<int>("ImgDbl", optionValue.toInt());
			}
		}
		optionValue = OptionsManager::OptionValue("SiftDescriptorWidth").c_str(); {
			if(optionValue.isEmpty()) {
			} else {
				taskConfig.AddConfig<int>("DescriptorWidth", optionValue.toInt());
			}
		}
		optionValue = OptionsManager::OptionValue("SiftHistogramBins").c_str(); {
			if(optionValue.isEmpty()) {
			} else {
				taskConfig.AddConfig<int>("HistogramBins", optionValue.toInt());
			}
		}
		videoProcessor.ConfigureJob("Sift", "Sift Features", taskConfig);
	}
}
