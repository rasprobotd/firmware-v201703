TEMPLATE = app
TARGET = sopovrobotics

QT += core network websockets script
QT -= gui


CONFIG   += console
CONFIG   -= app_bundle
OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/
CONFIG += c++11 c++14

INCLUDEPATH += src/interfaces

RESOURCES = sopovrobotics.qrc

SOURCES += \
	src/main.cpp \
	src/websocketserver.cpp \
	src/sett.cpp \
	src/thread_auto.cpp \
	src/cmd_handlers/cmd_forward_handler.cpp \
	src/cmd_handlers/cmd_backward_handler.cpp \
	src/cmd_handlers/cmd_turnleft_handler.cpp \
	src/cmd_handlers/cmd_turnright_handler.cpp \
	src/cmd_handlers/cmd_check_lets_handler.cpp \
	src/cmd_handlers/cmd_stop_handler.cpp \
	src/cmd_handlers/cmd_stop_auto_handler.cpp \
	src/cmd_handlers/cmd_start_auto_handler.cpp \
	src/cmd_handlers/create_cmd_handlers.cpp \
	src/cmd_handlers/cmd_takevideo0_handler.cpp \

HEADERS += \
	src/interfaces/iwebsocketserver.h \
	src/interfaces/icmdhandler.h \
	src/websocketserver.h \
	src/sett.h \
	src/thread_auto.h \
	src/cmd_handlers/create_cmd_handlers.h \
	src/cmd_handlers/cmd_forward_handler.h \
	src/cmd_handlers/cmd_backward_handler.h \
	src/cmd_handlers/cmd_turnleft_handler.h \
	src/cmd_handlers/cmd_turnright_handler.h \
	src/cmd_handlers/cmd_check_lets_handler.h \
	src/cmd_handlers/cmd_stop_handler.h \
	src/cmd_handlers/cmd_stop_auto_handler.h \
	src/cmd_handlers/cmd_start_auto_handler.h \
	src/cmd_handlers/cmd_takevideo0_handler.h \


