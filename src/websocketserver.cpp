#include "websocketserver.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QHostAddress>
#include "cmd_handlers/create_cmd_handlers.h"
#include <QProcess>
#include <QFile>
// QT_USE_NAMESPACE

// ---------------------------------------------------------------------

WebSocketServer::WebSocketServer(quint16 port, bool debug, QObject *parent) : QObject(parent) {
	m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Rasp Robot Daemon"), QWebSocketServer::NonSecureMode, this);
	m_debug = debug;

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "Rasp Robot Daemon listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
        create_cmd_handlers(m_mapCmdHandlers);
    }

	mPinA1 = 23;
	mPinA2 = 18;
	mPinB1 = 14;
	mPinB2 = 15;

	unexportPin(mPinA1);
	unexportPin(mPinA2);
	unexportPin(mPinB1);
	unexportPin(mPinB2);

	exportPin(mPinA1);
	exportPin(mPinA2);
	exportPin(mPinB1);
	exportPin(mPinB2);

	directionOutPin(mPinA1);
	directionOutPin(mPinA2);
	directionOutPin(mPinB1);
	directionOutPin(mPinB2);
}

// ---------------------------------------------------------------------

WebSocketServer::~WebSocketServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

// ---------------------------------------------------------------------

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
	
	if (m_debug)
        qDebug() << "NewConnection " << pSocket->peerAddress().toString() << " " << pSocket->peerPort();
        
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
    
    QJsonObject obj;
    obj["msg"] = "info";
	obj["version"] = "v201703";
	obj["firmware"] = 1;
	obj["name"] = "Infrared eye";
	// TODO 

	QJsonObject capabilities;
	capabilities["turnleft"] = true;
	capabilities["turnright"] = true;
	capabilities["forward"] = true;
	capabilities["backward"] = true;
	capabilities["autocontrol"] = true;
	capabilities["light"] = false;
	obj["capabilities"] = capabilities;
	
	this->sendMessage(pSocket, obj);
}

// ---------------------------------------------------------------------

void WebSocketServer::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
	QJsonObject jsonData = doc.object();
	QString cmd = "";
	if(jsonData.contains("cmd")){
		cmd = jsonData["cmd"].toString();
	}else{
		QJsonObject obj;
		obj["error"] = "Invalid command format";
		this->sendMessage(pClient, obj);
	}
    if (m_debug){
		if(cmd != "takevideo0")
			qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] <<< " << message;
	}

	if(m_mapCmdHandlers.contains(cmd)){
		m_mapCmdHandlers[cmd]->handle(pClient, this, jsonData);
	}else{
		qDebug() << "Unknown command: " << cmd;
		QJsonObject obj;
		obj["error"] = "Unknown command";
		this->sendMessage(pClient, obj);
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessage(QWebSocket *pClient, QJsonObject obj){
	 if (pClient) {
		QJsonDocument doc(obj);
		QString message = doc.toJson(QJsonDocument::Compact);
		qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] >>> " << message;
        pClient->sendTextMessage(message);
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::sendMessage(QWebSocket *pClient, const QByteArray &data){
	 if (pClient) {
		// qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] >>> send binary data " << data.size();
        pClient->sendBinaryMessage(data);
    }
}

// ---------------------------------------------------------------------

void WebSocketServer::turnleft(){

	setPinValue(mPinA1, 1);
	setPinValue(mPinA2, 0);
	setPinValue(mPinB1, 0);
	setPinValue(mPinB2, 1);
}

// ---------------------------------------------------------------------

void WebSocketServer::turnright(){
	setPinValue(mPinA1, 1);
	setPinValue(mPinA2, 0);
	setPinValue(mPinB1, 1);
	setPinValue(mPinB2, 0);
}

// ---------------------------------------------------------------------

void WebSocketServer::forward(){
	setPinValue(mPinA1, 1);
	setPinValue(mPinA2, 0);
	setPinValue(mPinB1, 0);
	setPinValue(mPinB2, 1);
}

// ---------------------------------------------------------------------

void WebSocketServer::backward(){
	setPinValue(mPinA1, 0);
	setPinValue(mPinA2, 1);
	setPinValue(mPinB1, 1);
	setPinValue(mPinB2, 0);
}

// ---------------------------------------------------------------------

void WebSocketServer::stop(){
	setPinValue(mPinA1, 0);
	setPinValue(mPinA2, 0);
	setPinValue(mPinB1, 0);
	setPinValue(mPinB2, 0);
}

// ---------------------------------------------------------------------

void WebSocketServer::unexportPin(int pin){
	QFile file("/sys/class/gpio/unexport");
	if (file.open(QIODevice::ReadWrite)){
		QTextStream stream( &file );
		stream << QString::number(pin) << endl;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::exportPin(int pin){
	QFile file("/sys/class/gpio/export");
	if (file.open(QIODevice::ReadWrite)){
		QTextStream stream( &file );
		stream << QString::number(pin) << endl;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::setPinValue(int pin, int value){
	QFile file("/sys/class/gpio/gpio" + QString::number(pin) + "/value");
	if (file.open(QIODevice::ReadWrite)){
		QTextStream stream( &file );
		stream << QString::number(value) << endl;
		qDebug() << "Set value " << value << " to " << pin;
	}
}

// ---------------------------------------------------------------------


void WebSocketServer::directionOutPin(int pin){
	QFile file("/sys/class/gpio/gpio" + QString::number(pin) + "/direction");
	if (file.open(QIODevice::ReadWrite)){
		QTextStream stream( &file );
		stream << "out" << endl;
	}
}

// ---------------------------------------------------------------------
