#include "stdafx.h"
#include "AStar.h"

#include "BinaryHeap.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std ;

#include <stdlib.h>
#include <time.h>

int CUR_LEN = 0 ;

CBinaryHeap OPEN ;
int cost[8] = { 10, 14, 10, 14, 14, 10, 14, 10 } ;
int dir[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{-1,-1},{0,-1},{1,-1},{1,0}} ;
char dir_ch[8][3] = { "↑","↗","→","↘","↖","←","↙","↓" } ;
char map_ch [MAX_LEN][MAX_LEN] = {0} ;
bool GenMap[MAX_LEN][MAX_LEN] = {0} ;
int  MapSize, MapRate ;

inline bool IsValidPos ( int x, int y )
{
	if ( x < 0 || x >= CUR_LEN )
		return false ;
	if ( y < 0 || y >= CUR_LEN )
		return false ;
	return true ;
}

bool AStar_GenerateMap ()
{
	srand ( time(NULL ) ) ;
	ofstream fout ( "map.in" ) ;

	int len = MapSize ;
	int num = len * len * MapRate / 100 ;
	fout << len << endl ;

	memset ( GenMap, 0x1, sizeof(GenMap) ) ;

	int i, j ;
	for ( i = 0; i < num; i++ )
	{
		int x = rand () % len ;
		int y = rand () % len ;
		if ( x == len-1 && y == len-1 )
			continue ;
		GenMap[x][y] = 0 ;
	}

	GenMap[0][0] = GenMap[len-1][len-1] = 1 ;

	for ( i = 0; i < len; i++ )
	{
		for ( j = 0; j < len; j++ )
			fout << (GenMap[i][j] ? '.' :'*') ;
		fout << endl ;
	}

	fout.close () ;
	return true ;
}

int AStar_InitialMap ()
{
	memset ( map, 0, sizeof(map) ) ;
	memset ( map_ch, 0, sizeof(map_ch) ) ;

	ifstream fin ( "map.in" ) ;
	if ( fin.is_open() == false )
		return 0 ;

	int i, j ;
	fin >> CUR_LEN ;

	for ( i = 0; i < CUR_LEN; i++ )
		fin >> map_ch[i] ;
	
	for ( i = 0; i < CUR_LEN; i++ )
	{
		for ( j = 0; j < CUR_LEN; j++ )
		{
			map[i][j].type = ( map_ch[i][j] == '.' ? 1 : 0 ) ;
			map[i][j].dir_parent = 9 ;
		}
	}

	fin.close () ;
	return CUR_LEN ;
}

bool  AStar_GetShortestPath ()
{
	int lx = CUR_LEN, ly = CUR_LEN ;
	int nStartX = 0, nStartY = 0 ;
	int nEndX = lx-1, nEndY = ly-1 ;

	OPEN.BHAddNew ( nStartX, nStartY, 3 ) ;
	map[nStartX][nStartY].status = 1 ;
	map[nStartX][nStartY].index = 0 ;

	while ( OPEN.BHGetNum() > 0 )
	{
		NODE TopNode = OPEN.BHGetTop () ;
		if ( TopNode.x == nEndX && TopNode.y == nEndY )
			return true ;

		map[TopNode.x][TopNode.y].status = 2 ;
		OPEN.BHDelTop () ;

		for ( int i = 0; i < 8; i++ )
		{
			int a = TopNode.x + dir[i][0] ;
			int b = TopNode.y + dir[i][1] ;
			if ( IsValidPos(a,b) == false || map[a][b].type == 0 )
				continue ;

			// 计算当前消费g(n)
			int cur_cost = map[TopNode.x][TopNode.y].g + cost[i] ;

			// 若该结点首次被访问
			if ( map[a][b].status == 0 )
			{
				map[a][b].status = 1 ;
				map[a][b].dir_parent = 7 - i ;
				map[a][b].g = cur_cost ;
				map[a][b].h = ( nEndX - a ) + ( nEndY - b ) ;	// 只在首次访问时估价h(n)
				OPEN.BHAddNew ( a, b, map[a][b].g + map[a][b].h ) ;
			}
			else if ( cur_cost < map[a][b].g )
			{
				map[a][b].dir_parent = 7-i ;
				map[a][b].g = cur_cost ;
				if ( map[a][b].status == 1 )
				{
					OPEN.BHAdjustByIndex ( map[a][b].index, map[a][b].g + map[a][b].h ) ;
				}
				else if ( map[a][b].status == 2 )
				{
					OPEN.BHAddNew ( a, b, map[a][b].g + map[a][b].h ) ;		
				}
				map[a][b].status = 1 ;
			}
		} // for
	} // while 
	
	return false ;
}

bool AStar_Output ()
{
	ofstream fout ( "map.out" ) ;

	int sx = CUR_LEN-1, sy = CUR_LEN-1, cur_dir = 0 ;
	while ( true ) 
	{
		cur_dir = map[sx][sy].dir_parent ;
		sx += dir[cur_dir][0] ;
		sy += dir[cur_dir][1] ;
		if ( IsValidPos ( sx, sy ) == false )
		{
			return false ;
		}

		map_ch[sx][sy] = 7-cur_dir ;
		if ( sx == 0 && sy == 0 )
			break ;
	}

	for ( int i = 0; i < CUR_LEN; i++ )
	{
		for ( int j = 0; j < CUR_LEN; j++ )
		{
			if ( map_ch[i][j] < 8 )
				fout << dir_ch[map_ch[i][j]] ;
			else
				fout << setw(2) << map_ch[i][j] ;
		}
		fout << endl ;
	}
	fout.close () ;

	return true ;
}

void AStar_SetMapParam ( int size, int rate )
{
	MapSize = size ;
	MapRate = rate ;
}

//
//void TempOutput ()
//{
//	int i, j ;
//	for ( i = 0; i < CUR_LEN; i++ )
//	{
//		for ( j = 0; j < CUR_LEN; j++ )
//		{
//			cout << map[i][j].dir_parent ;
//		}
//		cout << endl ;
//	}
//}

//int _tmain(int argc, _TCHAR* argv[])
//{
//	AStar_GenerateMap () ;
//
//	AStar_InitialMap () ;
//
//	int a = clock () ;
//	bool ret = AStar_GetShortestPath () ;
//	int b = clock () ;
//	cout << "cost = " << b - a << endl ;
//
//	//TempOutput () ;
//	if ( ret == true )
//		AStar_Output () ;
//	else
//		cout << "no path " << endl ;
//	return 0;
//}
