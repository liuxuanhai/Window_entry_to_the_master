#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "Info.h"
#include "DirManager.h"
#include "FAT.h"

class CSystem {
public:
	LPSTR			lpCurUser ;		// 当前登陆的用户名
	SYSTEM_HANDLE	SysHandle ;		// 映射结构
	CDirManager		DirManager ;	// 目录项管理
	CFAT			FATManager ;	// FAT管理

public:
	// 构造/析构函数
	CSystem () {}
	~CSystem () {}

	// 安装文件系统
	BOOL	SysInstall		() ;

	// 卸载文件系统
	BOOL	SysUnstall		() ;

	// 初始化文件系统
	BOOL	SysInitial		() ;

	// 取得用户信息（可用于枚举用户信息）
	UINT	SysGetUserInfo	( PUSER_INFO pItem, UINT nCurAddr ) ;

	// 检测用户是否存在
	BOOL	SysIsUserExist	( LPSTR lpUserName ) ;

	// 检测登陆信息是否合法
	BOOL	SysCheckLogInfo	( LPSTR lpUserName, LPSTR lpUserPwd ) ;

	// 添加用户
	BOOL	SysAddUserInfo	( LPSTR lpUserName, LPSTR lpUserPwd ) ;

	// 删除指定的用户
	BOOL	SysDeleteUser	( LPSTR lpUserName ) ;

	// 检测当前登陆用户的权限
	BOOL	SysCheckUserPow	( LPSTR lpPath, LPSTR lpName ) ;

	// 枚举指定目录
	UINT	SysFirstEnumDir	( LPSTR lpPath, PDIRECTORY_ITEM pItem ) ;
	UINT	SysNextEnumDir	( UINT nCurAddr, PDIRECTORY_ITEM pItem ) ;

	// 检测在指定目录下的文件是否存在
	BOOL	SysIsFileExist	( LPSTR lpPath, LPSTR lpName ) ;

	// 创建新文件
	BOOL	SysCreateFile	( LPSTR lpPath, LPSTR lpName, LPSTR lpExt, LPSTR lpContent ) ;

	// 删除指定文件
	BOOL	SysDeleteFile	( LPSTR lpPath, LPSTR lpName ) ;
	
	// 读取指定文件的内容
	CString SysReadFile		( LPSTR lpPath, LPSTR lpName ) ;

	// 创建子目录
	BOOL	SysCreateSubDir	( LPSTR lpPath, LPSTR lpSubDirName ) ;
	
	// 删除子目录
	BOOL	SysDeleteSubDir	( LPSTR lpPath, LPSTR lpSubDirName ) ;

	// 低级格式化
	BOOL	SysLowFormat	() ;
	
	// 高级格式化
	BOOL	SysHighFormat	() ;

public:
	// 创建文件系统内存映射
	BOOL	SysCreateMap	( DWORD dwCreateFlag ) ;
	
	// 删除文件系统的内存映射
	void	SysDeleteMap	() ;

	// 刷新映射数据到磁盘文件
	BOOL	SysFlushData	( LPVOID lpAddr, UINT nNumToFlush ) ;
} ;

#endif