#include "cmd_forward_handler.h"

QString CmdForwardHandler::cmd(){
	return "forward";
}

void CmdForwardHandler::handle(QWebSocket */*pClient*/, IWebSocketServer *pWebSocketServer, QJsonObject /*obj*/){
	// QJsonObject jsonData;
	// jsonData["cmd"] = QJsonValue(cmd());
	// jsonData["result"] = "OK";
	// pWebSocketServer->sendMessage(pClient, jsonData);
	pWebSocketServer->forward();
}
