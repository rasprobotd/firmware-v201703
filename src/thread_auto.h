#ifndef THREAD_AUTO_H
#define THREAD_AUTO_H

#include <QString>
#include <QThread>
#include <QScriptEngine>
#include <iwebsocketserver.h>

class ThreadAuto : public QThread {
	Q_OBJECT
public:
	ThreadAuto(IWebSocketServer *pSopovRoboticsEngine);
	~ThreadAuto();

	IWebSocketServer *getSopovRoboticsEngine();
	
protected:
	
	void run();
	
private:


	IWebSocketServer *m_pSopovRoboticsEngine;
};

#endif // THREAD_AUTO_H
