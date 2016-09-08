#pragma once

#include <windows.h>
#include <dinput.h>
#pragma comment ( lib, "dinput8.lib" )
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dxerr9.lib" )

// 支持手柄的最大数量
#define JS_MAXNUM			8

class CJoystick
{
private:
	UINT					uDeviceNum ;	// 手柄数量
	LPDIRECTINPUT8			lpDI ;			// Direct Input对象
	LPDIRECTINPUTDEVICE8	lpDID[JS_MAXNUM] ;				// Direct Input手柄设备对象
	DIDEVICEINSTANCE		DIDeviceInstance[JS_MAXNUM] ;	// 手柄设备实例

public:
	CJoystick(void);
public:
	~CJoystick(void);

public:
	// 初始化DirectInput，创建DI对象和枚举输入设备
	bool	JS_Initial ( ) ;
	// 启动输入设备
	bool	JS_StartByIndex ( UINT uIndex, \
					HWND hwnd, bool bExclusive = false, bool bForeGround = true ) ;
	// 取得输入设备的立即数据
	bool	JS_GetStateByIndex ( UINT uIndex, DIJOYSTATE2* lpState ) ;
	// 停止输入设备
	void	JS_StopByIndex ( UINT uIndex ) ;

public:
	UINT	JS_GetDeviceNum () ;

friend BOOL CALLBACK EnumDeviceProc ( LPCDIDEVICEINSTANCE lpddi, LPVOID lpRef ) ;
friend BOOL CALLBACK EnumObjectProc ( LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID lpRef ) ;
} ;

