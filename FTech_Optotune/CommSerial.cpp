// CCommSerial.cpp : implementation file
//

#include "stdafx.h"
#include "CommSerial.h"

// CCommSerial
IMPLEMENT_DYNAMIC(CCommSerial, CWnd)
HWND hCommWnd;

CCommSerial::CCommSerial()
{
	m_pParant		= NULL;
	m_strComPort	= L"";
	m_strBaudRate	= L"";
	m_strDataBit	= L"";
	m_strStopBit	= L"";
	m_strParityBit	= L"";
	m_bRcvHex		= false;
}

CCommSerial::~CCommSerial()
{
}

BEGIN_MESSAGE_MAP(CCommSerial, CWnd)
	ON_MESSAGE(WM_COMM_READ, OnCommunication)
END_MESSAGE_MAP()



// CCommSerial message handlers
LRESULT CCommSerial::OnCommunication(WPARAM wParam, LPARAM lParam)	
{
	CString tmp=L"",result=L"";
	char size[128]={0,};
	BYTE aByte; 
	int iCount = 0;

	int nSize = (m_ComPort.m_QueueRead).GetSize(); // 큐에 들어온 데이터 갯수

	for(int i  = 0 ; i < nSize; i++) //들어온 갯수 만큼 데이터를 읽어 와 화면에 보여줌
	{
		m_ComPort.m_QueueRead.GetByte(&aByte); //큐에서 데이터 1BYTE 를 읽어옴

		if (m_bRcvHex == true)
		{
			tmp.Format(_T("%02X"), aByte);	

			iCount++;

			if((iCount % nSize) == 0) 
				tmp += _T("\r\n");
			else if((iCount % 1) ==0)
				tmp += _T(" ");
			
			result += tmp;
		}
		else
		{
			tmp.Format(L"%c",aByte);
			result += tmp;
		}
	}	
	
	iCount = 0;
	m_ComPort.m_QueueRead.Clear();

	if (m_pParant != NULL)
		m_pParant->SendMessage(WM_RECEIVE_MSG, (WPARAM)&result, (LPARAM)GetPortID());
	return 0;
}

BYTE CCommSerial::ConvertCode2Ascii(char cData)
{
	//char문자를 hex값으로 변경
	BYTE byAsciiValue;
	if( ( '0' <= cData ) && ( cData <='9' ) )
	{
		byAsciiValue = cData - '0';
	}
	else if( ( 'A' <= cData ) && ( cData <= 'F' ) )
	{
		byAsciiValue = (cData - 'A') + 10;
	}
	else if( ( 'a' <= cData ) && ( cData <= 'f' ) )
	{
		byAsciiValue = (cData - 'a') + 10;
	}
	else
	{
		byAsciiValue = 0;
	}
	return byAsciiValue;
}

CString CCommSerial::ConvertComPort(CString strPort)
{
	strPort.Replace(L" ",L"");
	strPort.MakeUpper();

	if		(strPort == L"COM1" ) return L"COM1" ;
	else if (strPort == L"COM2" ) return L"COM2" ;
	else if (strPort == L"COM3" ) return L"COM3" ;
	else if (strPort == L"COM4" ) return L"COM4" ;
	else if (strPort == L"COM5" ) return L"COM5" ;
	else if (strPort == L"COM6" ) return L"COM6" ;
	else if (strPort == L"COM7" ) return L"COM7" ;
	else if (strPort == L"COM8" ) return L"COM8" ;
	else if (strPort == L"COM9" ) return L"COM9" ;
	else if (strPort == L"COM10") return L"\\\\.\\COM10";
	else if (strPort == L"COM11") return L"\\\\.\\COM11";
	else if (strPort == L"COM12") return L"\\\\.\\COM12";
	else if (strPort == L"COM13") return L"\\\\.\\COM13";
	else if (strPort == L"COM14") return L"\\\\.\\COM14";
	else if (strPort == L"COM15") return L"\\\\.\\COM15";
	else if (strPort == L"COM16") return L"\\\\.\\COM16";
	else if (strPort == L"COM17") return L"\\\\.\\COM17";
	else if (strPort == L"COM18") return L"\\\\.\\COM18";
	else if (strPort == L"COM19") return L"\\\\.\\COM19";

	return L"";
}

DWORD CCommSerial::ConvertBaudRate(CString strBaudRate)
{
	strBaudRate.Replace(L" ",L"");
	strBaudRate.MakeUpper();

	if		(strBaudRate == L"4800"  ) return CBR_4800;
	else if (strBaudRate == L"9600"  ) return CBR_9600;
	else if (strBaudRate == L"14400" ) return CBR_14400;
	else if (strBaudRate == L"19200" ) return CBR_19200;
	else if (strBaudRate == L"38400" ) return CBR_38400;
	else if (strBaudRate == L"56000" ) return CBR_56000;
	else if (strBaudRate == L"57600" ) return CBR_57600;
	else if (strBaudRate == L"115200") return CBR_115200;

	return 0;
}

BYTE CCommSerial::ConvertDataBit(CString strDataBit)
{
	strDataBit.Replace(L" ",L"");
	strDataBit.MakeUpper();

	if		(strDataBit == L"4") return 4;
	else if (strDataBit == L"5") return 5;
	else if (strDataBit == L"6") return 6;
	else if (strDataBit == L"7") return 7;
	else if (strDataBit == L"8") return 8;

	return 0;
}

BYTE CCommSerial::ConvertStopBit(CString strStopBit)
{
	strStopBit.Replace(L" ",L"");
	strStopBit.MakeUpper();

	if		(strStopBit == L"1"  ) return ONESTOPBIT;
	else if (strStopBit == L"1.5") return ONE5STOPBITS;
	else if (strStopBit == L"2"  ) return TWOSTOPBITS;

	return 0;
}

BYTE CCommSerial::ConvertParityBit(CString strParityBit)
{
	strParityBit.Replace(L" ",L"");
	strParityBit.MakeUpper();

	if		(strParityBit == L"NONE") return NOPARITY;
	else if (strParityBit == L"ODD" ) return ODDPARITY;
	else if (strParityBit == L"EVEN") return EVENPARITY;

	return 0;
}

bool CCommSerial::OnOpen(CString strPort, CString strBaudRate, CString strDataBit, CString strStopBit, CString strParityBit) 
{
	if(m_ComPort.IsConnected() == false)										
	{
		hCommWnd = this->m_hWnd; 
		CString port	= ConvertComPort(strPort);
		DWORD baudrate	= ConvertBaudRate(strBaudRate);
		BYTE databit	= ConvertDataBit(strDataBit);
		BYTE stopbit	= ConvertStopBit(strStopBit);
		BYTE paritybit	= ConvertParityBit(strParityBit);

		if(m_ComPort.OpenPort(port, baudrate, databit, stopbit, paritybit) == TRUE)
		{
			m_strComPort = port;
			m_strBaudRate = strBaudRate;
			m_strDataBit = strDataBit;
			m_strStopBit = strStopBit;
			m_strParityBit = strParityBit;

			int len = port.GetLength();
			port = port.Mid(3,len-3);
			m_ComPort.m_wPortID = _ttoi(port)+1;
			
			return true;
		}
		else
			return false;
	}
	else
	{
		AfxMessageBox(_T("Already Port is Opened!"));
		return false;
	}
}

bool CCommSerial::OnOpen(CWnd* pParent, CString strPort, CString strBaudRate, CString strDataBit, CString strStopBit, CString strParityBit) 
{
	if (pParent == NULL) return false;

	if(m_ComPort.IsConnected() == false)										
	{
		CString port	= ConvertComPort(strPort);
		DWORD baudrate	= ConvertBaudRate(strBaudRate);
		BYTE databit	= ConvertDataBit(strDataBit);
		BYTE stopbit	= ConvertStopBit(strStopBit);
		BYTE paritybit	= ConvertParityBit(strParityBit);

		if(m_ComPort.OpenPort(port, baudrate, databit, stopbit, paritybit) == TRUE)
		{
			m_strComPort = port;
			m_strBaudRate = strBaudRate;
			m_strDataBit = strDataBit;
			m_strStopBit = strStopBit;
			m_strParityBit = strParityBit;

			int len = port.GetLength();
			port = port.Mid(3,len-3);
			m_ComPort.m_wPortID = _ttoi(port);

			if (IsWindow(this->m_hWnd) == FALSE)
				Create(NULL,NULL, WS_CHILD,  CRect( 0, 0, 0 ,0 ), pParent, WM_USER + _ttoi(port));

			m_pParant = pParent;
			hCommWnd = this->m_hWnd; 

			return true;
		}
		else
			return false;
	}
	else
	{
		AfxMessageBox(_T("Already Port is Opened!"));
		return false;
	}
}

bool CCommSerial::OnClose() 
{
	if(m_ComPort.IsConnected() == true)		
	{	
		m_ComPort.ClosePort();		

		if (IsWindow(this->m_hWnd) == TRUE)
			DestroyWindow();

		return true;	
	}
	else
		return false;
}

void CCommSerial::OnSendData(bool bHEX, bool bSTX, CString strMSG, bool bETX, bool bCRLF) 
{
	SendData(bHEX,bSTX,strMSG,bETX,bCRLF);
}

void CCommSerial::SendData(bool bHEX, bool bSTX, CString strMSG, bool bETX, bool bCRLF)
{
	CString strHexMsg=L"";
	int nBufPos=0, nDatasize=0;
	BYTE* pSendBuff = NULL;

	if (bHEX == true)
	{
		strHexMsg = strMSG;
		strHexMsg.Replace(_T(" "),_T(""));		// 공백을 없앰.
		strHexMsg.Replace(_T("\r\n"),_T(""));	// 줄넘김을 없앰.
	}
	else
	{
		int nSize = strMSG.GetLength();
		for(int i = 0; i < nSize; i++)			// 문자열을 Hex문자열로 변경.
		{
			CString tmp=L"";
			tmp.Format(_T("%X"),strMSG[i]);
			strHexMsg += tmp;
		}
	}

	if(bSTX == TRUE)
		strHexMsg = _T("02") + strHexMsg; // [STX] + Msg

	if(bETX == TRUE)
		strHexMsg += _T("03"); // Msg + [ETX]

	if(bCRLF == TRUE)
		strHexMsg += _T("0D0A"); // Msg + [CR] + [LF]

	nDatasize = strHexMsg.GetLength();		// 최종 문자열의 길이를 얻음.

	if (nDatasize %2 != 0) nDatasize -= 1;

	pSendBuff = new BYTE[nDatasize];	// 전송 버퍼를 생성.
	memset(pSendBuff,0,nDatasize);

	for(int i = 0; i < nDatasize ; i+=2)	// 문자열을 실제 Hex 값으로 변경.
	{
		BYTE byHigh = ConvertCode2Ascii((char)strHexMsg[i]);
		BYTE byLow  = ConvertCode2Ascii((char)strHexMsg[i+1]);
		pSendBuff[nBufPos++] = (byHigh <<4) | byLow;
	}

	//마지막 문자가 1자리수 일때 처리 하기 위해  예) 1 -> 01
	if(nDatasize %2 !=0)
		pSendBuff[nBufPos++] = ConvertCode2Ascii((char)strHexMsg[nDatasize-1]);

	int etc = nBufPos % 8;
	//포트에 데이터를 8개씩 쓰기 위해
	//데이터의 길이가 8의 배수가 아니면 나머지 데이터는 따로 보내줌
	for(int j =0; j < nBufPos-etc; j+= 8)//8의 배수 보냄
		m_ComPort.WriteComm(&pSendBuff[j], 8) ;

	if(etc != 0)//나머지 데이터 전송
		m_ComPort.WriteComm(&pSendBuff[nBufPos -etc], etc) ;// 포트에 데이터 쓰기 

	delete []pSendBuff;
	pSendBuff = NULL;
}

