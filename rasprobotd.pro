TEMPLATE = app
TARGET = rasprobotd

QT += core network websockets
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle
OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/
CONFIG += c++11 c++14

SOURCES += \
	src/main.cpp \
	src/websocketserver.cpp \
	src/cmd_handlers/cmd_forward_handler.cpp \
	src/cmd_handlers/cmd_backward_handler.cpp \
	src/cmd_handlers/cmd_turnleft_handler.cpp \
	src/cmd_handlers/cmd_turnright_handler.cpp \
	src/cmd_handlers/cmd_stop_handler.cpp \
	src/cmd_handlers/create_cmd_handlers.cpp \
	src/cmd_handlers/cmd_takevideo0_handler.cpp \

HEADERS += \
	src/interfaces/iwebsocketserver.h \
	src/interfaces/icmdhandler.h \
	src/websocketserver.h \
	src/cmd_handlers/create_cmd_handlers.h \
	src/cmd_handlers/cmd_forward_handler.h \
	src/cmd_handlers/cmd_backward_handler.h \
	src/cmd_handlers/cmd_turnleft_handler.h \
	src/cmd_handlers/cmd_turnright_handler.h \
	src/cmd_handlers/cmd_stop_handler.h \
	src/cmd_handlers/cmd_takevideo0_handler.h \
