#include "cmd_stop_auto_handler.h"

QString CmdStopAutoHandler::cmd(){
	return "stop_auto";
}

void CmdStopAutoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject /*obj*/){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = "OK";
	pWebSocketServer->sendMessage(pClient, jsonData);
	pWebSocketServer->stop_auto();
}
