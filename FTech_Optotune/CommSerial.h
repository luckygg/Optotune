#pragma once

#include "CommThread.h"

#define WM_RECEIVE_MSG WM_USER+100
class CCommSerial : public CWnd
{
	DECLARE_DYNAMIC(CCommSerial)

public:
	CCommSerial();
	virtual ~CCommSerial();


public :
	//******************************************************************************************************************
	/// \brief					메시지 전송.
	/// \param [in] bHEX		전송되는 메시지가 HEX / ASCII 인지 확인.
	/// \param [in] bSTX		전송되는 메시지에 STX를 붙일 것인지 확인.
	/// \param [in] strMSG		전송되는 메시지.
	/// \param [in] bETX		전송되는 메시지에 ETX를 붙일 것인지 확인.
	/// \param [in] bCRLF		전송되는 메시지에 CRLF를 붙일 것인지 확인.
	void OnSendData(bool bHEX, bool bSTX, CString strMSG, bool bETX, bool bCRLF);
	//******************************************************************************************************************
	/// \brief					 포트를 열기위해 사용. (응답 메시지를 받지 않기 위한 용도.)
	/// \param [in] strPort		 COM PORT 문자열 입력.(ex. COM1 / COM9 / COM 14)
	/// \param [in] strBaudRate	 BuadRate 문자열 입력.(ex. 9600 / 19200 / 115200)
	/// \param [in] strDataBit	 DataBit 문자열 입력.(ex. 4 / 5 / 6 / 7 / 8)
	/// \param [in] strStopBit	 StopBit 문자열 입력.(ex. 1 / 1.5 / 2)
	/// \param [in] strParityBit ParityBit 문자열 입력.(ex. NONE / ODD / EVEN)
	/// \param bool				 결과 반환.
	bool OnOpen(CString strPort, CString strBaudRate, CString strDataBit, CString strStopBit, CString strParityBit);
	//******************************************************************************************************************
	/// \brief					 포트를 열기위해 사용. (응답 메시지를 받기 위한 용도.)
	/// \param [in] pParent		 응답을 받기 위해 상위 클래스의 CWnd 포인터 전달.
	/// \param [in] strPort		 COM PORT 문자열 입력.(ex. COM1 / COM9 / COM 14)
	/// \param [in] strBaudRate	 BuadRate 문자열 입력.(ex. 9600 / 19200 / 115200)
	/// \param [in] strDataBit	 DataBit 문자열 입력.(ex. 4 / 5 / 6 / 7 / 8)
	/// \param [in] strStopBit	 StopBit 문자열 입력.(ex. 1 / 1.5 / 2)
	/// \param [in] strParityBit ParityBit 문자열 입력.(ex. NONE / ODD / EVEN)
	/// \param bool				 결과 반환.
	bool OnOpen(CWnd* pParent, CString strPort, CString strBaudRate, CString strDataBit, CString strStopBit, CString strParityBit);
	//******************************************************************************************************************
	/// \brief					 포트를 닫기위해 사용.
	bool OnClose();
	//******************************************************************************************************************
	/// \brief					 포트가 열려있는지 확인.
	/// \param bool				 결과 반환.
	bool IsOpened() {return m_ComPort.IsConnected(); }
	//******************************************************************************************************************
	/// \brief					 연결된 포트의 번호를 확인.
	/// \param WORD				 포트 번호 반환.
	WORD	GetPortID()		{ return m_ComPort.m_wPortID; }
	//******************************************************************************************************************
	/// \brief					 연결된 포트의 포트 번호 문자열을 확인.
	/// \param CString			 포트 문자열 반환.
	CString GetComPort()	{ return m_strComPort;	}
	//******************************************************************************************************************
	/// \brief					 연결된 포트의 Baud Rate 문자열을 확인.
	/// \param CString			 BaudRate 반환.
	CString GetBaudRate()	{ return m_strBaudRate; }
	//******************************************************************************************************************
	/// \brief					 연결된 포트의 Data Bit 문자열을 확인.
	/// \param CString			 Data Bit 문자열반환.
	CString GetDataBit()	{ return m_strDataBit;	}
	//******************************************************************************************************************
	/// \brief					 연결된 포트의 Stop Bit 문자열을 확인.
	/// \param CString			 Stop Bit 문자열 반환.
	CString GetStopBit()	{ return m_strStopBit;	}
	//******************************************************************************************************************
	/// \brief					 연결된 포트의 Parity Bit 문자열을 확인.
	/// \param CString			 Parity Bit 문자열 반환.
	CString GetParityBit()	{ return m_strParityBit;}
	//******************************************************************************************************************
	/// \brief					 Character 문자를 BYTE로 변환.
	/// \param BYTE				 변환된 BYTE 반환.
	BYTE ConvertCode2Ascii(char cData);
	//******************************************************************************************************************
	/// \brief					 응답 메시지의 데이터 타입을 선택.
	/// \param [in] bHEX		 true : HEX 데이터, false : ASCII 데이터.
	void SetReceiveType(bool bHEX) { m_bRcvHex=bHEX; }
private :
	CCommThread m_ComPort;
	CWnd* m_pParant;

	//----- Send Message -----//
	void SendData(bool bHEX, bool bSTX, CString strMSG, bool bETX, bool bCRLF);
	
	//----- Convert data type -----//
	CString ConvertComPort(CString strPort);
	DWORD ConvertBaudRate(CString strBaudRate);
	BYTE ConvertDataBit(CString strDataBit);
	BYTE ConvertStopBit(CString strStopBit);
	BYTE ConvertParityBit(CString strParityBit);
	
	//----- Current Port Information -----//
	CString m_strComPort;
	CString m_strBaudRate;
	CString m_strDataBit;
	CString m_strStopBit;
	CString m_strParityBit;
	
	bool m_bRcvHex;
protected:
	//afx_msg long OnCommunication(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommunication(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


