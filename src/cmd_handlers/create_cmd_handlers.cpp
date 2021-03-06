#include "create_cmd_handlers.h"

#include "cmd_backward_handler.h"
#include "cmd_check_lets_handler.h"
#include "cmd_forward_handler.h"
#include "cmd_start_auto_handler.h"
#include "cmd_stop_auto_handler.h"
#include "cmd_stop_handler.h"
#include "cmd_takevideo0_handler.h"
#include "cmd_turnleft_handler.h"
#include "cmd_turnright_handler.h"


void create_cmd_handlers(QMap<QString, ICmdHandler *> &pHandlers){
	QVector<ICmdHandler *> v;
	v.push_back(new CmdStartAutoHandler());
	v.push_back(new CmdStopAutoHandler());
	v.push_back(new CmdStopHandler());
	v.push_back(new CmdForwardHandler());
	v.push_back(new CmdBackwardHandler());
	v.push_back(new CmdTurnleftHandler());
	v.push_back(new CmdTurnrightHandler());
	v.push_back(new CmdTakevideo0Handler());
	v.push_back(new CmdCheckLetsHandler());

	for(int i = 0; i < v.size(); i++){
		QString cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
			qDebug() << "[WARNING] cmd_handler for command " << cmd << " - already registered and will be skipped";	
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
