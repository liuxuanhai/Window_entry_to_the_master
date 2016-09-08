
#include "stdafx.h"
#include "HuffmanSystem.h"


HuffNode::HuffNode ()
{
	this->value		= 0 ;
	this->nCount	= 0 ;
	this->isLeaf	= true ;
	this->pLeft		= this->pRight = NULL ;
}

HuffNode::HuffNode ( HuffNode* lc, HuffNode* rc )
{
	isLeaf = false ;
	nCount = lc->nCount + rc->nCount ;
	pLeft  = lc ;
	pRight = rc ;
}
bool HuffNode::IsLeafNode () 
{ 
	return isLeaf ; 
}
BYTE HuffNode::GetValue () 
{
	return value ; 
}

UINT HuffNode::GetCount () 
{ 
	return nCount ; 
}

void HuffNode::SetValue ( BYTE value  ) 
{ 
	this->value  = value ;
}

void HuffNode::SetCount ( UINT nCount ) 
{ 
	this->nCount = nCount ; 
}

void HuffNode::AddCount ( ) 
{ 
	this->nCount++ ;
}


CHuffSystem::CHuffSystem ()
{
	root = NULL ;
	Item = new HuffNode[256] ;
	CodingInBinStr = new CString[256] ;
}

CHuffSystem::~CHuffSystem ()
{
	HuffList.DeleteAll() ;
}

CString CHuffSystem::TextHuffEncoding ( CString szText )
{
	InitContext() ;
	UINT nStrLen = szText.GetLength() ;
	GenFreqPair ( szText.GetBuffer(nStrLen), nStrLen ) ;
	GenHuffCoding ( BuildHuffTree () ) ;
	return HuffEncoding ( (PBYTE)szText.GetBuffer(nStrLen), nStrLen ) ;
}

PBYTE CHuffSystem::TextHuffDecoding ( CString szBinStr )
{
	LPBYTE lpStr = HuffDecoding ( szBinStr, szBinStr.GetLength() ) ;
	return lpStr ;
}

bool CHuffSystem::FileHuffEncoding ( CString szFileName )
{
	InitContext() ;

	// Generate frequence table from file
	if ( !GenFreqPairByFile ( szFileName ) )
		return false;

	// Create target coding file kernel object
	HANDLE	hTarFile = CreateFile ( szFileName+".huf", GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if ( hTarFile == INVALID_HANDLE_VALUE )
		return false ;

	DWORD	dwHeadSize ;
	if ( ( dwHeadSize = GenHuffFileHead(hTarFile) ) == 0 )
	{
		CloseHandle ( hTarFile ) ;
		return false ;
	}

	// Coding for file
	DWORD	dwCodingLen ;
	if ( ( dwCodingLen = HuffEncodingByFile(hTarFile,szFileName) ) == 0 )
	{
		CloseHandle ( hTarFile ) ;
		return false ;
	}

	DWORD	dwWritten ;
	SetFilePointer ( hTarFile, dwHeadSize-4, 0, FILE_BEGIN ) ;
	WriteFile ( hTarFile, (PBYTE)(&dwCodingLen), 4, &dwWritten, NULL ) ;

	CloseHandle ( hTarFile )  ;
	return true ;
}

bool CHuffSystem::FileHuffDecoding ( CString szFileName )
{
	InitContext () ;

	HANDLE	hFile, hFileMap ;
	LPVOID	pMapView ;
	DWORD	dwTemp = 0, dwPace = DEFAULT_PAGE_SIZE ;
	DWORD	dwOriginalFileSize = 0, dwCodingBits = 0 ;

	// Create file kernel object
	hFile = CreateFile ( szFileName, GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if ( hFile == INVALID_HANDLE_VALUE )
		return false ;

	// Get file size
	dwLowFileSize = GetFileSize ( hFile, &dwHighFileSize ) ;

	// Create file mapping object 
	hFileMap = CreateFileMapping ( hFile, NULL, PAGE_READWRITE, 0, 0, NULL ) ;
	if ( hFileMap == NULL )
	{
		CloseHandle ( hFile ) ;
		return false ;
	}

	// Mapping file view
	pMapView = MapViewOfFile ( hFileMap, FILE_MAP_READ, 0, 0, 0 ) ;
	if ( pMapView == NULL )
	{
		CloseHandle ( hFileMap ) ;
		CloseHandle ( hFile ) ;
		return false ;
	}

	// Get frequence table from coding file which to be decoding
	PBYTE	pMap = (PBYTE)pMapView ;
	BYTE	bTemp, nByte = pMap[6] ;
	DWORD	i, j, index, nItemCount = (pMap[3]&128) ? 256 : pMap[7] ;
	for ( i = 0, index = 8;  i < nItemCount; i++ )
	{
		bTemp = pMap[index++] ;
		for ( j = 0; j < nByte; j++ )
			Item[bTemp].SetCount ( Item[bTemp].GetCount() + (pMap[index++]<<(8*j)) ) ;
	}

	// Get original file size 
	dwOriginalFileSize	= *(DWORD*)(&(pMap[index])) ;
	index += 4 ;

	// Get coding length of coded file 
	dwCodingBits		= *(DWORD*)(&(pMap[index])) ;
	index += 4 ;

	// Get begin pos of coding file 
	UINT nCodingBegPos	= pMap[4] + ( pMap[5] << 8 ) ;

	// Build huffman tree and generate huffman coding 
	GenHuffCoding ( BuildHuffTree () ) ;

	// Create target file kernel object
	szFileName.Delete ( szFileName.GetLength()-4, 4 ) ;
	HANDLE	hTarFile = CreateFile ( szFileName, GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if ( hFile == INVALID_HANDLE_VALUE )
		return false ;

	HuffNode*	p = root ;
	BYTE		nValue ;
	PBYTE		pCode = &pMap[nCodingBegPos] ;
	DWORD		pdwWritten = 0, dwDiv = 0, dwRes = 0;
	for ( index = 0; index < dwCodingBits; index++ ) 
	{
		if ( pCode[dwDiv] & ( 128 >> dwRes ) )
			p = p->pRight ;
		else 
			p = p->pLeft ;

		if ( p->IsLeafNode() )
		{
			nValue	= p->GetValue() ;
			WriteFile ( hTarFile, &nValue, 1, &pdwWritten, NULL ) ;
			p = root ;
		}

		if ( (++dwRes) == 8 )
		{
			dwRes = 0; 
			dwDiv++ ;
		}
	}

	CloseHandle ( hTarFile ) ;
	UnmapViewOfFile ( pMapView ) ;
	CloseHandle ( hFileMap ) ;
	CloseHandle ( hFile ) ;
	return true ;
}

UINT CHuffSystem::GenHuffFileHead ( HANDLE hFile )
{
	UINT	i, j, nTempMax = 0, nItemNum = 0 ;
	BYTE	nCountSizeFlag ;
	for ( i = 0; i < 256; i++ )
	{
		if ( Item[i].GetCount() > nTempMax )
			nTempMax = Item[i].GetCount() ;
		if ( Item[i].GetCount() > 0 )
			nItemNum++ ;
	}

	if ( nTempMax <= 0xff )
		nCountSizeFlag = 1 ;
	else if ( nTempMax <= 0xffff )
		nCountSizeFlag = 2 ;
	else if ( nTempMax <= 0xffffff )
		nCountSizeFlag = 3 ;
	else
		nCountSizeFlag = 4 ;


	BYTE HeadInfo[MAX_HEAD_SIZE] = "HUF.";
	if ( nItemNum == 256 )
	{
		nItemNum = 0; 
		HeadInfo[3] += 128 ;
	}
	HeadInfo[6] = nCountSizeFlag ;
	HeadInfo[7] = nItemNum ;

	UINT nTemp , nByteCount = 8 ;
	for ( i = 0; i < 256; i++ )
	{
		if ( Item[i].GetCount() > 0 )
		{
			HeadInfo[nByteCount++] = (byte)i ;
			nTemp = Item[i].GetCount() ;
			for ( j = 0; j < nCountSizeFlag; j++, nTemp >>= 8 )
				HeadInfo[nByteCount++] = ( nTemp & 0xff ) ;
		}
	}

	// Append original file size inform
	nTemp = dwLowFileSize ;
	for ( j = 0; j < 4; j++, nTemp >>= 8 )
		HeadInfo[nByteCount++] = ( nTemp & 0xff ) ;

	// Reserve 4 bytes for restoring Coding length
	nByteCount += 4 ;

	// Append Target file header length
	HeadInfo[4] = (BYTE)( nByteCount & 0xff ) ;
	HeadInfo[5] = (BYTE)( nByteCount >> 8 ) ;

	// Write new file header to target file 
	SetFilePointer ( hFile, 0, 0, FILE_END ) ;
	DWORD	pdwWritten = 0 ;
	WriteFile ( hFile, HeadInfo, nByteCount, &pdwWritten, NULL ) ;

	return pdwWritten ;
}

void CHuffSystem::InitContext ()
{
	HuffList.DeleteAll() ;
	//DeleteHuffTree ( root ) ;
	for ( UINT i = 0; i < 256; i++ )
	{
		Item[i].SetValue ( i ) ;
		Item[i].SetCount ( 0 ) ;
		CodingInBinStr[i] = "" ;
	}
}

// Get frequence of each byte
void CHuffSystem::GenFreqPair ( LPVOID lpByte, UINT nSize ) 
{
	UINT i ;
	for ( i = 0; i < nSize; i++ )
	{
		Item[*((PBYTE)lpByte)].AddCount() ;
		lpByte = (LPVOID)((BYTE*)lpByte + 1 ) ;
	}
}

// Get frequence of each byte of file 
bool CHuffSystem::GenFreqPairByFile ( CString szFileName )
{
	HANDLE	hFile, hFileMap ;
	LPVOID	pMapView ;
	DWORD	dwTemp = 0, dwPace = DEFAULT_PAGE_SIZE ;

	// Create file kernel object
	hFile = CreateFile ( szFileName, GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if ( hFile == INVALID_HANDLE_VALUE )
		return false;

	// Get file size
	dwLowFileSize = GetFileSize ( hFile, &dwHighFileSize ) ;

	// Create file mapping object 
	hFileMap = CreateFileMapping ( hFile, NULL, PAGE_READWRITE, 0, 0, NULL ) ;
	if ( hFileMap == NULL )
	{
		CloseHandle ( hFile ) ;
		return false ;
	}

	while ( dwTemp < dwLowFileSize )
	{
		if ( dwLowFileSize - dwTemp < dwPace )
			dwPace = dwLowFileSize - dwTemp ;

		// Mapping file view
		pMapView = MapViewOfFile ( hFileMap, FILE_MAP_READ, 0, dwTemp, dwPace ) ;
		if ( pMapView == NULL )
		{
			CloseHandle ( hFileMap ) ;
			CloseHandle ( hFile ) ;
			return false ;
		}
		else
		{
			GenFreqPair ( pMapView, dwPace ) ;
			UnmapViewOfFile ( pMapView ) ;
			dwTemp += dwPace ;
		}
	}

	CloseHandle ( hFileMap ) ;
	CloseHandle ( hFile ) ;
	return true ;
}

// Build Huffman tree
HuffNode* CHuffSystem::BuildHuffTree ()
{
	BuildSortList() ;
	Node<HuffNode>* p = HuffList.pList ;

	while ( p->pNext )
	{
		HuffNode pIntlNode ( &(p->Item), &(p->pNext->Item) ) ;
		InsertNodeInSort ( pIntlNode ) ;
		p = p->pNext->pNext ;
	}	

	root = &(p->Item) ;
	return root ;
}

// Delete Huffman tree 
void CHuffSystem::DeleteHuffTree ( HuffNode* SubTree )
{
	if ( SubTree == NULL )
		return ;
	if ( SubTree->pLeft != NULL )
	{
		DeleteHuffTree ( SubTree->pLeft ) ;
		SubTree = NULL ;
	}
	if ( SubTree->pRight != NULL )
	{
		DeleteHuffTree ( SubTree->pRight ) ;
		SubTree = NULL ;
	}
	delete SubTree ;
}

// Generate Huffman coding
void CHuffSystem::GenHuffCoding ( HuffNode* root, CString code )
{
	if ( root->IsLeafNode() )
	{
		CodingInBinStr[root->GetValue()] = code ;
		return ;
	}
	else
	{
		GenHuffCoding ( root->pLeft,  code+"0" ) ;
		GenHuffCoding ( root->pRight, code+"1" ) ;
	}
}

// Encoding produce
CString CHuffSystem::HuffEncoding ( PBYTE pStr, UINT nStrLen )
{
	CString TempBinStr = "" ;

	for ( UINT i = 0; i < nStrLen; i++ )
		TempBinStr += CodingInBinStr[pStr[i]] ;

	return TempBinStr ;
}

// Decoding produce
PBYTE CHuffSystem::HuffDecoding ( CString szText, UINT uStrLen )
{
	UINT nByteCount = 0;
	PBYTE TempByte = new BYTE[DEFAULT_PAGE_SIZE] ;

	HuffNode* p = root ;
	for ( UINT i = 0; i < uStrLen ;  )
	{
		p = root ;
		while ( !(p->IsLeafNode()) && i < uStrLen  )
		{
			if ( szText.GetAt(i++) == '0' )
				p = p->pLeft ;
			else
				p = p->pRight ;
		}
		TempByte[nByteCount++] = p->GetValue() ;
	}
	return TempByte ;
}

// Encoding produce ( file )
UINT CHuffSystem::HuffEncodingByFile ( HANDLE hTarFile, CString szFileName )
{
	HANDLE	hFile, hFileMap ;
	PBYTE	pMapView ;

	// Create file kernel object
	hFile = CreateFile ( szFileName, GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if ( hFile == INVALID_HANDLE_VALUE )
		return 0;

	// Create file mapping object 
	hFileMap = CreateFileMapping ( hFile, NULL, PAGE_READWRITE, 0, 0, NULL ) ;
	if ( hFileMap == NULL )
	{
		CloseHandle ( hFile ) ;
		return 0 ;
	}

	GenHuffCoding ( BuildHuffTree () ) ;

	// Mapping file view
	pMapView = (PBYTE)MapViewOfFile ( hFileMap, FILE_MAP_WRITE, 0, 0, 0 ) ;
	if ( pMapView == NULL )
	{
		CloseHandle ( hFileMap ) ;
		CloseHandle ( hFile ) ;
		return 0 ;
	}

	DWORD	dwWritten ;
	BYTE	nValue = 0 ;
	UINT	i, j, nTemp = 128,nCurLen, nStrIndex, nBits = 0 ; 
	for ( i = 0; i < dwLowFileSize; i++ )
	{
		nStrIndex	= pMapView[i] ;
		nCurLen		= CodingInBinStr[nStrIndex].GetLength() ;
		nBits		+= nCurLen ;
		for ( j = 0; j < nCurLen; j++ )
		{
			if ( CodingInBinStr[nStrIndex].GetAt(j) == '1' )
				nValue += nTemp ;
			nTemp >>= 1 ;
			if ( nTemp == 0 ||
				(nValue && i == dwLowFileSize-1 && j == nCurLen-1) )
			{
				WriteFile ( hTarFile, &nValue, 1, &dwWritten, NULL ) ;
				nValue	= 0 ;
				nTemp	= 128 ;
			}
		}
	}

	UnmapViewOfFile ( pMapView ) ;
	CloseHandle ( hFileMap ) ;
	CloseHandle ( hFile )  ;

	return nBits ;
}

bool CHuffSystem::WriteConToNewFile ( CString szFileName, BYTE pbNewFile[], DWORD dwNewFileSize, bool flag ) 
{
	if ( flag )		// Encoding
		szFileName += ".huf" ;
	else			//Decoding
		szFileName.Delete ( szFileName.GetLength()-4, 4 ) ;

	// Create file kernel object
	HANDLE	hFile = CreateFile ( szFileName, GENERIC_READ|GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if ( hFile == INVALID_HANDLE_VALUE )
		return false ;

	DWORD pdwWritten = 0 ;
	WriteFile ( hFile, pbNewFile, dwNewFileSize, &pdwWritten, NULL ) ;

	CloseHandle ( hFile )  ;
	return true ;
}

// Build sorted list by using FreqPairs
void CHuffSystem::BuildSortList()
{
	for ( UINT i = 0; i < 256; i++ )
	{
		if ( this->Item[i].GetCount() == 0 )
			continue ;
		InsertNodeInSort ( this->Item[i] ) ;
	}
}

// Insert Huffman tree's internal node to list by sort
void CHuffSystem::InsertNodeInSort ( HuffNode NewNode )
{
	Node<HuffNode>* pHead = HuffList.pList ;
	Node<HuffNode>* pTail = HuffList.pTail ;

	if ( !pTail )
		HuffList.InsertNullList ( NewNode ) ;
	else if ( NewNode.GetCount() < pHead->Item.GetCount() )
		HuffList.InsertInHead ( NewNode ) ;
	else if ( NewNode.GetCount() >= pTail->Item.GetCount() )
		HuffList.InsertInTail ( NewNode ) ;
	else
	{
		while ( pHead != pTail )
		{
			if ( NewNode.GetCount() < pHead->Item.GetCount() )
				break ;
			pHead = pHead->pNext ;
		}
		HuffList.InsertInIntl ( pHead, NewNode ) ;
	}
}

CString CHuffSystem::ConvertByteArrayToBinStr ( PBYTE pBeg, UINT nBits ) 
{
	CString TarStr = "" ;
	UINT i, div, index ;
	for ( i = 0; i < nBits; i++ )
	{
		div = i >> 3; index = 7 - ( i & 0x7 ) ;
		if ( pBeg[div] & ( 1 << index ) )
			TarStr += "1" ;
		else
			TarStr += "0" ;
	}
	return TarStr ;
}