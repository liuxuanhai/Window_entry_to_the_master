// MyModalDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ModalDlg_MFC.h"
#include "MyModalDialog.h"


// CMyModalDialog 对话框

IMPLEMENT_DYNAMIC(CMyModalDialog, CDialog)

CMyModalDialog::CMyModalDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMyModalDialog::IDD, pParent)
{

}

CMyModalDialog::~CMyModalDialog()
{
}

void CMyModalDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyModalDialog, CDialog)
END_MESSAGE_MAP()


// CMyModalDialog 消息处理程序
