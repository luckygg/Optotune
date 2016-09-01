//----------------------------------------------------------
// ButtonEx Control
//----------------------------------------------------------
// Programmed by William Kim
//----------------------------------------------------------
// Last Update : 2016-09-01 18:35
//----------------------------------------------------------
#pragma once

#include "CommSerial.h"
class COptotune
{
public:
	COptotune(void);
	~COptotune(void);

public :
	//----- Open & Close -----//
	bool OnOpen(CString Port, CString Baudrate, CString Databit, CString Stopbit, CString Paritybit);
	bool OnOpen(CWnd* pParent, CString Port, CString Baudrate, CString Databit, CString Stopbit, CString Paritybit);
	bool OnClose();
	bool IsOpened() { return m_CommSerial.IsOpened(); }

	void SetRangeMinus(bool bEnable) { m_bRangeMinus = bEnable; }
	
	//----- Protocol -----//
	void SetModeFocalPower();
	void SetModeCurrent();
	void OnSendDataCurrent(int nValue);
	void OnSendDataFP(int nValue, bool bEL16);
	void GetTemperature();
	float ConvertTemperature(CString strHex);

	//----- Communication information -----//
	WORD GetPortID() { return m_CommSerial.GetPortID(); }
	CString GetComPort()	{ return m_CommSerial.GetComPort();   }
	CString GetBaudRate()	{ return m_CommSerial.GetBaudRate();  }
	CString GetDataBit()	{ return m_CommSerial.GetDataBit();   }
	CString GetStopBit()	{ return m_CommSerial.GetStopBit();	  }
	CString GetParityBit()	{ return m_CommSerial.GetParityBit(); }

private :
	CCommSerial	m_CommSerial;
	bool m_bRangeMinus;
	unsigned short update_crc_16( unsigned short crc, char c ) ;
	void init_crc16_tab( void ) ;
	CString DecToHex(int value);
	void SendTemperatureLimit();
	void SendStartCommand();
};
