QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../service_creator/util.cc \
    appointment_request_widget.cc \
    appointment_widget.cc \
    id_check_box.cc \
    id_radio_button.cc \
    main.cc \
    main_window.cc \
    question_display_widget.cc \
    request_widget_base.cc \
    service_configurator_widget.cc \
    service_selector_window.cc

HEADERS += \
    ../service_creator/util.h \
    appointment_request_widget.h \
    appointment_widget.h \
    id_check_box.h \
    id_radio_button.h \
    main_window.h \
    question_display_widget.h \
    request_widget_base.h \
    service_configurator_widget.h \
    service_selector_window.h

FORMS += \
    appointment_request_widget.ui \
    appointment_widget.ui \
    main_window.ui \
    question_display_widget.ui \
    service_configurator_widget.ui \
    service_selector_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../appointy/build/appointy_base/Debug/bin/ -lappointy_base
unix:!macx: LIBS += -L$$PWD/../appointy/build/appointy/Debug/bin/ -lappointy

INCLUDEPATH += $$PWD/../appointy/appointy_base/include/util
INCLUDEPATH += $$PWD/../appointy/appointy_base/include/appointment
INCLUDEPATH += $$PWD/../appointy/appointy_base/include/service
INCLUDEPATH += $$PWD/../appointy/ext/json/include
INCLUDEPATH += $$PWD/../appointy/ext/json/include/nlohmann
INCLUDEPATH += $$PWD/../appointy/appointy/include/
DEPENDPATH += $$PWD/../appointy/appointy_base/
DEPENDPATH += $$PWD/../appointy/appointy/
