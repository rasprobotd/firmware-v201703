#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMap>
#include "interfaces/icmdhandler.h"
#include "interfaces/iwebsocketserver.h"

// QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
// QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject, public IWebSocketServer {
	
	private:
		Q_OBJECT
	public:
		explicit WebSocketServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
		~WebSocketServer();

		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj);
		virtual void sendMessage(QWebSocket *pClient, const QByteArray &data);
		virtual void turnleft();
		virtual void turnright();
		virtual void forward();
		virtual void backward();
		virtual void stop();
				
	Q_SIGNALS:
		void closed();

	private Q_SLOTS:
		void onNewConnection();
		void processTextMessage(QString message);
		void processBinaryMessage(QByteArray message);
		void socketDisconnected();

	private:
		void unexportPin(int pin);
		void exportPin(int pin);
		void setPinValue(int pin, int value);
		void directionOutPin(int pin);
			
		QWebSocketServer *m_pWebSocketServer;
		QList<QWebSocket *> m_clients;
		QMap<QString, ICmdHandler *> m_mapCmdHandlers;
		bool m_debug;
		int mPinA1;
		int mPinA2;
		int mPinB1;
		int mPinB2;
};

#endif //WEBSOCKETSERVER_H
