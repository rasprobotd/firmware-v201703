#ifndef SETT_H
#define SETT_H

#include <QString>
#include <QSettings>
#include <QVariant>

class Sett {
	
	public:
		Sett();

		int drivers_PIN_A1();
		int drivers_PIN_A2();
		int drivers_PIN_B1();
		int drivers_PIN_B2();
		int let_PIN_IN();
		int let_PIN_ENABLE();
		int let_PIN_SERVO();

		int drivers_turnleft_A1();
		int drivers_turnleft_A2();
		int drivers_turnleft_B1();
		int drivers_turnleft_B2();
		int drivers_turnright_A1();
		int drivers_turnright_A2();
		int drivers_turnright_B1();
		int drivers_turnright_B2();
		int drivers_forward_A1();
		int drivers_forward_A2();
		int drivers_forward_B1();
		int drivers_forward_B2();
		int drivers_backward_A1();
		int drivers_backward_A2();
		int drivers_backward_B1();
		int drivers_backward_B2();
		int let0_POS_SERVO();
		int let1_POS_SERVO();
		int let2_POS_SERVO();
		
		bool load();
		bool save();
		bool isLoaded();
		
	private:
		bool m_bLoaded;
		
		int readIntFromSettings(QSettings &sett, QString settName, int defaultValue);
		QString readStringFromSettings(QSettings &sett, QString settName, QString defaultValue);
		QStringList readStringListFromSettings(QSettings &sett, QString settName, QStringList defaultValue);
		bool readBooleanFromSettings(QSettings &sett, QString settName, bool defaultValue);
		
		QString m_sFilename;
		
		int m_nDrivers_PIN_A1;
		int m_nDrivers_PIN_A2;
		int m_nDrivers_PIN_B1;
		int m_nDrivers_PIN_B2;
		int m_nLet_PIN_IN;
		int m_nLet_PIN_ENABLE;
		int m_nLet_PIN_SERVO;
		int m_nLet0_POS_SERVO;
		int m_nLet1_POS_SERVO;
		int m_nLet2_POS_SERVO;

		int m_nDrivers_turnleft_A1;
		int m_nDrivers_turnleft_A2;
		int m_nDrivers_turnleft_B1;
		int m_nDrivers_turnleft_B2;
		int m_nDrivers_turnright_A1;
		int m_nDrivers_turnright_A2;
		int m_nDrivers_turnright_B1;
		int m_nDrivers_turnright_B2;
		int m_nDrivers_forward_A1;
		int m_nDrivers_forward_A2;
		int m_nDrivers_forward_B1;
		int m_nDrivers_forward_B2;
		int m_nDrivers_backward_A1;
		int m_nDrivers_backward_A2;
		int m_nDrivers_backward_B1;
		int m_nDrivers_backward_B2;
		
};

#endif // SETT_H
