// TCPServerDlg.h : 头文件
//

#pragma once


// CTCPServerDlg 对话框
class CTCPServerDlg : public CDialog
{
// 构造
public:
	CTCPServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCPSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	UINT nDataA;
public:
	UINT nDataB;
public:
	UINT nDataC;
public:
	BOOL	bStatus ;
	HANDLE	hThread ;
	friend	DWORD WINAPI ServerThread ( LPVOID lParam ) ;
};
