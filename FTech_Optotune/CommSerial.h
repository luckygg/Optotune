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
	/// \brief					�޽��� ����.
	/// \param [in] bHEX		���۵Ǵ� �޽����� HEX / ASCII ���� Ȯ��.
	/// \param [in] bSTX		���۵Ǵ� �޽����� STX�� ���� ������ Ȯ��.
	/// \param [in] strMSG		���۵Ǵ� �޽���.
	/// \param [in] bETX		���۵Ǵ� �޽����� ETX�� ���� ������ Ȯ��.
	/// \param [in] bCRLF		���۵Ǵ� �޽����� CRLF�� ���� ������ Ȯ��.
	void OnSendData(bool bHEX, bool bSTX, CString strMSG, bool bETX, bool bCRLF);
	//******************************************************************************************************************
	/// \brief					 ��Ʈ�� �������� ���. (���� �޽����� ���� �ʱ� ���� �뵵.)
	/// \param [in] strPort		 COM PORT ���ڿ� �Է�.(ex. COM1 / COM9 / COM 14)
	/// \param [in] strBaudRate	 BuadRate ���ڿ� �Է�.(ex. 9600 / 19200 / 115200)
	/// \param [in] strDataBit	 DataBit ���ڿ� �Է�.(ex. 4 / 5 / 6 / 7 / 8)
	/// \param [in] strStopBit	 StopBit ���ڿ� �Է�.(ex. 1 / 1.5 / 2)
	/// \param [in] strParityBit ParityBit ���ڿ� �Է�.(ex. NONE / ODD / EVEN)
	/// \param bool				 ��� ��ȯ.
	bool OnOpen(CString strPort, CString strBaudRate, CString strDataBit, CString strStopBit, CString strParityBit);
	//******************************************************************************************************************
	/// \brief					 ��Ʈ�� �������� ���. (���� �޽����� �ޱ� ���� �뵵.)
	/// \param [in] pParent		 ������ �ޱ� ���� ���� Ŭ������ CWnd ������ ����.
	/// \param [in] strPort		 COM PORT ���ڿ� �Է�.(ex. COM1 / COM9 / COM 14)
	/// \param [in] strBaudRate	 BuadRate ���ڿ� �Է�.(ex. 9600 / 19200 / 115200)
	/// \param [in] strDataBit	 DataBit ���ڿ� �Է�.(ex. 4 / 5 / 6 / 7 / 8)
	/// \param [in] strStopBit	 StopBit ���ڿ� �Է�.(ex. 1 / 1.5 / 2)
	/// \param [in] strParityBit ParityBit ���ڿ� �Է�.(ex. NONE / ODD / EVEN)
	/// \param bool				 ��� ��ȯ.
	bool OnOpen(CWnd* pParent, CString strPort, CString strBaudRate, CString strDataBit, CString strStopBit, CString strParityBit);
	//******************************************************************************************************************
	/// \brief					 ��Ʈ�� �ݱ����� ���.
	bool OnClose();
	//******************************************************************************************************************
	/// \brief					 ��Ʈ�� �����ִ��� Ȯ��.
	/// \param bool				 ��� ��ȯ.
	bool IsOpened() {return m_ComPort.IsConnected(); }
	//******************************************************************************************************************
	/// \brief					 ����� ��Ʈ�� ��ȣ�� Ȯ��.
	/// \param WORD				 ��Ʈ ��ȣ ��ȯ.
	WORD	GetPortID()		{ return m_ComPort.m_wPortID; }
	//******************************************************************************************************************
	/// \brief					 ����� ��Ʈ�� ��Ʈ ��ȣ ���ڿ��� Ȯ��.
	/// \param CString			 ��Ʈ ���ڿ� ��ȯ.
	CString GetComPort()	{ return m_strComPort;	}
	//******************************************************************************************************************
	/// \brief					 ����� ��Ʈ�� Baud Rate ���ڿ��� Ȯ��.
	/// \param CString			 BaudRate ��ȯ.
	CString GetBaudRate()	{ return m_strBaudRate; }
	//******************************************************************************************************************
	/// \brief					 ����� ��Ʈ�� Data Bit ���ڿ��� Ȯ��.
	/// \param CString			 Data Bit ���ڿ���ȯ.
	CString GetDataBit()	{ return m_strDataBit;	}
	//******************************************************************************************************************
	/// \brief					 ����� ��Ʈ�� Stop Bit ���ڿ��� Ȯ��.
	/// \param CString			 Stop Bit ���ڿ� ��ȯ.
	CString GetStopBit()	{ return m_strStopBit;	}
	//******************************************************************************************************************
	/// \brief					 ����� ��Ʈ�� Parity Bit ���ڿ��� Ȯ��.
	/// \param CString			 Parity Bit ���ڿ� ��ȯ.
	CString GetParityBit()	{ return m_strParityBit;}
	//******************************************************************************************************************
	/// \brief					 Character ���ڸ� BYTE�� ��ȯ.
	/// \param BYTE				 ��ȯ�� BYTE ��ȯ.
	BYTE ConvertCode2Ascii(char cData);
	//******************************************************************************************************************
	/// \brief					 ���� �޽����� ������ Ÿ���� ����.
	/// \param [in] bHEX		 true : HEX ������, false : ASCII ������.
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


