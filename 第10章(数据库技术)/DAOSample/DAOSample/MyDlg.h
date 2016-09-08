#pragma once


// CMyDlg 对话框

class CMyDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyDlg)

public:
	CMyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();

public:
	BOOL		bShowMode ;
	VOID		SetMode ( BOOL bType ) ;
	CString&	GetBookName () ;
	CString&	GetAuthor () ;
	CString&	GetPublish () ;
	UINT		GetPrice () ;

	VOID		SetBookName ( CString str ) ;
	VOID		SetAuthor ( CString str ) ;
	VOID		SetPublish ( CString str ) ;
	VOID		SetPrice ( UINT nPrice ) ;
public:
	CString szBookName;
public:
	CString szAuthor;
public:
	CString szPublish;
public:
	UINT nPrice;
};
