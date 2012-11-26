# Configuration QT
QT          += core gui opengl

# Type d'objet gnr et destination
TARGET       = projet-si
TEMPLATE     = app

QMAKE_CXXFLAGS += -DEBUG -g -Wall

# Fichier sources
SOURCES     += \
               Pass/binaryconversionpass.cpp \
               Pass/blurpass.cpp \
               Pass/houghcircledetectionpass.cpp \
               Pass/gaussianblurpass.cpp \
               Pass/graylevelpass.cpp \
               videoextractor.cpp \
               imageconverter.cpp \
               imageprocessor.cpp \
               Pass/ipass.cpp \
               main.cpp \
               mainwindow.cpp \
               Pass/passchain.cpp \
               controller.cpp \
               circle.cpp \
               Pass/customcircledetectionpass.cpp \
               Pass/roipass.cpp \
               parameterdock.cpp \
               imagescaledialog.cpp \
               customgraphicsscene.cpp \
               videoprocessor.cpp \
               Pass/cannyfilterpass.cpp \
    customgraphicsview.cpp \
    Pass/angledetectionpass.cpp \
    Pass/circledetectionpass.cpp \
    aboutdialog.cpp \
    extractsequencedialog.cpp \
    Pass/meanshiftfilterpass.cpp \
    dynamicpasschain.cpp \
    Pass/sobelfilterpass.cpp \
    savedialog.cpp

# Fichiers d'entte
HEADERS     += \
               Pass/binaryconversionpass.h \
               Pass/blurpass.h \
               Pass/houghcircledetectionpass.h \
               Pass/gaussianblurpass.h \
               Pass/graylevelpass.h \
               videoextractor.h \
               imageconverter.h \
               imageprocessor.h \
               Pass/ipass.h \
               mainwindow.h \
               Pass/passchain.h \
               controller.h \
               circle.h \
               Pass/customcircledetectionpass.h \
               Pass/roipass.h \
               parameterdock.h \
               imagescaledialog.h \
               customgraphicsscene.h \
               videoprocessor.h \
               Pass/cannyfilterpass.h \
    customgraphicsview.h \
    Pass/angledetectionpass.h \
    Pass/circledetectionpass.h \
    aboutdialog.h \
    extractsequencedialog.h \
    Pass/meanshiftfilterpass.h \
    dynamicpasschain.h \
    Pass/sobelfilterpass.h \
    savedialog.h


# Fichiers UI  compiler
FORMS       += mainwindow.ui \
    parameterdock.ui \
    imagescaledialog.ui \
    aboutdialog.ui \
    extractsequencedialog.ui \
    dynamicpasschain.ui \
    savedialog.ui

# Fichiers ressources
RESOURCES   += toolbar.qrc
#RC_FILE     += resources.rc

# Bibliothques  inclure
LIBS += -lopencv_core \
        -lopencv_gpu \
        -lopencv_video \
        -lopencv_legacy \
        -lopencv_imgproc \
        -lopencv_highgui
