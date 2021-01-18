QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appointment_request_widget.cc \
    appointment_widget.cc \
    main.cc \
    main_window.cc \
    request_widget_base.cc

HEADERS += \
    appointment_request_widget.h \
    appointment_widget.h \
    main_window.h \
    request_widget_base.h

FORMS += \
    appointment_request_widget.ui \
    appointment_widget.ui \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../appointy/build/appointy/Debug/bin/ -lappointy

INCLUDEPATH += $$PWD/../appointy/src
INCLUDEPATH += $$PWD/../appointy/src/appointment
INCLUDEPATH += $$PWD/../appointy/src/service
INCLUDEPATH += $$PWD/../appointy/ext/json/nlohmann/include/nlohmann
INCLUDEPATH += $$PWD/../appointy/ext/json/nlohmann/include/
DEPENDPATH += $$PWD/../appointy/src
