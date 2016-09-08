// ProperyDialog_Wizard.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ProperyDialog_Wizard.h"
#include "ProperyDialog_WizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MySheet.h"

// CProperyDialog_WizardApp

BEGIN_MESSAGE_MAP(CProperyDialog_WizardApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CProperyDialog_WizardApp 构造

CProperyDialog_WizardApp::CProperyDialog_WizardApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CProperyDialog_WizardApp 对象

CProperyDialog_WizardApp theApp;


// CProperyDialog_WizardApp 初始化

BOOL CProperyDialog_WizardApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	///////////////////////////////////////
	// 定义属性对话框对象
	CMySheet MySheet ( L"[第2章]属性对话框——向导对话框" ) ;
	// 设置属性对话框为向导模型
	MySheet.SetWizardMode () ;
	// 显示向导对话框
	MySheet.DoModal () ;
	return TRUE ;
	///////////////////////////////////////

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CProperyDialog_WizardDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
