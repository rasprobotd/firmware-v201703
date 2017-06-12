#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <QtCore>
#include <QFile>
#include <QString>
#include "websocketserver.h"

int main(int argc, char** argv) {
	QCoreApplication app(argc, argv);
	
	QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("SopovRobotics");
    parser.addHelpOption();
    parser.process(a);

    WebSocketServer *server = new WebSocketServer();
    QObject::connect(server, &WebSocketServer::closed, &a, &QCoreApplication::quit);
    
	return app.exec();
}
