
#include <windows.h>
#include <string.h>

// 创建进程
VOID CreateProcessSample1 ()
{
	WCHAR lpPath[] = L"notepad.exe" ;
	STARTUPINFO si = { sizeof(si) } ;
	PROCESS_INFORMATION pi ;
	BOOL bStatus = CreateProcess ( NULL, lpPath, NULL, NULL, FALSE, 0,NULL, NULL, &si, &pi ) ;
	if ( bStatus == FALSE )
	{
		MessageBox ( 0, L"CreateProcess error, notepad.exe", 0, 0 ) ;
		return ;
	}
}

// 创建挂起进程
VOID CreateProcessSample2 ()
{
	WCHAR lpPath[] = L"notepad.exe" ;
	STARTUPINFO si = { sizeof(si) } ;
	PROCESS_INFORMATION pi ;
	// 步骤1：使用CREATE_SUSPENDED创建挂起进程
	BOOL bStatus = CreateProcess ( NULL, lpPath, NULL, NULL, FALSE, CREATE_SUSPENDED,NULL, NULL, &si, &pi ) ;
	if ( bStatus == FALSE )
	{
		MessageBox ( 0, L"CreateProcess error, notepad.exe", 0, 0 ) ;
		return ;
	}

	// 步骤2：在这里对子进程进行数据处理和模块注入等操作
	// ……
	// 步骤3：恢复执行
	ResumeThread ( pi.hThread ) ;
}

VOID CreateProcessSample3 ()
{
	STARTUPINFO si = { sizeof(si) } ;
	PROCESS_INFORMATION pi ;
	
	WCHAR lpPath[] = L"notepad.exe" ;
	LPVOID lpEnvironment = GetEnvironmentStrings() ;

	int len = wcslen ( (LPWSTR)lpEnvironment ) * 2 + 2;
	LPVOID lpNewEnvironment = GlobalAlloc ( GPTR, len ) ;
	memcpy ( lpNewEnvironment, lpEnvironment, len ) ;

	BOOL bStatus = CreateProcess ( NULL, lpPath, NULL, NULL, TRUE, 0,lpNewEnvironment, NULL, &si, &pi ) ;
	FreeEnvironmentStrings ( (LPWCH)lpEnvironment ) ;
	if ( bStatus == FALSE )
	{
		MessageBox ( 0, L"CreateProcess error, notepad.exe", 0, 0 ) ;
		return ;
	}
}

VOID CreateProcessSample4 ()
{
	WCHAR lpPath[] = L"notepad.exe" ;

	STARTUPINFO si = { sizeof(si) } ;
	// 设置STARTF_USESHOWWINDOW标记，使得STARTUPINFO结构的wShowWindow字段有效
	si.dwFlags |= STARTF_USESHOWWINDOW ;	
	// 设置窗口的显示方式，SW_HIDE表示隐藏方式
	si.wShowWindow = SW_HIDE ;

	PROCESS_INFORMATION pi ;
	BOOL bStatus = CreateProcess ( NULL, lpPath, NULL, NULL, FALSE, 0,NULL, NULL, &si, &pi ) ;
	if ( bStatus == FALSE )
	{
		MessageBox ( 0, L"CreateProcess error, notepad.exe", 0, 0 ) ;
		return ;
	}
}

int WINAPI WinMain (HINSTANCE hinstExe,HINSTANCE, PSTR pszCmdLine, int nCmdShow )
{

	return 0 ;
}
