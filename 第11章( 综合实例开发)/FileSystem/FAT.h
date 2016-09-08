#ifndef _FAT_H
#define _FAT_H

#include "Info.h"

class CFAT {
public:
	UINT	nCluPos ;	// 最后分配的空闲簇号
	LPVOID	lpBase ;	// 文件系统映射基地址

public:
	CFAT	() ;
	~CFAT	();

	// 取得一个空闲簇
	UINT	GetIdleCluster	();

	// 设置指定簇为空闲
	void	SetIdleCluster	( UINT nCluIndex ) ;

	// 取得指定簇的后继簇
	UINT	GetNextCluster	( UINT nCluIndex ) ;
	
	// 设置指定簇的后继簇
	void	SetNextCluster	( UINT nCluIndex, UINT nNewClundex ) ;

public:
	// 取得指定簇的地址
	UINT	GetAddByIndex	( UINT nCluIndex ) ;

	// 取得指定簇的标识地址（即在FAT中的位置）
	UINT	GetCluPointer	( UINT nCluIndex ) ;

	// 判断该簇是否为尾簇可（即无后继簇）
	BOOL	IsTailCluster	( UINT nCluIndex ) ;

	// 取得指定路径的目录簇
	UINT	GetCluIndexByPathName ( LPSTR lpPathName ) ;
};

#endif