// Ping.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Winsock2.h"
#pragma comment ( lib, "ws2_32.lib" )

#include <windows.h>
#include "stdio.h"

// 定义默认的缓冲区长度
#define DEF_BUF_SIZE		1024

// 定义IP包头长度
#define IP_HEADER_SIZE		20

// 定义ICMP包头长度
#define ICMP_HEADER_SIZE	( sizeof(ICMP_HEADER) )

// 定义ICMP包实体数据长度
#define ICMP_DATA_SIZE		32

// 定义ICMP包总长度
#define ICMP_PACK_SIZE		( ICMP_HEADER_SIZE + ICMP_DATA_SIZE )

// 定义ICMP包头
typedef struct _ICMP_HEADER {
	BYTE	bType ;			// 类型
	BYTE	bCode ;			// 代码
	USHORT	nCheckSum ;		// 校验和
	USHORT	nId ;			// 标识，本进程ID
	USHORT	nSequence ;		// 序列号
	UINT	nTimeStamp ;	// 可选项，这里为时间，用于计算时间
} ICMP_HEADER, *PICMP_HEADER ;

// 用于格式化输出信息的缓冲区
char szInfo[DEF_BUF_SIZE] ;

BOOL Ping ( char* lpDestIp ) ;
USHORT GetCheckSum ( LPBYTE lpBuf, DWORD dwSize ) ;

int main(int argc, char* argv[])
{
	int value[4] ;
	char szDestIp[DEF_BUF_SIZE] = {0} ;
	while ( scanf ( "%s", szDestIp) )
		Ping ( szDestIp ) ;
	return 0;
}

// 实现PING程序
// 参数：目标地址IP字符串，格式如“127.0.0.1”
BOOL Ping ( char* lpDestIp )
{
	// 设置目标地址
	SOCKADDR_IN DestSockAddr ;
	DestSockAddr.sin_family = AF_INET ;
	DestSockAddr.sin_addr.s_addr = inet_addr( lpDestIp ) ;
	DestSockAddr.sin_port = htons ( 0 ) ;

	// 创建ICMP回显请求包
	char ICMPPack[ICMP_PACK_SIZE] = {0} ;
	PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack ;
	pICMPHeader->bType = 8 ;
	pICMPHeader->bCode = 0 ;
	pICMPHeader->nId = (USHORT)::GetCurrentProcessId() ;
	pICMPHeader->nCheckSum = 0 ;
	pICMPHeader->nTimeStamp = 0 ;
	memset ( &(ICMPPack[ICMP_HEADER_SIZE]), 'E', ICMP_DATA_SIZE ) ;	// 填充数据部分，内容任意

	// 初始化WinSock库
	WORD wVersionRequested = MAKEWORD( 2, 2 );
	WSADATA wsaData;
	if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
		return FALSE ;

	// 创建原始套接字
	SOCKET	RawSock = socket ( AF_INET, SOCK_RAW, IPPROTO_ICMP ) ;
	if ( RawSock == INVALID_SOCKET )
	{
		printf ( "Create socket error!\n" ) ;
		return FALSE ;
	}
	
	// 设置接收超时为1秒
	int nTime = 1000 ;
	int ret = ::setsockopt ( RawSock, SOL_SOCKET,SO_RCVTIMEO, (char*)&nTime, sizeof(nTime));
	
	char szRecvBuf [ DEF_BUF_SIZE] ;
	SOCKADDR_IN	SourSockAddr ;
	for ( int i = 0 ; i < 4; i++ )
	{
		pICMPHeader->nCheckSum = 0 ;				// 初始时校验值为0
		pICMPHeader->nSequence = i ;				// 序号
		pICMPHeader->nTimeStamp = ::GetTickCount() ;// 当前时间

		// 计算校验值，校验值要在ICMP数据报创建完才能计算
		pICMPHeader->nCheckSum = GetCheckSum ( (LPBYTE)ICMPPack, ICMP_PACK_SIZE ) ;
		
		// 发送ICMP数据包
		int nRet = ::sendto ( RawSock, ICMPPack, ICMP_PACK_SIZE,0,(SOCKADDR*)&DestSockAddr, sizeof(DestSockAddr) ) ;
		if ( nRet == SOCKET_ERROR )
		{
			printf ( "sendto error!\n" ) ;
			return FALSE ;
		}

		// 接收ICMP响应
		int nLen = sizeof(SourSockAddr) ;
		nRet = ::recvfrom ( RawSock, szRecvBuf, DEF_BUF_SIZE,0,(SOCKADDR*)&SourSockAddr, &nLen ) ;
		if ( nRet == SOCKET_ERROR )
		{
			if ( ::WSAGetLastError() == WSAETIMEDOUT )
			{
				printf ( "Request Timeout\n" ) ;
				continue ;
			}
			else
			{
				printf ( "recvfrom error!\n" ) ;
				return FALSE ;
			}
		}

		// 计算ICMP数据报的时间差
		int nTime = ::GetTickCount() - pICMPHeader->nTimeStamp ;

		int nRealSize = nRet - IP_HEADER_SIZE - ICMP_HEADER_SIZE ;
		if ( nRealSize < 0  )
		{
			printf ( "To less recv bytes!\n" ) ;
			continue ;
		}

		// 检测是否当前所发出的ICMP响应包
		PICMP_HEADER pRecvHeader = (PICMP_HEADER)(szRecvBuf+IP_HEADER_SIZE) ;
		if ( pRecvHeader->bType != 0 )
		{
			printf ( "Not ICMP respond type!\n" ) ;
			return FALSE ;
		}

		if ( pRecvHeader->nId != ::GetCurrentProcessId () )
		{
			printf ( "not valid id!\n" ) ;
			return FALSE ;
		}

		printf ( "%d bytes replay from %s : bytes=%d time=%dms\n", \
			nRet, inet_ntoa(SourSockAddr.sin_addr), nRealSize, nTime ) ;

		::Sleep ( 1000 ) ;
	}
	
	closesocket ( RawSock ) ;
	WSACleanup () ;

	return TRUE ;
}

// 计算ICMP包校验值
// 参数1：ICMP包缓冲区
// 参数2：ICMP包长度
USHORT GetCheckSum ( LPBYTE lpBuf, DWORD dwSize )
{
	DWORD	dwCheckSum = 0 ;
	USHORT* lpWord = (USHORT*)lpBuf ;

	// 累加
	while ( dwSize > 1 )
	{
		dwCheckSum += *lpWord++ ;
		dwSize -= 2 ;
	}

	// 如果长度是奇数
	if ( dwSize == 1 )
		dwCheckSum += *((LPBYTE)lpWord) ;

	// 高16位和低16位相加
	dwCheckSum = ( dwCheckSum >> 16 ) + ( dwCheckSum & 0xFFFF ) ;

	// 取反
	return (USHORT)(~dwCheckSum ) ;
}
