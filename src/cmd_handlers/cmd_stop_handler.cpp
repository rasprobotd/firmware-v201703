#include "cmd_stop_handler.h"

QString CmdStopHandler::cmd(){
	return "stop";
}

void CmdStopHandler::handle(QWebSocket */*pClient*/, IWebSocketServer *pWebSocketServer, QJsonObject /*obj*/){
	// QJsonObject jsonData;
	// jsonData["cmd"] = QJsonValue(cmd());
	// jsonData["result"] = "OK";
	// pWebSocketServer->sendMessage(pClient, jsonData);
	pWebSocketServer->stop();
}
