
// FTech_OptotuneDlg.h : header file
//

#pragma once
#include "Optotune.h"

#define RANGE_MIN 0
#define RANGE_FP_MAX_EL16 1188
#define RANGE_FP_MAX_EL30 1202
#define RANGE_CURRENT_MAX 8192
// CFTech_OptotuneDlg dialog
class CFTech_OptotuneDlg : public CDialogEx
{
// Construction
public:
	CFTech_OptotuneDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FTECH_OPTOTUNE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public :
	COptotune m_Optotune;
	LRESULT OnReceiveMsg(WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnection();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
