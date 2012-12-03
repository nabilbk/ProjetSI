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
               Gui/mainwindow.cpp \
               Pass/passchain.cpp \
               controller.cpp \
               circle.cpp \
               Pass/customcircledetectionpass.cpp \
               Pass/roipass.cpp \
               Gui/parameterdock.cpp \
               Gui/imagescaledialog.cpp \
               Gui/customgraphicsscene.cpp \
               videoprocessor.cpp \
               Pass/cannyfilterpass.cpp \
    Gui/customgraphicsview.cpp \
    Pass/angledetectionpass.cpp \
    Pass/circledetectionpass.cpp \
    Gui/aboutdialog.cpp \
    Gui/extractsequencedialog.cpp \
    Pass/meanshiftfilterpass.cpp \
    dynamicpasschain.cpp \
    Pass/sobelfilterpass.cpp \
    Gui/savedialog.cpp

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
               Gui/mainwindow.h \
               Pass/passchain.h \
               controller.h \
               circle.h \
               Pass/customcircledetectionpass.h \
               Pass/roipass.h \
               Gui/parameterdock.h \
               Gui/imagescaledialog.h \
               Gui/customgraphicsscene.h \
               videoprocessor.h \
               Pass/cannyfilterpass.h \
    Gui/customgraphicsview.h \
    Pass/angledetectionpass.h \
    Pass/circledetectionpass.h \
    Gui/aboutdialog.h \
    Gui/extractsequencedialog.h \
    Pass/meanshiftfilterpass.h \
    dynamicpasschain.h \
    Pass/sobelfilterpass.h \
    Gui/savedialog.h


# Fichiers UI  compiler
FORMS       += Gui/mainwindow.ui \
    Gui/parameterdock.ui \
    Gui/imagescaledialog.ui \
    Gui/aboutdialog.ui \
    Gui/extractsequencedialog.ui \
    dynamicpasschain.ui \
    Gui/savedialog.ui

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

OTHER_FILES += \
    README.md
