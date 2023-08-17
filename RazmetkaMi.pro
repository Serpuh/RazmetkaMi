#-------------------------------------------------
#
# Project created by QtCreator 2022-06-27T09:59:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RazmetkaMi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH  +=  \
C:\OpenCV\opencv-4.5.5\build \
C:\OpenCV\opencv-4.5.5\modules\core\include \
C:\OpenCV\opencv-4.5.5\modules\imgcodecs\include \
C:\OpenCV\opencv-4.5.5\modules\imgproc\include \
C:\OpenCV\opencv_contrib-master\modules\aruco\include \
C:\OpenCV\opencv_contrib-master\modules\xfeatures2d\include \
C:\OpenCV\opencv-4.5.5\modules\calib3d\include \
C:\OpenCV\opencv-4.5.5\modules\highgui\include \
C:\OpenCV\opencv-4.5.5\modules\features2d\include \
C:\OpenCV\opencv-4.5.5\modules\flann\include \
C:\OpenCV\opencv-4.5.5\modules\videoio\include

LIBS += \
C:\OpenCV\opencv-4.5.5\build\lib\Release\opencv_core455.lib \
C:\OpenCV\opencv-4.5.5\build\lib\Release\opencv_imgproc455.lib \
C:\OpenCV\opencv-4.5.5\build\lib\Release\opencv_imgcodecs455.lib \
C:\OpenCV\opencv-4.5.5\build\lib\Release\opencv_highgui455.lib \
C:\OpenCV\opencv-4.5.5\build\lib\Release\opencv_videoio455.lib \
C:\OpenCV\opencv-4.5.5\build\lib\Release\opencv_aruco455.lib \
C:\OpenCV\opencv-4.5.5\build\lib\Release\opencv_calib3d455.lib

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    PicView.cpp \
    func_new.cpp \
    ViewPort.cpp \
    GrView.cpp

HEADERS += \
    mainwindow.h \
    PicView.h \
    func_new.h \
    ViewPort.h \
    GrView.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc

win32: RC_ICONS = $$PWD/res/pen.ico
