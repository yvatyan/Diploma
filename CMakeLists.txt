cmake_minimum_required(VERSION 2.8.11)
set(QT_MINIMUM_VERSION 5.0.0)

project(Diploma)
# SHOULD BE CONFIGURED!!!
set(QT_DIR /home/binary_mind/Qt5.8.0/5.8)

# Find includes in corresponding build directories (e.g. find generated ui.h files)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc/uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Specify cmake scripts of Qt5
set(CMAKE_PREFIX_PATH ${QT_DIR}/gcc_64/lib/cmake)

# Add include directories 
include_directories(
    Core
    Core/ThreadManager
    GUI
    GUI/VideoPlayer
)

# Tell CMake to create the "Demo" executable
add_executable(
    Demo 
    Main.cpp
    Core/ImageData.cpp 
    Core/Job.cpp 
    Core/OptionsManager.cpp 
    Core/VideoFrameProcessor.cpp 
    Core/ThreadManager/ThreadRunnableTask.cpp 
    GUI/ApplicationStyles.cpp 
    GUI/FileLineEdit.cpp 
    GUI/MainWindow.cpp 
    GUI/VideoPlayer/Background.cpp 
    GUI/VideoPlayer/CoverWidget.cpp 
    GUI/VideoPlayer/VideoPlayer.cpp 
    GUI/VideoPlayer/VideoWidget.cpp 
)

# Find the Qt libraries
find_package(Qt5Widgets)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)

# Use the modules from Qt 5
target_link_libraries(Demo Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets)

