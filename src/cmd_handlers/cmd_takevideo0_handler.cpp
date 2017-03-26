#include "cmd_takevideo0_handler.h"
#include <QProcess>

QString CmdTakevideo0Handler::cmd(){
	return "takevideo0";
}

void CmdTakevideo0Handler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QJsonObject obj){
	// $cmd = "";
	QProcess process;
	process.setProcessChannelMode(QProcess::MergedChannels);

	// Start the process
	process.start(QString("streamer -q -c /dev/video0 -f jpeg -o /dev/stdout"),QIODevice::ReadWrite);
	
	// process.start(QString("streamer -q -c /dev/video0 -f jpeg -o 1.jpeg"),QIODevice::ReadWrite);
	
	

	// Wait for it to start
	if(!process.waitForStarted())
		return;

	// Continue reading the data until EOF reached
	QByteArray data;

	while(process.waitForReadyRead())
		data.append(process.readAll());

	pWebSocketServer->sendMessage(pClient, data);

	// Output the data
	// qDebug(data.data());
	// qDebug("Done!");

	/*QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["connectedusers"] = pWebSocketServer->getConnectedUsers();
	pWebSocketServer->sendMessage(pClient, jsonData);*/
}
