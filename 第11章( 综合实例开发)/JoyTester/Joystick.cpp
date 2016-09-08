#include "StdAfx.h"
#include "Joystick.h"

CJoystick::CJoystick( )
{
	this->lpDI		= NULL ;
	this->uDeviceNum = 0 ;
	memset ( this->lpDID, 0, sizeof(this->lpDID) ) ;
	memset ( this->DIDeviceInstance, 0, sizeof(this->DIDeviceInstance) ) ;
}

CJoystick::~CJoystick(void)
{
}

BOOL CALLBACK EnumDeviceProc ( LPCDIDEVICEINSTANCE lpddi, LPVOID lpRef )
{
	CJoystick* pJoystick = (CJoystick*)lpRef ;
	memcpy ( &(pJoystick->DIDeviceInstance[pJoystick->uDeviceNum]), lpddi, sizeof(DIDEVICEINSTANCE) ) ;
	pJoystick->uDeviceNum ++ ;
	if ( pJoystick->uDeviceNum == JS_MAXNUM )
		return FALSE ;
	return TRUE ;
}

bool CJoystick::JS_Initial ()
{
	// 创建DI对象
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL) ;
	HRESULT	hr = DirectInput8Create ( hInstance, 0X0800, IID_IDirectInput8, (LPVOID*)&this->lpDI, NULL ) ;
	if ( FAILED(hr) )
		return false ;

	// 枚举设备
	this->lpDI->EnumDevices ( DI8DEVCLASS_GAMECTRL, EnumDeviceProc, this, DIEDFL_ATTACHEDONLY ) ;
	return true ;
}

BOOL CALLBACK EnumObjectProc ( LPCDIDEVICEOBJECTINSTANCE lpddi, LPVOID lpRef )
{
	LPDIRECTINPUTDEVICE8	lpCurDID = (LPDIRECTINPUTDEVICE8)lpRef ;

	if ( lpddi->dwType & DIDFT_AXIS )
	{
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = lpddi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        if( FAILED( lpCurDID->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return FALSE;
	}
	return TRUE ;
}

bool CJoystick::JS_StartByIndex ( UINT uIndex, \
					HWND hwnd, bool bExclusive, bool bForeGround ) 
{
	if ( uIndex >= this->uDeviceNum )
		return false ;

	// 创建输入设备对象
	HRESULT hr = this->lpDI->CreateDevice ( \
		this->DIDeviceInstance[uIndex].guidInstance, &(this->lpDID[uIndex]), NULL ) ;
	if ( FAILED(hr) )
		return false ;
		
	// 设置数据格式
	hr = this->lpDID[uIndex]->SetDataFormat ( &c_dfDIJoystick2 ) ;
	if ( FAILED(hr) )
		return false ;

	// 枚举输入设备上的对象，设置轴属性
	this->lpDID[uIndex]->EnumObjects ( EnumObjectProc, (LPVOID)(this->lpDID[uIndex]), DIDFT_ALL ) ;
	
	// 设置协作模式
	DWORD dwFlags = 0 ;
	dwFlags |= ( bExclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE ) ;
	dwFlags |= ( bForeGround ? DISCL_FOREGROUND : DISCL_BACKGROUND ) ;
	hr = this->lpDID[uIndex]->SetCooperativeLevel ( hwnd, dwFlags ) ;
	return ( SUCCEEDED(hr) ? true : false ) ;
}

bool CJoystick::JS_GetStateByIndex ( UINT uIndex, DIJOYSTATE2* lpState ) 
{
	if ( uIndex >= this->uDeviceNum )
		return false ;

	// 取得设备访问权
	HRESULT hr = this->lpDID[uIndex]->Acquire() ;
	if ( FAILED(hr) )
		return false ;

	// 取得立即模式数据
	hr = this->lpDID[uIndex]->GetDeviceState ( sizeof(DIJOYSTATE2), lpState ) ;
	if ( FAILED(hr) )
		return false ;
	
	return true ;
}

void CJoystick::JS_StopByIndex ( UINT uIndex )
{
	if ( uIndex >= this->uDeviceNum )
		return ;

	if ( this->lpDID[uIndex] )
	{
		this->lpDID[uIndex]->Unacquire () ;
		this->lpDID[uIndex]->Release () ;
	}
	if ( this->lpDI )
		this->lpDI->Release () ;
}

UINT CJoystick::JS_GetDeviceNum ()
{
	return this->uDeviceNum ;
}
