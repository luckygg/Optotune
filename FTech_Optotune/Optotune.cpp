#include "StdAfx.h"
#include "Optotune.h"

//----------- CRC 16 --------------------//
#define                 P_16        0xA001
static unsigned short   crc_tab16[256];
static int              crc_tab16_init = FALSE;

COptotune::COptotune(void)
{
	m_bRangeMinus = false;
}


COptotune::~COptotune(void)
{
	
}

bool COptotune::OnOpen(CString Port, CString Baudrate, CString Databit, CString Stopbit, CString Paritybit)
{
	BOOL ret = m_CommSerial.OnOpen(Port,Baudrate,Databit,Stopbit,Paritybit);
	if (ret != TRUE)
		return false;

	SendStartCommand();
	SendTemperatureLimit();

	return true;
}

bool COptotune::OnOpen(CWnd* pParent, CString Port, CString Baudrate, CString Databit, CString Stopbit, CString Paritybit)
{
	if (pParent == NULL) return false;

	BOOL ret = m_CommSerial.OnOpen(pParent,Port,Baudrate,Databit,Stopbit,Paritybit);
	if (ret != TRUE)
		return false;

	SendStartCommand();
	SendTemperatureLimit();

	return true;
}

bool COptotune::OnClose()
{
	if (m_CommSerial.IsOpened() == false)
		return false;

	BOOL ret = m_CommSerial.OnClose();
	if (ret != TRUE)
		return false;

	return true;
}

CString COptotune::DecToHex(int value)
{
	CString tmp;
	tmp.Format(_T("%04x"),value);
	return tmp;
}

void COptotune::init_crc16_tab( void ) 
{

	int i, j;
	unsigned short crc, c;

	for (i=0; i<256; i++) {

		crc = 0;
		c   = (unsigned short) i;

		for (j=0; j<8; j++) {

			if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ P_16;
			else                      crc =   crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}

	crc_tab16_init = TRUE;

}  

unsigned short COptotune::update_crc_16( unsigned short crc, char c ) 
{

	unsigned short tmp, short_c;

	short_c = 0x00ff & (unsigned short) c;

	if ( ! crc_tab16_init ) init_crc16_tab();

	tmp =  crc       ^ short_c;
	crc = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];

	return crc;

}

void COptotune::OnSendDataFP(int nValue, bool bEL16)
{
	//EL-16의 경우 -474 ~ 714 까지 안정 범위.
	//EL-30의 경우, 6.83dpt (2366) ~ 12.83dpt (3566) 까지 안정 범위.
	
	if (bEL16 == true)
		nValue -= 474;	//EL-16
	else
		nValue += 2366;	//EL-30
	
	CString data = DecToHex(nValue);
	if (nValue < 0)
		data = data.Right(4);

	data = L"50774441" + data + L"0000"; //PwDA

	char input_string[128];
	char *ptr, *dest, hex_val, prev_byte;
	unsigned short crc_16;


	strcpy_s(input_string, 128, (char*)_bstr_t(data));

	ptr  = input_string;
	dest = input_string;

	while( *ptr  &&  *ptr != '\r'  &&  *ptr != '\n' ) {

		if ( *ptr >= '0'  &&  *ptr <= '9' ) *dest++ = (char) ( (*ptr) - '0'      );
		if ( *ptr >= 'A'  &&  *ptr <= 'F' ) *dest++ = (char) ( (*ptr) - 'A' + 10 );
		if ( *ptr >= 'a'  &&  *ptr <= 'f' ) *dest++ = (char) ( (*ptr) - 'a' + 10 );

		ptr++;
	}

	* dest    = '\x80';
	*(dest+1) = '\x80';

	crc_16         = 0;
	prev_byte = 0;
	ptr       = input_string;
	while ( *ptr != '\x80' ) {
		hex_val  = (char) ( ( * ptr     &  '\x0f' ) << 4 );
		hex_val |= (char) ( ( *(ptr+1)  &  '\x0f' )      );

		crc_16         = update_crc_16(crc_16,hex_val);
		prev_byte = hex_val;
		ptr      += 2;
	}

	input_string[0] = 0;


	CString test;
	test.Format(_T("%04x"),crc_16);
	CString high, low;

	int len = test.GetLength();

	if (len < 3)
	{
		high = L"00";
		low = test;
	}
	else if (len == 3)
	{
		low = test.Right(2);
		high = test.Left(1);
	}
	else
	{
		low = test.Right(2);
		high = test.Left(2);
	}

	data = data + low + high;

	data.MakeUpper();

	m_CommSerial.OnSendData(true,false,data,false,false);
}  

void COptotune::OnSendDataCurrent(int nValue)
{
	if (m_bRangeMinus)
		nValue -= 4096;

	CString data = DecToHex(nValue);

	if (nValue < 0)
		data = data.Right(4);

	data = L"4177" + data;

	char input_string[128];
	char *ptr, *dest, hex_val, prev_byte;
	unsigned short crc_16;


	strcpy_s(input_string, 128, (char*)_bstr_t(data));

	ptr  = input_string;
	dest = input_string;

	while( *ptr  &&  *ptr != '\r'  &&  *ptr != '\n' ) {

		if ( *ptr >= '0'  &&  *ptr <= '9' ) *dest++ = (char) ( (*ptr) - '0'      );
		if ( *ptr >= 'A'  &&  *ptr <= 'F' ) *dest++ = (char) ( (*ptr) - 'A' + 10 );
		if ( *ptr >= 'a'  &&  *ptr <= 'f' ) *dest++ = (char) ( (*ptr) - 'a' + 10 );

		ptr++;
	}

	* dest    = '\x80';
	*(dest+1) = '\x80';

	crc_16         = 0;
	prev_byte = 0;
	ptr       = input_string;
	while ( *ptr != '\x80' ) {
		hex_val  = (char) ( ( * ptr     &  '\x0f' ) << 4 );
		hex_val |= (char) ( ( *(ptr+1)  &  '\x0f' )      );

		crc_16         = update_crc_16(crc_16,hex_val);
		prev_byte = hex_val;
		ptr      += 2;
	}

	input_string[0] = 0;


	CString test;
	test.Format(_T("%04x"),crc_16);
	CString high, low;

	int len = test.GetLength();

	if (len < 3)
	{
		high = L"00";
		low = test;
	}
	else if (len == 3)
	{
		low = test.Right(2);
		high = test.Left(1);
	}
	else
	{
		low = test.Right(2);
		high = test.Left(2);
	}

	data = data + low + high;

	m_CommSerial.OnSendData(true,false,data,false,false);
}  


//////////////////////////////////////////////////////////////////////////
void COptotune::SendStartCommand()
{
	if (m_CommSerial.IsOpened() == false) return;
	
	m_CommSerial.OnSendData(false,false,L"Start",false,false);
}

void COptotune::SendTemperatureLimit()
{
	if (m_CommSerial.IsOpened() == false) return;

	//Temperature limits 20 ~ 35 degree.
	m_CommSerial.OnSendData(true,false,L"50 77 54 41 02 30 01 40 32 37",false,false);
}

void COptotune::SetModeCurrent()
{
	if (m_CommSerial.IsOpened() == false) return;

	// MwDA
	m_CommSerial.OnSendData(true,false,L"4D 77 44 41 54 46",false,false);
}

void COptotune::SetModeFocalPower()
{
	if (m_CommSerial.IsOpened() == false) return;
	
	// MwCA
	m_CommSerial.OnSendData(true,false,L"4D 77 43 41 56 76",false,false);
}

void COptotune::GetTemperature()
{
	if (m_CommSerial.IsOpened() == false) return;

	m_CommSerial.SetReceiveType(true);
	// TA
	m_CommSerial.OnSendData(true,false,L"5441FEF0",false,false);
}

float COptotune::ConvertTemperature(CString strHex)
{
	if (strHex.IsEmpty() == true) return 0;

	int len = strHex.GetLength();

	int pos=0;
	int sum=0;
	for (int i=0; i<len; i+=2)
	{
		BYTE high = m_CommSerial.ConvertCode2Ascii((char)strHex[i]);
		BYTE low = m_CommSerial.ConvertCode2Ascii((char)strHex[i+1]);

		if (i==0)
		{
			sum += (high<<12);
			sum += (low<<8);
		}
		else
		{
			sum += (high<<4);
			sum += (low);
		}
		
	}
	return (float)(sum*0.0625);
}