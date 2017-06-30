#include "sett.h"
#include <QFile>
#include <QDir>
#include <QtCore/QDebug>
#include <QCoreApplication>

Sett::Sett(){
	m_sFilename = "/etc/sopovrobotics/sett.ini";
	m_bLoaded = false;
	qDebug() << "[INFO] Loading settings...";

	if(!QDir("/etc/sopovrobotics").exists()){
		qDebug() << "[INFO] Try mkdir /etc/sopovrobotics";
		if(!QDir("/etc").mkdir("sopovrobotics")){
			m_bLoaded = false;
			qDebug() << "[ERROR] Cloud not create dir /etc/sopovrobotics";
			return;
		}
	}
	
	// default values
	m_nDrivers_PIN_A1 = 17;
	m_nDrivers_PIN_A2 = 23;
	m_nDrivers_PIN_B1 = 22;
	m_nDrivers_PIN_B2 = 18;
	m_nLet_PIN_IN = 4;
	m_nLet_PIN_SERVO = 17;
	
	m_nDrivers_turnleft_A1 = 0;
	m_nDrivers_turnleft_A2 = 1;
	m_nDrivers_turnleft_B1 = 1;
	m_nDrivers_turnleft_B2 = 0;

	m_nDrivers_turnright_A1 = 1;
	m_nDrivers_turnright_A2 = 0;
	m_nDrivers_turnright_B1 = 0;
	m_nDrivers_turnright_B2 = 1;

	m_nDrivers_forward_A1 = 0;
	m_nDrivers_forward_A2 = 1;
	m_nDrivers_forward_B1 = 0;
	m_nDrivers_forward_B2 = 1;

	m_nDrivers_backward_A1 = 1;
	m_nDrivers_backward_A2 = 0;
	m_nDrivers_backward_B1 = 1;
	m_nDrivers_backward_B2 = 0;

	m_nLet0_POS_SERVO = 800;
	m_nLet1_POS_SERVO = 1200;
	m_nLet2_POS_SERVO = 1700;

	
	if(QFile::exists(m_sFilename)){
		if(!load()){
			m_bLoaded = false;
			qDebug() << "[INFO] Could not load settings (2)";
			return;
		}else{
			m_bLoaded = true;
			qDebug() << "[INFO] Loaded settings";
		}
	}
	
	// try save
	if(!save()){
		m_bLoaded = false;
		qDebug() << "[ERROR] Cloud not save settings to " << m_sFilename;
		return;
	}else{
		qDebug() << "[INFO] Saved settings to " << m_sFilename;
	}
};

// ---------------------------------------------------------------------

bool Sett::isLoaded(){
	return m_bLoaded;
}

// ---------------------------------------------------------------------

int Sett::readIntFromSettings(QSettings &sett, QString settName, int defaultValue){
	int nResult = defaultValue;
	if(sett.contains(settName)){
		nResult = sett.value(settName, nResult).toInt();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return nResult;
}

// ---------------------------------------------------------------------

QString Sett::readStringFromSettings(QSettings &sett, QString settName, QString defaultValue){
	QString sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return sResult;
}

// ---------------------------------------------------------------------

QStringList Sett::readStringListFromSettings(QSettings &sett, QString settName, QStringList defaultValue){
	QStringList sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toStringList();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return sResult;
}

// ---------------------------------------------------------------------

bool Sett::readBooleanFromSettings(QSettings &sett, QString settName, bool defaultValue){
	bool bResult = defaultValue;
	if(sett.contains(settName)){
		bResult = sett.value(settName, bResult).toBool();
	}else{
		qDebug() << "[WARNING] " << settName << " - not found in " << m_sFilename << "\n\t Will be used default value: " << defaultValue;
	}
	return bResult;
}

// ---------------------------------------------------------------------

bool Sett::load(){

	if(QFile::exists(m_sFilename)){
		QSettings sett(m_sFilename, QSettings::IniFormat);
		
		m_nDrivers_PIN_A1 = readIntFromSettings(sett, "DRIVERS/PIN_A1", m_nDrivers_PIN_A1);
		m_nDrivers_PIN_A2 = readIntFromSettings(sett, "DRIVERS/PIN_A2", m_nDrivers_PIN_A2);
		m_nDrivers_PIN_B1 = readIntFromSettings(sett, "DRIVERS/PIN_B1", m_nDrivers_PIN_B1);
		m_nDrivers_PIN_B2 = readIntFromSettings(sett, "DRIVERS/PIN_B2", m_nDrivers_PIN_B2);
		m_nLet_PIN_IN = readIntFromSettings(sett, "LET/PIN_IN", m_nLet_PIN_IN);
		m_nLet_PIN_SERVO = readIntFromSettings(sett, "LET/PIN_SERVO", m_nLet_PIN_SERVO);
	
		m_nDrivers_turnleft_A1 = readIntFromSettings(sett, "DRIVERS/turnleft_A1", m_nDrivers_turnleft_A1);
		m_nDrivers_turnleft_A2 = readIntFromSettings(sett, "DRIVERS/turnleft_A2", m_nDrivers_turnleft_A2);
		m_nDrivers_turnleft_B1 = readIntFromSettings(sett, "DRIVERS/turnleft_B1", m_nDrivers_turnleft_B1);
		m_nDrivers_turnleft_B2 = readIntFromSettings(sett, "DRIVERS/turnleft_B2", m_nDrivers_turnleft_B2);

		m_nDrivers_turnright_A1 = readIntFromSettings(sett, "DRIVERS/turnright_A1", m_nDrivers_turnright_A1);
		m_nDrivers_turnright_A2 = readIntFromSettings(sett, "DRIVERS/turnright_A2", m_nDrivers_turnright_A2);
		m_nDrivers_turnright_B1 = readIntFromSettings(sett, "DRIVERS/turnright_B1", m_nDrivers_turnright_B1);
		m_nDrivers_turnright_B2 = readIntFromSettings(sett, "DRIVERS/turnright_B2", m_nDrivers_turnright_B2);

		m_nDrivers_forward_A1 = readIntFromSettings(sett, "DRIVERS/forward_A1", m_nDrivers_forward_A1);
		m_nDrivers_forward_A2 = readIntFromSettings(sett, "DRIVERS/forward_A2", m_nDrivers_forward_A2);
		m_nDrivers_forward_B1 = readIntFromSettings(sett, "DRIVERS/forward_B1", m_nDrivers_forward_B1);
		m_nDrivers_forward_B2 = readIntFromSettings(sett, "DRIVERS/forward_B2", m_nDrivers_forward_B2);

		m_nDrivers_backward_A1 = readIntFromSettings(sett, "DRIVERS/backward_A1", m_nDrivers_backward_A1);
		m_nDrivers_backward_A2 = readIntFromSettings(sett, "DRIVERS/backward_A2", m_nDrivers_backward_A2);
		m_nDrivers_backward_B1 = readIntFromSettings(sett, "DRIVERS/backward_B1", m_nDrivers_backward_B1);
		m_nDrivers_backward_B2 = readIntFromSettings(sett, "DRIVERS/backward_B2", m_nDrivers_backward_B2);
		
		m_nLet0_POS_SERVO = readIntFromSettings(sett, "LET/let0_pos_servo", m_nLet0_POS_SERVO);
		m_nLet1_POS_SERVO = readIntFromSettings(sett, "LET/let1_PIN_SERVO", m_nLet1_POS_SERVO);
		m_nLet2_POS_SERVO = readIntFromSettings(sett, "LET/let2_PIN_SERVO", m_nLet2_POS_SERVO);
	
		return true;
	}
	return false;
}

// ---------------------------------------------------------------------

bool Sett::save(){
	
	QSettings sett(m_sFilename, QSettings::IniFormat);

	if(!sett.isWritable()){
		return false;
	}

	sett.setValue("DRIVERS/PIN_A1", m_nDrivers_PIN_A1);
	sett.setValue("DRIVERS/PIN_A2", m_nDrivers_PIN_A2);
	sett.setValue("DRIVERS/PIN_B1", m_nDrivers_PIN_B1);
	sett.setValue("DRIVERS/PIN_B2", m_nDrivers_PIN_B2);
	sett.setValue("LET/PIN_IN", m_nLet_PIN_IN);
	sett.setValue("LET/PIN_SERVO", m_nLet_PIN_SERVO);
	
	sett.setValue("DRIVERS/turnleft_A1", m_nDrivers_turnleft_A1);
	sett.setValue("DRIVERS/turnleft_A2", m_nDrivers_turnleft_A2);
	sett.setValue("DRIVERS/turnleft_B1", m_nDrivers_turnleft_B1);
	sett.setValue("DRIVERS/turnleft_B2", m_nDrivers_turnleft_B2);

	sett.setValue("DRIVERS/turnright_A1", m_nDrivers_turnright_A1);
	sett.setValue("DRIVERS/turnright_A2", m_nDrivers_turnright_A2);
	sett.setValue("DRIVERS/turnright_B1", m_nDrivers_turnright_B1);
	sett.setValue("DRIVERS/turnright_B2", m_nDrivers_turnright_B2);

	sett.setValue("DRIVERS/forward_A1", m_nDrivers_forward_A1);
	sett.setValue("DRIVERS/forward_A2", m_nDrivers_forward_A2);
	sett.setValue("DRIVERS/forward_B1", m_nDrivers_forward_B1);
	sett.setValue("DRIVERS/forward_B2", m_nDrivers_forward_B2);

	sett.setValue("DRIVERS/backward_A1", m_nDrivers_backward_A1);
	sett.setValue("DRIVERS/backward_A2", m_nDrivers_backward_A2);
	sett.setValue("DRIVERS/backward_B1", m_nDrivers_backward_B1);
	sett.setValue("DRIVERS/backward_B2", m_nDrivers_backward_B2);
	sett.setValue("LET/let0_pos_servo", m_nLet0_POS_SERVO);
	sett.setValue("LET/let1_PIN_SERVO", m_nLet1_POS_SERVO);
	sett.setValue("LET/let2_PIN_SERVO", m_nLet2_POS_SERVO);

	return true;
}

// ---------------------------------------------------------------------

int Sett::drivers_PIN_A1(){
	return m_nDrivers_PIN_A1;
};

// ---------------------------------------------------------------------

int Sett::drivers_PIN_A2(){
	return m_nDrivers_PIN_A2;
};

// ---------------------------------------------------------------------

int Sett::drivers_PIN_B1(){
	return m_nDrivers_PIN_B1;
};

// ---------------------------------------------------------------------

int Sett::drivers_PIN_B2(){
	return m_nDrivers_PIN_B2;
};


// ---------------------------------------------------------------------

int Sett::let_PIN_IN(){
	return m_nLet_PIN_IN;
};

// ---------------------------------------------------------------------

int Sett::let_PIN_SERVO(){
	return m_nLet_PIN_SERVO;
};

// ---------------------------------------------------------------------

int Sett::drivers_turnleft_A1(){
	return m_nDrivers_turnleft_A1;
};

// ---------------------------------------------------------------------

int Sett::drivers_turnleft_A2(){
	return m_nDrivers_turnleft_A2;
};

// ---------------------------------------------------------------------

		
int Sett::drivers_turnleft_B1(){
	return m_nDrivers_turnleft_B1;
};

// ---------------------------------------------------------------------

int Sett::drivers_turnleft_B2(){
	return m_nDrivers_turnleft_B2;
};

// ---------------------------------------------------------------------

int Sett::drivers_turnright_A1(){
	return m_nDrivers_turnright_A1;
};

// ---------------------------------------------------------------------

int Sett::drivers_turnright_A2(){
	return m_nDrivers_turnright_A2;
};

// ---------------------------------------------------------------------

int Sett::drivers_turnright_B1(){
	return m_nDrivers_turnright_B1;
};

// ---------------------------------------------------------------------

int Sett::drivers_turnright_B2(){
	return m_nDrivers_turnright_B2;
};

// ---------------------------------------------------------------------

int Sett::drivers_forward_A1(){
	return m_nDrivers_forward_A1;
};

// ---------------------------------------------------------------------

int Sett::drivers_forward_A2(){
	return m_nDrivers_forward_A2;
};

// ---------------------------------------------------------------------

int Sett::drivers_forward_B1(){
	return m_nDrivers_forward_B1;
};

// ---------------------------------------------------------------------

int Sett::drivers_forward_B2(){
	return m_nDrivers_forward_B2;
};

// ---------------------------------------------------------------------

int Sett::drivers_backward_A1(){
	return m_nDrivers_backward_A1;
};

// ---------------------------------------------------------------------

int Sett::drivers_backward_A2(){
	return m_nDrivers_backward_A2;
};

// ---------------------------------------------------------------------

int Sett::drivers_backward_B1(){
	return m_nDrivers_backward_B1;
};

// ---------------------------------------------------------------------

int Sett::drivers_backward_B2(){
	return m_nDrivers_backward_B2;
};

// ---------------------------------------------------------------------

int Sett::let0_POS_SERVO(){
	return m_nLet0_POS_SERVO;
}

// ---------------------------------------------------------------------

int Sett::let1_POS_SERVO(){
	return m_nLet1_POS_SERVO;
}

// ---------------------------------------------------------------------

int Sett::let2_POS_SERVO(){
	return m_nLet2_POS_SERVO;
}
		
		
		
