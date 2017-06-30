#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMap>
#include <QElapsedTimer>
#include <QThread>
#include "interfaces/icmdhandler.h"
#include "interfaces/iwebsocketserver.h"
#include "sett.h"

// QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
// QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject, public IWebSocketServer {
	
	private:
		Q_OBJECT
	public:
		explicit WebSocketServer(QObject *parent = Q_NULLPTR);
		~WebSocketServer();

		virtual void sendMessage(QWebSocket *pClient, QJsonObject obj);
		virtual void sendMessage(QWebSocket *pClient, const QByteArray &data);
		virtual void turnleft();
		virtual void turnright();
		virtual void forward();
		virtual void backward();
		virtual void stop();
		virtual void check_lets();
		virtual bool let0();
		virtual bool let1();
		virtual bool let2();

		bool isServerStarted();

	Q_SIGNALS:
		void closed();

	private Q_SLOTS:
		void onNewConnection();
		void processTextMessage(QString message);
		void processBinaryMessage(QByteArray message);
		void socketDisconnected();

	private:
		void openDriverPins();
	
		void unexportPin(int pin);
		void exportPin(int pin);
		void setPinValue(int pin, int value);
		void directionOutPin(int pin);
		int getPinValue(int pin);
		void directionInPin(int pin);

		void pwmPin(int pin, qint64 width_signal_usec);

		QWebSocketServer *m_pWebSocketServer;
		QList<QWebSocket *> m_clients;
		QMap<QString, ICmdHandler *> m_mapCmdHandlers;
		int mPinA1;
		int mPinA2;
		int mPinB1;
		int mPinB2;
		
		bool m_bLet0;
		bool m_bLet1;
		bool m_bLet2;
		
		bool m_bServerStarted;
		
		Sett *m_pSett;
};

#endif //WEBSOCKETSERVER_H
