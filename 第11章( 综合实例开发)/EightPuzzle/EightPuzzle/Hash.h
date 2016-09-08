#pragma once

#define MAX_HASH_RANGE	10000
#define MAX_CRASH_NUM	150

class CHash
{
public:
	CHash(void);
public:
	~CHash(void);

private:
	int		nCount[MAX_HASH_RANGE] ;	// 记录每一个hash值中的数据个数
	int*	pHashTable[MAX_HASH_RANGE] ;// hash表

public:
	BOOL	IsValueExist ( int nValue ) ;	// 检测是否已存在
	VOID	AddValue ( int nValue ) ;		// 添加数据
};
