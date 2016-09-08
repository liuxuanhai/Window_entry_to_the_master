// SelProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "MemSearch.h"
#include "SelProcess.h"

#include "psapi.h"
#pragma comment ( lib, "psapi.lib" )


// CSelProcess 对话框

IMPLEMENT_DYNAMIC(CSelProcess, CDialog)

CSelProcess::CSelProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CSelProcess::IDD, pParent)
{

}

CSelProcess::~CSelProcess()
{
}

void CSelProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_INFO, ProcInfoList);
}


BEGIN_MESSAGE_MAP(CSelProcess, CDialog)
	ON_BN_CLICKED(IDC_SELECT, &CSelProcess::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_FLUSH, &CSelProcess::OnBnClickedFlush)
END_MESSAGE_MAP()


// CSelProcess 消息处理程序
void CSelProcess::OnBnClickedSelect()
{
	this->nCurSel = this->ProcInfoList.GetCurSel () ;
	if ( this->nCurSel == -1 )
	{
		this->MessageBox ( L"请选择进程！" ) ;
		return ;
	}
	CDialog::OnOK () ;
}

void CSelProcess::OnBnClickedFlush()
{
	this->ProcList.clear () ;
	this->ProcInfoList.ResetContent () ;

	DWORD	cbNeededProcess, cbNeededModule, dwProcessId[1024];
	HMODULE	hMod ;
	WCHAR	szPathName[2048] ;
	CString TempStr ;
	MY_PROCESS_INFO	ProcInfo ;
	EnumProcesses ( dwProcessId, sizeof(dwProcessId), &cbNeededProcess ) ;
	for ( unsigned i = 0; i < ( cbNeededProcess/sizeof(DWORD) ); i++ )
	{
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessId[i] );
		if ( hProcess != INVALID_HANDLE_VALUE )
		{
			EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeededModule ) ;
			GetModuleFileNameEx ( hProcess, hMod, szPathName, sizeof(szPathName) ) ;

			ProcInfo.dwProcessId = dwProcessId[i] ;
			if ( ProcInfo.dwProcessId == 0 )
				ProcInfo.szFilePath = L"System Idle Process" ;
			else if ( ProcInfo.dwProcessId == 4 )
				ProcInfo.szFilePath = L"System" ;
			else
				ProcInfo.szFilePath.Format ( L"%s", szPathName ) ;	
			this->ProcList.push_back ( ProcInfo ) ;

			TempStr.Format ( L"[%04d]%s", ProcInfo.dwProcessId, ProcInfo.szFilePath ) ;
			this->ProcInfoList.AddString ( TempStr ) ;

			CloseHandle ( hProcess ) ;
		}
	}
}

CString CSelProcess::GetSelProcessName()
{
	return this->ProcList[this->nCurSel].szFilePath ;
}
DWORD CSelProcess::GetSelProcessId()
{
	return this->ProcList[this->nCurSel].dwProcessId ;
}
BOOL CSelProcess::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->OnBnClickedFlush () ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
