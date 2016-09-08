#pragma once

#include <stdio.h>
#include <string.h>

#define MAX_HEAP_LEN 1000000
#define MAX_LEN		1000

typedef struct _MAP {
	int type ;			// 0:可行 1:障碍 2:起点 3:终点
	int status ;		// 0:未访问 1:OPEN 2:CLOSE
	int dir_parent ;	// dir ( 1 -- 8 )
	int g ;				// g(n)
	int h ;				// h(n)
	int index ;			// 在heap中的结点索引号
} MAP ;

MAP map[MAX_LEN][MAX_LEN] = {0} ;

typedef struct _NODE {
	int x, y ;
	int value ;
	struct _NODE () {
		x = y = value = 0 ;
	}
	struct _NODE ( int nx, int ny, int nv ) {
		x = nx ; y = ny ; value = nv ;
	}
	void operator = ( struct _NODE& node ) {
		x = node.x ;
		y = node.y ;
		value = node.value ;
	}
} NODE, *PNODE ;

class CBinaryHeap {
public:
	CBinaryHeap ()
	{
		this->pHeap = NULL ;
		this->BHReset () ;
	}
	~CBinaryHeap ()
	{
		delete pHeap ;
	}

public:
	void BHReset ()
	{
		count = 0 ;
		if ( pHeap == NULL )
			pHeap = new NODE[MAX_HEAP_LEN] ;
		else
			memset ( pHeap, 0, sizeof(pHeap) ) ;
	}

	bool BHAddNew ( int x, int y, int value )
	{
		NODE node ( x, y, value ) ;
		this->pHeap[this->count++] = node ;
		this->BHFilterUp ( this->count-1 ) ;
		return true ;
	}

	bool BHDelTop ()
	{
		if ( this->count <= 0 )
			return false ;

		this->count-- ;
		this->pHeap[0] = this->pHeap[this->count] ;
		this->BHFilterDown ( 0 ) ;
		return true ;
	}

	bool BHAdjustByIndex ( int index, int nNewValue )
	{
		this->pHeap[index].value = nNewValue ;
		if ( this->BHFilterDown ( index ) == index )
			this->BHFilterUp ( index ) ;
		return true ;
	}

	bool BHGetNum ()
	{
		return this->count ;
	}

	NODE& BHGetTop ()
	{
		return this->pHeap[0] ;
	}

private:
	int		count ;
	PNODE	pHeap ;		

private:
	int BHFilterDown ( int index )
	{
		NODE TempNode = this->pHeap[index] ;
		int cur = index ;
		int son = 2 * cur + 1 ;
		while ( son < this->count )
		{
			if ( son+1 < this->count && this->pHeap[son].value > this->pHeap[son+1].value )
				son++ ;

			if ( TempNode.value < this->pHeap[son].value )
				break ;
			else
			{
				this->pHeap[cur] = this->pHeap[son] ;
				this->BHUpdatePos ( this->pHeap[son].x, this->pHeap[son].y, cur ) ;
				cur = son ;
				son = 2 * cur + 1 ;
			}
		}
		this->pHeap[cur] = TempNode ;
		this->BHUpdatePos ( TempNode.x, TempNode.y, son ) ;
		return son ;
	}

	int BHFilterUp ( int index )
	{
		NODE TempNode = this->pHeap[index] ;
		int cur = index ;
		int parent = ( cur - 1 ) / 2 ;
		while ( cur != 0 )
		{
			if ( this->pHeap[parent].value <= TempNode.value )
				break ;

			this->pHeap[cur] = this->pHeap[parent] ;
			this->BHUpdatePos ( this->pHeap[parent].x, this->pHeap[parent].y, cur ) ;
			cur = parent ;
			parent = ( cur - 1 ) / 2 ;
		}
		this->pHeap[cur] = TempNode ;
		this->BHUpdatePos ( TempNode.x, TempNode.y, cur ) ;
		return cur ;
	}

	void BHUpdatePos ( int x, int y, int pos )
	{
		map[x][y].index = pos ;
	}
} ;

