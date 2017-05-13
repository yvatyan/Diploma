cmake_minimum_required(VERSION 2.8.11)
set(QT_MINIMUM_VERSION 5.0.0)

project(Diploma)

# SHOULD BE CONFIGURED!!!
set(QT_DIR /home/binary_mind/Qt5.8.0/5.8)
set(OpenCV_DIR /home/binary_mind/OpenCV3.2.0)

# Find includes in corresponding build directories (e.g. find generated ui.h files)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc/uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Specify cmake scripts
## OpenCVModules.cmake and OpenCVConfig.cmake files should be copy from build to cmake directory 
set(CMAKE_PREFIX_PATH ${QT_DIR}/gcc_64/lib/cmake;${OpenCV_DIR}/cmake)

# Find the packages
find_package(Qt5Widgets)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)
find_package(OpenCV)
find_package(PkgConfig)
pkg_check_modules(GTK2 gtk+-2.0)
find_package(Threads)

# Specify include directories
include_directories(
    Core/ProcessingEngine/OpenSift/include
	${GTK2_INCLUDE_DIRS}
)

# Tell CMake to create the "Demo" executable
add_executable(
    Demo 
    Main.cpp
    Core/ImageData.cpp 
	Core/Misc.cpp
    Core/OptionsManager.cpp 
	Core/ProcessingEngine/Demo/MonoChromeProc.cpp
	Core/ProcessingEngine/OpenSift/src/imgfeatures.cpp
	Core/ProcessingEngine/OpenSift/src/sift.cpp
	Core/ProcessingEngine/OpenSift/src/utils.cpp
	Core/ProcessingEngine/SiftFeatures.cpp
    Core/VideoFrameProcessor.cpp 
	Core/VideoFrameData.cpp
    Core/ThreadManager/ThreadRunnableTask.cpp 
    GUI/ApplicationStyles.cpp 
    GUI/FileLineEdit.cpp 
    GUI/MainWindow.cpp 
    GUI/VideoPlayer/Background.cpp 
    GUI/VideoPlayer/CoverWidget.cpp 
    GUI/VideoPlayer/VideoPlayer.cpp 
    GUI/VideoPlayer/VideoWidget.cpp 
)

# Use the modules
target_link_libraries(
	Demo
	Qt5::Widgets
	Qt5::Multimedia
	Qt5::MultimediaWidgets
	${OpenCV_LIBS}
	${GTK2_LIBRARIES}
)

