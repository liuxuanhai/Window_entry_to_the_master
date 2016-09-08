#pragma once

#include "Data.h"

#define MAX_THREAD_NUM	8
#define	MAX_PIPE_NUM	128
#define PIPE_NAME		L"\\\\.\\Pipe\\SockDataInfo"

typedef struct {
	HANDLE		hPipe ;
	OVERLAPPED	ov ;
} PIPE_INSTRUCT;


class CPipeServer
{
public:
	CPipeServer(void);
public:
	~CPipeServer(void);

private:
	BOOL			bServerStatus ;
	HANDLE			hCompletionPort ;
	HANDLE			hThread[MAX_THREAD_NUM] ;
	PIPE_INSTRUCT	PipeInst[MAX_PIPE_NUM] ;

	CTreeCtrl*		pSockTree ;
	HTREEITEM		hTreeRoot ;

public:
	BOOL	GetServerStatus () ;
	BOOL	StartServer ( CTreeCtrl* pTree = NULL ) ;
	VOID	StopServer () ;	
	friend	UINT ServerThread ( LPVOID lpParameter ) ;
};
