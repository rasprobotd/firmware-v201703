#ifndef INTERFACES_IWEBSOCKETSERVER_H
#define INTERFACES_IWEBSOCKETSERVER_H

#include <QJsonObject>
#include <QWebSocket>
#include "icmdhandler.h"

class IWebSocketServer {
	public:
		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj) = 0;
		virtual void sendMessage(QWebSocket *pClient, const QByteArray &data) = 0;
		virtual void turnleft() = 0;
		virtual void turnright() = 0;
		virtual void forward() = 0;
		virtual void backward() = 0;
		virtual void stop() = 0;
};

#endif // INTERFACES_IWEBSOCKETSERVER_H
