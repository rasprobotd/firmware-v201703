#ifndef INTERFACES_ISOPOV_ROBOTICS_ENGINE_H
#define INTERFACES_ISOPOV_ROBOTICS_ENGINE_H

#include <QJsonObject>
#include <QWebSocket>

class ISopovRoboticsEngine {
	public:
		virtual void turnleft() = 0;
		virtual void turnright() = 0;
		virtual void forward() = 0;
		virtual void backward() = 0;
		virtual void stop() = 0;
		virtual void start_auto() = 0;
		virtual void stop_auto() = 0;
		virtual void check_lets() = 0;
		virtual bool let0() = 0;
		virtual bool let1() = 0;
		virtual bool let2() = 0;
};

#endif // INTERFACES_ISOPOV_ROBOTICS_ENGINE_H
