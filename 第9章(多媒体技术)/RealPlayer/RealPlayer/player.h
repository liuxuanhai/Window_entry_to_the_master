#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CPlayer 包装类

class CPlayer : public CWnd
{
protected:
	DECLARE_DYNCREATE(CPlayer)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xCFCDAA03, 0x8BE4, 0x11CF, { 0xB8, 0x4B, 0x0, 0x20, 0xAF, 0xBB, 0xCC, 0xFA } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// IRealAudio

// Functions
//

	CString get_Source()
	{
		CString result;
		InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Source(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Console()
	{
		CString result;
		InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Console(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Controls()
	{
		CString result;
		InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Controls(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x68, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_NoLabels()
	{
		BOOL result;
		InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_NoLabels(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoStart()
	{
		BOOL result;
		InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoStart(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoGotoURL()
	{
		BOOL result;
		InvokeHelper(0x6b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoGotoURL(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_WindowName()
	{
		CString result;
		InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_WindowName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString GetSource()
	{
		CString result;
		InvokeHelper(0xd6, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetSource(LPCTSTR lpszNewValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszNewValue);
	}
	CString GetConsole()
	{
		CString result;
		InvokeHelper(0xd8, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetConsole(LPCTSTR lpszNewValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszNewValue);
	}
	CString GetControls()
	{
		CString result;
		InvokeHelper(0xda, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetControls(LPCTSTR lpszNewValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xdb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lpszNewValue);
	}
	BOOL GetNoLabels()
	{
		BOOL result;
		InvokeHelper(0xdc, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetNoLabels(BOOL bNewValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xdd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bNewValue);
	}
	BOOL GetAutoStart()
	{
		BOOL result;
		InvokeHelper(0xde, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetAutoStart(BOOL bNewValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xdf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bNewValue);
	}
	BOOL GetAutoGotoURL()
	{
		BOOL result;
		InvokeHelper(0xe0, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetAutoGotoURL(BOOL bNewValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xe1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bNewValue);
	}
	short GetVolume()
	{
		short result;
		InvokeHelper(0xe2, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	void SetVolume(short nVol)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xe3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nVol);
	}
	BOOL GetMute()
	{
		BOOL result;
		InvokeHelper(0xe4, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetMute(BOOL bMute)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xe5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bMute);
	}
	BOOL GetLoop()
	{
		BOOL result;
		InvokeHelper(0xe6, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetLoop(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xe7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetImageStatus()
	{
		BOOL result;
		InvokeHelper(0xea, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetImageStatus(BOOL bEnable)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xeb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bEnable);
	}
	long GetPacketsTotal()
	{
		long result;
		InvokeHelper(0xec, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPacketsReceived()
	{
		long result;
		InvokeHelper(0xed, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPacketsOutOfOrder()
	{
		long result;
		InvokeHelper(0xee, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPacketsMissing()
	{
		long result;
		InvokeHelper(0xef, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPacketsEarly()
	{
		long result;
		InvokeHelper(0xf0, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPacketsLate()
	{
		long result;
		InvokeHelper(0xf1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetBandwidthAverage()
	{
		long result;
		InvokeHelper(0xf2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetBandwidthCurrent()
	{
		long result;
		InvokeHelper(0xf3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void DoPlayPause()
	{
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoStop()
	{
		InvokeHelper(0xca, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoNextItem()
	{
		InvokeHelper(0xcb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoPrevItem()
	{
		InvokeHelper(0xcc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL CanPlayPause()
	{
		BOOL result;
		InvokeHelper(0xcd, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL CanStop()
	{
		BOOL result;
		InvokeHelper(0xce, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL HasNextItem()
	{
		BOOL result;
		InvokeHelper(0xcf, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL HasPrevItem()
	{
		BOOL result;
		InvokeHelper(0xd0, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL HasNextEntry()
	{
		BOOL result;
		InvokeHelper(0x153, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL HasPrevEntry()
	{
		BOOL result;
		InvokeHelper(0x154, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void DoNextEntry()
	{
		InvokeHelper(0x155, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoPrevEntry()
	{
		InvokeHelper(0x156, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EditPreferences()
	{
		InvokeHelper(0xd2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void HideShowStatistics()
	{
		InvokeHelper(0xd3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL IsStatisticsVisible()
	{
		BOOL result;
		InvokeHelper(0xd4, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void DoGotoURL(LPCTSTR url, LPCTSTR target)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xd5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, url, target);
	}
	void DoPlay()
	{
		InvokeHelper(0x101, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void DoPause()
	{
		InvokeHelper(0x102, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long GetPosition()
	{
		long result;
		InvokeHelper(0x103, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPlayState()
	{
		long result;
		InvokeHelper(0x104, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetLength()
	{
		long result;
		InvokeHelper(0x105, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetTitle()
	{
		CString result;
		InvokeHelper(0x106, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetAuthor()
	{
		CString result;
		InvokeHelper(0x107, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetCopyright()
	{
		CString result;
		InvokeHelper(0x108, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetClipWidth()
	{
		long result;
		InvokeHelper(0x109, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetClipHeight()
	{
		long result;
		InvokeHelper(0x10a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL CanPlay()
	{
		BOOL result;
		InvokeHelper(0x10b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL CanPause()
	{
		BOOL result;
		InvokeHelper(0x10c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetPosition(long lPosition)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x10d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lPosition);
	}
	long GetNumLoop()
	{
		long result;
		InvokeHelper(0x10e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetNumLoop(long lVal)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x10f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lVal);
	}
	BOOL GetCenter()
	{
		BOOL result;
		InvokeHelper(0x110, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetCenter(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x111, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetNoLogo()
	{
		BOOL result;
		InvokeHelper(0x112, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetNoLogo(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x113, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetMaintainAspect()
	{
		BOOL result;
		InvokeHelper(0x114, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetMaintainAspect(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x115, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	CString GetBackgroundColor()
	{
		CString result;
		InvokeHelper(0x116, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetBackgroundColor(LPCTSTR pVal)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x117, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pVal);
	}
	BOOL GetStereoState()
	{
		BOOL result;
		InvokeHelper(0x118, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL GetLiveState()
	{
		BOOL result;
		InvokeHelper(0x119, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL GetShowStatistics()
	{
		BOOL result;
		InvokeHelper(0x11a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetShowStatistics(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x11b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetShowPreferences()
	{
		BOOL result;
		InvokeHelper(0x11c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetShowPreferences(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x11d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetShowAbout()
	{
		BOOL result;
		InvokeHelper(0x11e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetShowAbout(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x11f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetOriginalSize()
	{
		BOOL result;
		InvokeHelper(0x120, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetOriginalSize()
	{
		InvokeHelper(0x121, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL GetDoubleSize()
	{
		BOOL result;
		InvokeHelper(0x122, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetDoubleSize()
	{
		InvokeHelper(0x123, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL GetFullScreen()
	{
		BOOL result;
		InvokeHelper(0x124, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetFullScreen()
	{
		InvokeHelper(0x125, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL GetEnableContextMenu()
	{
		BOOL result;
		InvokeHelper(0x126, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetEnableContextMenu(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x127, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetEnableOriginalSize()
	{
		BOOL result;
		InvokeHelper(0x128, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetEnableOriginalSize(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x129, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetEnableDoubleSize()
	{
		BOOL result;
		InvokeHelper(0x12a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetEnableDoubleSize(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x12b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetEnableFullScreen()
	{
		BOOL result;
		InvokeHelper(0xf4, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetEnableFullScreen(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xf5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetEnableMessageBox()
	{
		BOOL result;
		InvokeHelper(0x151, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetEnableMessageBox(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x152, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	void SetTitle(LPCTSTR pVal)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xf6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pVal);
	}
	void SetAuthor(LPCTSTR pVal)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xf7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pVal);
	}
	void SetCopyright(LPCTSTR pVal)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xf8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pVal);
	}
	BOOL GetWantKeyboardEvents()
	{
		BOOL result;
		InvokeHelper(0x132, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetWantKeyboardEvents(BOOL bWantsEvents)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x131, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bWantsEvents);
	}
	BOOL GetWantMouseEvents()
	{
		BOOL result;
		InvokeHelper(0x134, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetWantMouseEvents(BOOL bWantsEvents)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x133, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bWantsEvents);
	}
	short GetNumEntries()
	{
		short result;
		InvokeHelper(0x135, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	short GetCurrentEntry()
	{
		short result;
		InvokeHelper(0x136, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	CString GetEntryTitle(short uEntryIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x137, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, uEntryIndex);
		return result;
	}
	CString GetEntryAuthor(short uEntryIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x138, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, uEntryIndex);
		return result;
	}
	CString GetEntryCopyright(short uEntryIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x139, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, uEntryIndex);
		return result;
	}
	CString GetEntryAbstract(short uEntryIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x13a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, uEntryIndex);
		return result;
	}
	void SetCanSeek(BOOL bCanSeek)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x13b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bCanSeek);
	}
	BOOL GetCanSeek()
	{
		BOOL result;
		InvokeHelper(0x13c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long GetBufferingTimeElapsed()
	{
		long result;
		InvokeHelper(0x13d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetBufferingTimeRemaining()
	{
		long result;
		InvokeHelper(0x13e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetConnectionBandwidth()
	{
		long result;
		InvokeHelper(0x13f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetPreferedLanguageString()
	{
		CString result;
		InvokeHelper(0x140, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetPreferedLanguageID()
	{
		long result;
		InvokeHelper(0x141, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetUserCountryID()
	{
		long result;
		InvokeHelper(0x142, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	short GetNumSources()
	{
		short result;
		InvokeHelper(0x143, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	CString GetSourceTransport(short nSourceNum)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x144, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nSourceNum);
		return result;
	}
	BOOL GetWantErrors()
	{
		BOOL result;
		InvokeHelper(0x145, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetWantErrors(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x146, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetShuffle()
	{
		BOOL result;
		InvokeHelper(0x147, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetShuffle(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x148, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	CString GetVersionInfo()
	{
		CString result;
		InvokeHelper(0x149, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetLastMessage()
	{
		CString result;
		InvokeHelper(0x14b, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetLastErrorSeverity()
	{
		long result;
		InvokeHelper(0x14e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetLastErrorRMACode()
	{
		long result;
		InvokeHelper(0x14d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetLastErrorUserCode()
	{
		long result;
		InvokeHelper(0x14f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetLastErrorUserString()
	{
		CString result;
		InvokeHelper(0x150, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetLastErrorMoreInfoURL()
	{
		CString result;
		InvokeHelper(0x14c, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetPreFetch(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x157, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	BOOL GetPreFetch()
	{
		BOOL result;
		InvokeHelper(0x158, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetRegion(LPCTSTR pVal)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x159, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pVal);
	}
	CString GetRegion()
	{
		CString result;
		InvokeHelper(0x15a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL GetIsPlus()
	{
		BOOL result;
		InvokeHelper(0x15b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL GetConsoleEvents()
	{
		BOOL result;
		InvokeHelper(0x15c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetConsoleEvents(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x15d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	CString GetDRMInfo(LPCTSTR pVal)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x15e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, pVal);
		return result;
	}
	void SetShowVideo(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x15f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	void AddLicense(LPCTSTR pVal)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x160, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pVal);
	}
	void SetVideoOverlay(BOOL bVal)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x161, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVal);
	}
	void EnsureTranscryptionLicense(LPCTSTR pVal)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x162, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pVal);
	}

// Properties
//



};
