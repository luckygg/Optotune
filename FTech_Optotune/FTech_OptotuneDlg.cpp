
// FTech_OptotuneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FTech_Optotune.h"
#include "FTech_OptotuneDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTech_OptotuneDlg dialog




CFTech_OptotuneDlg::CFTech_OptotuneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFTech_OptotuneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFTech_OptotuneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFTech_OptotuneDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECTION, &CFTech_OptotuneDlg::OnBnClickedBtnConnection)
	ON_WM_VSCROLL()
	ON_MESSAGE(WM_RECEIVE_MSG , OnReceiveMsg)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFTech_OptotuneDlg message handlers

BOOL CFTech_OptotuneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetDlgItemText(IDC_EDIT_PORT,L"COM");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFTech_OptotuneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFTech_OptotuneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFTech_OptotuneDlg::OnBnClickedBtnConnection()
{
	CString caption=L"";
	GetDlgItemText(IDC_BTN_CONNECTION, caption);

	if (caption == L"Connect")
	{
		CString port;
		GetDlgItemText(IDC_EDIT_PORT,port);
		bool ret = m_Optotune.OnOpen(this,port,L"115200",L"8",L"1",L"NONE");
		if (ret == true)
		{
			SetTimer(100,30,NULL);
			SetDlgItemText(IDC_BTN_CONNECTION, L"Disconnect");

			//m_Optotune.SetModeCurrent();
			//m_Optotune.SetRangeMinus(true);

			m_Optotune.SetModeFocalPower();
			CSliderCtrl* pCtrl = (CSliderCtrl*)GetDlgItem(IDC_VSL_VALUE);
			pCtrl->SetRange(0, RANGE_FP_MAX_EL16);
			//pCtrl->SetRange(0, RANGE_CURRENT_MAX);
			pCtrl->SetPos(0);
		}
	}
	else
	{
		bool ret = m_Optotune.OnClose();
		if (ret == true)
		{
			KillTimer(100);
			SetDlgItemText(IDC_BTN_CONNECTION, L"Connect");
		}
	}
}

void CFTech_OptotuneDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pCtrl = (CSliderCtrl*)GetDlgItem(IDC_VSL_VALUE);
	if (pScrollBar->GetSafeHwnd() == pCtrl->GetSafeHwnd())
	{
		int n = pCtrl->GetPos();
		SetDlgItemInt(IDC_EDIT_VALUE, n);

		//m_Optotune.OnSendDataCurrent(n);
		m_Optotune.OnSendDataFP(n,true);
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CFTech_OptotuneDlg::OnReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	int nSize = (int)lParam;
	CString *msg = (CString*)wParam;
	msg->Replace(L" ",L"");

	CString header = msg->Left(4);
	if (header == L"5441")
	{
		CString strValue = msg->Mid(6,4); // HEX
		float fValue = m_Optotune.ConvertTemperature(strValue);
		strValue.Format(L"%.3f",fValue);
		SetDlgItemText(IDC_LB_TEMPERATURE,strValue);
	}

	return 0;
}

void CFTech_OptotuneDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100)
	{
		if (m_Optotune.IsOpened() == false) return;

		m_Optotune.GetTemperature();
	}

	CDialogEx::OnTimer(nIDEvent);
}
