

#pragma once

#ifndef _MOMORY_H
#include <memory.h>
#endif

#include "CList.h"

#define DEFAULT_PAGE_SIZE ( 2 << 16 )
const int MAX_HEAD_SIZE = 1500 ;

class HuffNode
{
private:
	BYTE		value ;		// 结点ASCII值
	UINT		nCount ;	// 权值，频率统计
	bool 		isLeaf ;	// 是否叶子结点

public:
	HuffNode*	pLeft ;		// 指向左子树
	HuffNode*	pRight ;	// 指向右子树

public:
	// 默认构造函数
	HuffNode () ;
	// 根据左右子树创建根结点
	HuffNode ( HuffNode* lc, HuffNode* rc ) ;
	// 检测是否叶子结点
	bool IsLeafNode ()  ;
	// 取得结点ASCII值
	BYTE GetValue () ;
	// 取得权值
	UINT GetCount () ;
	// 设置ASCII
	void SetValue ( BYTE value  ) ;
	// 设置权值
	void SetCount ( UINT nCount ) ;
	// 添加记数
	void AddCount ( ) ;
} ;


class CHuffSystem
{
public:
	CHuffSystem () ;
	~CHuffSystem () ;

private:
	CCList<HuffNode>	HuffList ;		// 结点列表
	HuffNode*			root ;			// 二叉链表根结点

public:
	HuffNode*			Item ;			// 结点信息
	CString*			CodingInBinStr ;// 保存字节编码串
	DWORD				dwHighFileSize ;// 文件编/解码时文件大小高32位
	DWORD				dwLowFileSize ;	// 文件编/解码时文件大小低32位

public:
	// 字符串编码
	CString TextHuffEncoding ( CString szText ) ;
	// 字符串解码
	PBYTE TextHuffDecoding ( CString szBinStr ) ;
	// 文件编码
	bool FileHuffEncoding ( CString szFileName ) ;
	// 文件解码
	bool FileHuffDecoding ( CString szFileName ) ;
	// 产生文件头
	UINT GenHuffFileHead ( HANDLE hFile ) ;

protected:
	// 初始化编码
	void InitContext () ;
	// 统计字节出现频率
	void GenFreqPair ( LPVOID lpByte, UINT nSize ) ;
	// 统计文件字节出现频率
	bool GenFreqPairByFile ( CString szFileName ) ;
	// 建立Huffman编码树
	HuffNode* BuildHuffTree () ;
	// 删除Huffman编码树
	void DeleteHuffTree ( HuffNode* SubTree );
	// 根据指定的树进行编码
	void GenHuffCoding ( HuffNode* root, CString code = "" );

protected:
	// 编码过程
	CString HuffEncoding ( PBYTE pStr, UINT nStrLen );
	// 解码过程
	PBYTE HuffDecoding ( CString szText, UINT uStrLen );
	// 文件编码过程
	UINT HuffEncodingByFile ( HANDLE hTarFile, CString szFileName );
	// 写入新文件
	bool WriteConToNewFile ( CString szFileName, BYTE pbNewFile[], DWORD dwNewFileSize, bool flag );

protected:
	// 根据频率排序
	void BuildSortList();
	// 在链表中有序地插入结点
	void InsertNodeInSort ( HuffNode NewNode );

public:
	// 把字节串转化为二进制串
	CString ConvertByteArrayToBinStr ( PBYTE pBeg, UINT nBits );
} ;
