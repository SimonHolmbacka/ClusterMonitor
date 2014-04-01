# Plik utworzony przez menad?era projekt�w kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�wnego katalogu projektu: ./test
# Cel to program:  

include (../Flags.pri)  

symbian{
POST_TARGETDEPS += ../analogwidgets/libanalogwidgets.a 
LIBS += ../analogwidgets/libanalogwidgets.a  
}


unix{
POST_TARGETDEPS += ../analogwidgets/libanalogwidgets.a 
LIBS += ../analogwidgets/libanalogwidgets.a  
}

win32-g++:{
POST_TARGETDEPS += ../analogwidgets/libanalogwidgets.a 
LIBS += ../analogwidgets/libanalogwidgets.a  
}


win32-msvc*:{
 POST_TARGETDEPS += ../analogwidgets/analogwidgets.lib
 LIBS += ../analogwidgets/analogwidgets.lib
}

wince*:{
 POST_TARGETDEPS += ../analogwidgets/analogwidgets.lib
 LIBS += ../analogwidgets/analogwidgets.lib
}



INCLUDEPATH += ../analogwidgets/analogwidgets \
               ../analogwidgets 
MOC_DIR = objects 
UI_DIR = . 
OBJECTS_DIR = ../objects 

CONFIG += release \
          warn_on 
TEMPLATE = app
TARGET = test

RESOURCES += test.qrc 
FORMS += testform.ui
DESTDIR=../test 
HEADERS += \
           widgettester.h \ 
    init.h \
    monitor.h \
    utils.h

SOURCES += main.cpp \
           widgettester.cpp \ 
    monitor.cpp \
    utils.cpp
