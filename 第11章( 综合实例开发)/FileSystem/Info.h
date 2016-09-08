#ifndef _INFO_H
#define _INFO_H

#define SYSTEM_SPACE_NAME		"FileSystem"		// 指定文件系统的名称
#define CLUSTER_SIZE			4096				// 指定每个簇的大小（以字节为单位）
#define DEFAULT_SYSTEM_SPACE	( 256 * 1024 * 1024 ) // 指定文件系统的大小（以字节为单位）
#define FIRST_VALID_CLUSTER		33				// 指定首个可用簇的簇号
#define FAT_BIT16				2				// 指定FAT的型号为位
#define MAX_PATH_LENGTH			512				// 定义最长的路径名长度
#define MAX_DIR_ITEM_IN_PAGE	128				//每个文件夹的目录项不超过128个
#define DIRECTORY_ITEM_SIZE		32				// 定义文件目录项结构的大小
#define MAX_FILE_NAME_LEN		8				// 定义最长的文件名长度
#define USER_INFO_SIZE			32				// 定义用户信息结构的大小
#define MAX_USER_NAME_LEN		12				// 定义最长的用户名长度
#define MAX_USER_PWD_LEN		12				// 定义最长的密码长度

// 定义文件系统的映射结构
typedef struct {
	HANDLE	hFile ;				// 文件句柄
	HANDLE	hMapFile ;			// 内核句柄
	LPVOID	lpFileMapping ;		// 映射基地址
} SYSTEM_HANDLE ;

// 定义文件目录项结构
typedef struct _DIRECTORY_ITEM{
	BYTE	szFileName[8] ;		// 文件名
	BYTE	szFileExt[3] ;		// 扩展名
	BYTE	bAttribute ;		// 0:只读，1：可写， 2：目录
	unsigned short	nStartClu ;	// 起始簇
	UINT	nFileSize ;			// 文件大小（字节）
	BYTE	szUserName[12] ;	// 所属用户
	BYTE	bReserve[2] ;		// 保留
} DIRECTORY_ITEM ;
typedef DIRECTORY_ITEM*	PDIRECTORY_ITEM ;

// 定义用户信息结构
typedef struct _USER_INFO{
	BYTE	szUserName[12] ;	// 用户名
	BYTE	szUserPwd[12] ;		// 密码
	BYTE	bReserve[8] ;		// 保留
} USER_INFO ;
typedef USER_INFO* PUSER_INFO ;

#endif