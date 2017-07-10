#include "cmd_start_auto_handler.h"

QString CmdStartAutoHandler::cmd(){
	return "start_auto";
}

void CmdStartAutoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject /*obj*/){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = "OK";
	pWebSocketServer->sendMessage(pClient, jsonData);
	pWebSocketServer->start_auto();
}
