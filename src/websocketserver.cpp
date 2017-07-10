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
	m_pThreadAuto = NULL;
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
	int pin_A1 = m_pSett->drivers_PIN_A1();
	int pin_A2 = m_pSett->drivers_PIN_A2();
	int pin_B1 = m_pSett->drivers_PIN_B1();
	int pin_B2 = m_pSett->drivers_PIN_B2();

	unexportPin(pin_A1);
	unexportPin(pin_A2);
	unexportPin(pin_B1);
	unexportPin(pin_B2);

	exportPin(pin_A1);
	exportPin(pin_A2);
	exportPin(pin_B1);
	exportPin(pin_B2);

	directionOutPin(pin_A1);
	directionOutPin(pin_A2);
	directionOutPin(pin_B1);
	directionOutPin(pin_B2);
}

// ---------------------------------------------------------------------

void WebSocketServer::turnleft(){
	int pin_A1 = m_pSett->drivers_PIN_A1();
	int pin_A2 = m_pSett->drivers_PIN_A2();
	int pin_B1 = m_pSett->drivers_PIN_B1();
	int pin_B2 = m_pSett->drivers_PIN_B2();
	
	setPinValue(pin_A1, m_pSett->drivers_turnleft_A1());
	setPinValue(pin_A2, m_pSett->drivers_turnleft_A2());
	setPinValue(pin_B1, m_pSett->drivers_turnleft_B1());
	setPinValue(pin_B2, m_pSett->drivers_turnleft_B2());
}

// ---------------------------------------------------------------------

void WebSocketServer::turnright(){
	int pin_A1 = m_pSett->drivers_PIN_A1();
	int pin_A2 = m_pSett->drivers_PIN_A2();
	int pin_B1 = m_pSett->drivers_PIN_B1();
	int pin_B2 = m_pSett->drivers_PIN_B2();
	
	setPinValue(pin_A1, m_pSett->drivers_turnright_A1());
	setPinValue(pin_A2, m_pSett->drivers_turnright_A2());
	setPinValue(pin_B1, m_pSett->drivers_turnright_B1());
	setPinValue(pin_B2, m_pSett->drivers_turnright_B2());
}

// ---------------------------------------------------------------------

void WebSocketServer::forward(){
	int pin_A1 = m_pSett->drivers_PIN_A1();
	int pin_A2 = m_pSett->drivers_PIN_A2();
	int pin_B1 = m_pSett->drivers_PIN_B1();
	int pin_B2 = m_pSett->drivers_PIN_B2();
	
	setPinValue(pin_A1, m_pSett->drivers_forward_A1());
	setPinValue(pin_A2, m_pSett->drivers_forward_A2());
	setPinValue(pin_B1, m_pSett->drivers_forward_B1());
	setPinValue(pin_B2, m_pSett->drivers_forward_B2());
}

// ---------------------------------------------------------------------

void WebSocketServer::backward(){
	int pin_A1 = m_pSett->drivers_PIN_A1();
	int pin_A2 = m_pSett->drivers_PIN_A2();
	int pin_B1 = m_pSett->drivers_PIN_B1();
	int pin_B2 = m_pSett->drivers_PIN_B2();
	
	setPinValue(pin_A1, m_pSett->drivers_backward_A1());
	setPinValue(pin_A2, m_pSett->drivers_backward_A2());
	setPinValue(pin_B1, m_pSett->drivers_backward_B1());
	setPinValue(pin_B2, m_pSett->drivers_backward_B2());
}

// ---------------------------------------------------------------------

void WebSocketServer::stop(){
	int pin_A1 = m_pSett->drivers_PIN_A1();
	int pin_A2 = m_pSett->drivers_PIN_A2();
	int pin_B1 = m_pSett->drivers_PIN_B1();
	int pin_B2 = m_pSett->drivers_PIN_B2();
	
	setPinValue(pin_A1, 0);
	setPinValue(pin_A2, 0);
	setPinValue(pin_B1, 0);
	setPinValue(pin_B2, 0);
}

// ---------------------------------------------------------------------

void WebSocketServer::start_auto(){
	stop_auto();
	m_pThreadAuto = new ThreadAuto(this);
	m_pThreadAuto->start();
}

// ---------------------------------------------------------------------

void WebSocketServer::stop_auto(){
	if(m_pThreadAuto != NULL){
		m_pThreadAuto->terminate();
		m_pThreadAuto = NULL;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::check_lets(){
	int pin_let_in = m_pSett->let_PIN_IN();
	int pin_let_in_enable = m_pSett->let_PIN_ENABLE();
	int pin_let_servo = m_pSett->let_PIN_SERVO();
	int let0_pos = m_pSett->let0_POS_SERVO();
	int let1_pos = m_pSett->let1_POS_SERVO();
	int let2_pos = m_pSett->let2_POS_SERVO();
	
	unexportPin(pin_let_in);
 	exportPin(pin_let_in);
	directionInPin(pin_let_in);

	unexportPin(pin_let_in_enable);
 	exportPin(pin_let_in_enable);
	directionOutPin(pin_let_in_enable);
	
	unexportPin(pin_let_servo);
 	exportPin(pin_let_servo);
	directionOutPin(pin_let_servo);

	setPinValue(pin_let_in_enable,1);
	pwmPin(pin_let_servo, let0_pos);
	m_bLet0 = getPinValue(pin_let_in) == 0;
	setPinValue(pin_let_in_enable,0);
	
	QThread::usleep(200000);
	
	setPinValue(pin_let_in_enable,1);
	pwmPin(pin_let_servo, let1_pos);
	m_bLet1 = getPinValue(pin_let_in) == 0;
	setPinValue(pin_let_in_enable,0);
	
	QThread::usleep(200000);
	
	setPinValue(pin_let_in_enable,1);
	pwmPin(pin_let_servo, let2_pos);
	m_bLet2 = getPinValue(pin_let_in) == 0;
	setPinValue(pin_let_in_enable,0);

	// moto middle position
	pwmPin(pin_let_servo, let1_pos);

	unexportPin(pin_let_in);
	unexportPin(pin_let_in_enable);
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
	QString gpio_path = "/sys/class/gpio/unexport";
	QFile file(gpio_path);
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << QString::number(pin) << endl;
		qDebug() << "Unexport pin " << pin;
	}else{
		qDebug() << "Could not open " << gpio_path << " for pin " << pin;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::exportPin(int pin){
	QString gpio_path = "/sys/class/gpio/export";
	QFile file(gpio_path);
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << QString::number(pin) << endl;
		qDebug() << "Export pin " << pin;
	}else{
		qDebug() << "Could not open " << gpio_path << " for pin " << pin;
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::setPinValue(int pin, int value){
	QString gpio_path = "/sys/class/gpio/gpio" + QString::number(pin) + "/value";
	QFile file(gpio_path);
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << QString::number(value) << endl;
		qDebug() << "Set pin " << pin << " value " << value;
	}else{
		qDebug() << "Could not open " << gpio_path << " for set value";
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::directionOutPin(int pin){
	QString gpio_path = "/sys/class/gpio/gpio" + QString::number(pin) + "/direction";
	QFile file(gpio_path);
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << "out" << endl;
		qDebug() << "Direction out pin " << pin;
	}else{
		qDebug() << "Could not open " << gpio_path << " for out";
	}
}

// ---------------------------------------------------------------------

int WebSocketServer::getPinValue(int pin){
	QString gpio_path = "/sys/class/gpio/gpio" + QString::number(pin) + "/value";
	int value = -1;
	QFile file(gpio_path);
	if (file.open(QIODevice::ReadOnly)){
		QTextStream stream( &file );
		stream >> value;
		qDebug() << "Get pin " << pin << " value " << value;
	}else{
		qDebug() << "Could not open " << gpio_path << " for get value";
	}
	return value;
}

// ---------------------------------------------------------------------

void WebSocketServer::directionInPin(int pin){
	QString gpio_path = "/sys/class/gpio/gpio" + QString::number(pin) + "/direction";
	QFile file(gpio_path);
	if (file.open(QIODevice::WriteOnly)){
		QTextStream stream( &file );
		stream << "in" << endl;
		qDebug() << "Direction in pin " << pin;
	}else{
		qDebug() << "Could not open " << gpio_path << " for in";
	}
}

// ---------------------------------------------------------------------

void WebSocketServer::pwmPin(int pin, qint64 width_signal_usec){
	QString gpio_path = "/sys/class/gpio/gpio" + QString::number(pin) + "/value";
	QFile file(gpio_path);
	if (file.open(QIODevice::WriteOnly)){
		qDebug() << "PWM on pin " << pin << " started for " << width_signal_usec;
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
		qDebug() << "PWM on pin " << pin << " finished";
	}else{
		qDebug() << "Could not open " << gpio_path << " for pwm";
	}
}

// ---------------------------------------------------------------------
