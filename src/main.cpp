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
#include "sett.h"

int main(int argc, char** argv) {
	QCoreApplication app(argc, argv);

    WebSocketServer *pServer = new WebSocketServer();
    if(!pServer->isServerStarted()){
		return -1;
	}
    
    QObject::connect(pServer, &WebSocketServer::closed, &app, &QCoreApplication::quit);
	return app.exec();
}
