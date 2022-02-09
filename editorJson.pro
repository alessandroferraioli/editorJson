QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
.
DEFINES += QT_DEPRECATED_WARNINGS

##### Configuration
DEFINES += KEY_NOT_EDITABLE
DEFINES += USE_RFTE_HOME_DIR

##### Testing
#DEFINES += PRINT_DEBUG
#DEFINES += CLEAR_BTN_TEST





SOURCES += \
    main.cpp \
    editorjson.cpp

HEADERS += \
    editorjson.h

FORMS += \
    editorjson.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
