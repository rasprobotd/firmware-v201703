#ifndef CMD_TURNRIGHT_HANDLER_H
#define CMD_TURNRIGHT_HANDLER_H

#include "../interfaces/icmdhandler.h"
#include "../interfaces/iwebsocketserver.h"

#include <QString>
#include <QVariant>

class CmdTurnrightHandler : public ICmdHandler {
	
	public:
		virtual QString cmd();
		virtual void handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj);
};

#endif // CMD_TURNRIGHT_HANDLER_H
