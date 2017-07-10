#include "thread_auto.h"
#include <QFile>
#include <QDir>
#include <QtCore/QDebug>
#include <QCoreApplication>


ThreadAuto::ThreadAuto(IWebSocketServer *pSopovRoboticsEngine)
  : QThread(0) {
	  m_pSopovRoboticsEngine = pSopovRoboticsEngine;
}

// ---------------------------------------------------------------------

ThreadAuto::~ThreadAuto(){
}

// ---------------------------------------------------------------------

QScriptValue check_lets_(QScriptContext */*context*/, QScriptEngine *engine) {
	qDebug() << "check_lets";
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->check_lets();
    return engine->undefinedValue();
}

// ---------------------------------------------------------------------

QScriptValue let0_(QScriptContext */*context*/, QScriptEngine *engine) {
	qDebug() << "let0";
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	bool res = ((ThreadAuto *)pThread)->getSopovRoboticsEngine()->let0();
    return engine->toScriptValue(res);
}

// ---------------------------------------------------------------------

QScriptValue let1_(QScriptContext */*context*/, QScriptEngine *engine) {
	qDebug() << "let1";
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	bool res = ((ThreadAuto *)pThread)->getSopovRoboticsEngine()->let1();
    return engine->toScriptValue(res);
}

// ---------------------------------------------------------------------

QScriptValue let2_(QScriptContext */*context*/, QScriptEngine *engine) {
	qDebug() << "let2";
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	bool res = ((ThreadAuto *)pThread)->getSopovRoboticsEngine()->let2();
    return engine->toScriptValue(res);
}

// ---------------------------------------------------------------------

QScriptValue forward_(QScriptContext *context, QScriptEngine *engine) {
	int ms = context->argument(0).toInt32();
	qDebug() << "forward " << ms;
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->forward();
	QThread::msleep(ms);
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->stop();
    return engine->undefinedValue();
}

// ---------------------------------------------------------------------

QScriptValue backward_(QScriptContext *context, QScriptEngine *engine) {
	int ms = context->argument(0).toInt32();
	qDebug() << "backward " << ms;
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->backward();
	QThread::msleep(ms);
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->stop();
    return engine->undefinedValue();
}

// ---------------------------------------------------------------------

QScriptValue turnleft_(QScriptContext *context, QScriptEngine *engine) {
	int ms = context->argument(0).toInt32();
	qDebug() << "turnleft " << ms;
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->turnleft();
	QThread::msleep(ms);
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->stop();
    return engine->undefinedValue();
}

// ---------------------------------------------------------------------

QScriptValue turnright_(QScriptContext *context, QScriptEngine *engine) {
	int ms = context->argument(0).toInt32();
	qDebug() << "turnright " << ms;
	QObject *pThread = engine->globalObject().property("thread").toQObject();
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->turnright();
	QThread::msleep(ms);
	((ThreadAuto *)pThread)->getSopovRoboticsEngine()->stop();
    return engine->undefinedValue();
}

// ---------------------------------------------------------------------

void ThreadAuto::run(){
	QFile file(":/res/autocontrol.js");
	if (!file.open(QIODevice::ReadOnly)){
		return;
	}
	QTextStream in( &file );
	QString contents;
	contents.append(in.readAll());
	qDebug() << contents;
	file.close();
	 
	QScriptEngine engine;

	engine.globalObject().setProperty("thread", engine.newQObject((QObject *)this));

	engine.globalObject().setProperty("check_lets", engine.newFunction(check_lets_));
	engine.globalObject().setProperty("let0", engine.newFunction(let0_));
	engine.globalObject().setProperty("let1", engine.newFunction(let1_));
	engine.globalObject().setProperty("let2", engine.newFunction(let2_));
	
	engine.globalObject().setProperty("forward", engine.newFunction(forward_));
	engine.globalObject().setProperty("backward", engine.newFunction(backward_));
	engine.globalObject().setProperty("turnleft", engine.newFunction(turnleft_));
	engine.globalObject().setProperty("turnright", engine.newFunction(turnright_));
	
	// engine.globalObject().setProperty("console.log", engine.newFunction(let2_, turnleftProto));
    
	while(1){
		qDebug() << "evaluate";
		QScriptValue result = engine.evaluate(contents, "autocontrol.js");
		
		if(!engine.hasUncaughtException()){
			qDebug() << engine.uncaughtException().toString();
		}
		
		engine.collectGarbage();
		QThread::usleep(200000);
	}
}

// ---------------------------------------------------------------------

IWebSocketServer *ThreadAuto::getSopovRoboticsEngine(){
	return m_pSopovRoboticsEngine;
}

// ---------------------------------------------------------------------
