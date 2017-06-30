#include "cmd_check_lets_handler.h"

QString CmdCheckLetsHandler::cmd(){
	return "check_lets";
}

void CmdCheckLetsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject /*obj*/){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = "OK";
	
	pWebSocketServer->check_lets();
	jsonData["let0"] = pWebSocketServer->let0();
	jsonData["let1"] = pWebSocketServer->let1();
	jsonData["let2"] = pWebSocketServer->let2();
	
	pWebSocketServer->sendMessage(pClient, jsonData);
}
