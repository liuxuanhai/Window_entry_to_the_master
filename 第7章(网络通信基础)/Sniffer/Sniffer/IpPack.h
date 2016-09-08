
#include <windows.h>

typedef struct _PACK_INFO {
	USHORT	nLength ;			// 数据包长度
	USHORT	nProtocol ;			// 协议类型
	UINT	nSourIp ;			// 源IP
	UINT	nDestIp ;			// 目的IP
	USHORT	nSourPort ;			// 源端口号
	USHORT	nDestPort ;			// 目的端口号
} PACK_INFO, *LPPACK_INFO ;

typedef struct _IP_HEADER {
	BYTE	bVerAndHLen ;		// 版本信息（前4位）和头长度（后4位）
	BYTE	bTypeOfService ;	// 服务类型
	USHORT	nTotalLength ;		// 数据包长度
	USHORT	nID ;				// 数据包标识
	USHORT	nReserved ;			// 保留字段
	BYTE	bTTL ;				// 生成时间
	BYTE	bProtocol ;			// 协议类型
	USHORT	nCheckSum ;			// 校验和
	UINT	nSourIp ;			// 源IP
	UINT	nDestIp ;			// 目的IP
} IP_HEADER, *PIP_HEADER ;

typedef struct _TCP_HEADER {
	USHORT	nSourPort ;			// 源端口号
	USHORT	nDestPort ;			// 目的端口号
	UINT	nSequNum ;			// 序列号
	UINT	nAcknowledgeNum ;	// 确认号
	USHORT	nHLenAndFlag ;		// 前4位：TCP头长度；中6位：保留；后6位：标志位
	USHORT	nWindowSize ;		// 窗口大小
	USHORT	nCheckSum ;			// 检验和
	USHORT	nrgentPointer ;		// 紧急数据偏移量
} TCP_HEADER, *PTCP_HEADER ;

typedef struct _UDP_HEADER {
	USHORT	nSourPort ;			// 源端口号
	USHORT	nDestPort ;			// 目的端口号
	USHORT	nLength ;			// 数据包长度
	USHORT	nCheckSum ;			// 校验和
} UDP_HEADER, *PUDP_HEADER ;