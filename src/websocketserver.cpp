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

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent) {
	m_bServerStarted = false;

	m_pSett = new Sett();
	if(!m_pSett->isLoaded()){
		qDebug() << "[INFO] Settings not loaded";
		return;
	}

	m_pWebSocketServer = new QWebSocketServer(QStringLiteral("SopovRobotics"), QWebSocketServer::NonSecureMode, this);

    if (m_pWebSocketServer->listen(QHostAddress::Any, 7528)) {
		qDebug() << "SopovRobotics listening on port 7528";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
        create_cmd_handlers(m_mapCmdHandlers);
        m_bServerStarted = true;
    }

	openDriverPins();
}

// ---------------------------------------------------------------------

WebSocketServer::~WebSocketServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

// ---------------------------------------------------------------------

bool WebSocketServer::isServerStarted(){
	return m_bServerStarted;
}

// ---------------------------------------------------------------------

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
	
	qDebug() << "NewConnection " << pSocket->peerAddress().toString() << " " << pSocket->peerPort();
        
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
    
    QJsonObject obj;
    obj["msg"] = "info";
	obj["version"] = "v201703";
	obj["firmware"] = 2;
	obj["name"] = "Infrared eye";
	// TODO 

	QJsonObject capabilities;
	capabilities["turnleft"] = true;
	capabilities["turnright"] = true;
	capabilities["forward"] = true;
	capabilities["backward"] = true;
	capabilities["autocontrol"] = true;
	capabilities["light"] = false;
	capabilities["check_lets"] = false;
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
    
	if(cmd != "takevideo0")
		qDebug() << QDateTime::currentDateTimeUtc().toString() << " [WS] <<< " << message;
	

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
    // send comeback
    /*if (pClient) {
        pClient->sendBinaryMessage(message);
    }*/
}

// ---------------------------------------------------------------------

void WebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
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

void WebSocketServer::openDriverPins(){
	mPinA1 = m_pSett->drivers_PIN_A1();
	mPinA2 = m_pSett->drivers_PIN_A2();
	mPinB1 = m_pSett->drivers_PIN_B1();
	mPinB2 = m_pSett->drivers_PIN_B2();

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

void WebSocketServer::check_lets(){
	int pin_let_in = m_pSett->let_PIN_IN();
	int pin_let_servo = m_pSett->let_PIN_IN();
	int let0_pos = m_pSett->let0_POS_SERVO();
	int let1_pos = m_pSett->let1_POS_SERVO();
	int let2_pos = m_pSett->let2_POS_SERVO();
	
	unexportPin(pin_let_in);
 	exportPin(pin_let_in);
	directionInPin(pin_let_in);

	unexportPin(pin_let_servo);
 	exportPin(pin_let_servo);
	directionOutPin(pin_let_servo);

	pwmPin(pin_let_servo, let0_pos);
	m_bLet0 = getPinValue(pin_let_in) == 0;
	pwmPin(pin_let_servo, let1_pos);
	m_bLet1 = getPinValue(pin_let_in) == 0;
	pwmPin(pin_let_servo, let2_pos);
	m_bLet2 = getPinValue(pin_let_in) == 0;

	unexportPin(pin_let_in);
	unexportPin(pin_let_servo);
	
}

// ---------------------------------------------------------------------

bool WebSocketServer::let0(){
	return m_bLet0;
}

// ---------------------------------------------------------------------

bool WebSocketServer::let1(){
	return m_bLet1;
}

// ---------------------------------------------------------------------

bool WebSocketServer::let2(){
	return m_bLet2;
}

// ---------------------------------------------------------------------

void WebSocketServer::unexportPin(int pin){
	QFile file("/sys/class/gpio/unexport");
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << QString::number(pin) << endl;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::exportPin(int pin){
	QFile file("/sys/class/gpio/export");
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << QString::number(pin) << endl;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::setPinValue(int pin, int value){
	QFile file("/sys/class/gpio/gpio" + QString::number(pin) + "/value");
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << QString::number(value) << endl;
		qDebug() << "Set value " << value << " to " << pin;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::directionOutPin(int pin){
	QFile file("/sys/class/gpio/gpio" + QString::number(pin) + "/direction");
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << "out" << endl;
	}
}

// ---------------------------------------------------------------------

int WebSocketServer::getPinValue(int pin){
	int value = -1;
	QFile file("/sys/class/gpio/gpio" + QString::number(pin) + "/value");
	if (file.open(QIODevice::ReadWrite)){
		QTextStream stream( &file );
		stream >> value;
	}
	return value;
}

// ---------------------------------------------------------------------

void WebSocketServer::directionInPin(int pin){
	QFile file("/sys/class/gpio/gpio" + QString::number(pin) + "/direction");
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << "in" << endl;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::pwmPin(int pin, qint64 width_signal_usec){
	QFile file("/sys/class/gpio/gpio" + QString::number(pin) + "/value");
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		int counter = 0;
		QElapsedTimer timer;
		qint64 elapsed;
		while(true){
			timer.restart();
			stream << QString::number(1) << endl;
			elapsed = timer.nsecsElapsed()/1000;
			QThread::usleep(width_signal_usec);
			stream << QString::number(0) << endl;
			elapsed = timer.nsecsElapsed()/1000;
			QThread::usleep(20000 - elapsed);
			counter++;
			if(counter > 21){
				break;
				return;
			}
		}
	}
}

// ---------------------------------------------------------------------
