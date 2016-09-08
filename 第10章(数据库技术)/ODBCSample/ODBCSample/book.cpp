// book.h : CBook 类的实现



// CBook 实现

// 代码生成在 2007年12月15日, 19:29

#include "stdafx.h"
#include "book.h"
IMPLEMENT_DYNAMIC(CBook, CRecordset)

CBook::CBook(CDatabase* pdb)
	: CRecordset(pdb)
{
	column1 = 0;
	m_BookName = L"";
	m_Author = L"";
	m_Publish = L"";
	m_Price = 0;
	m_nFields = 5;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含明文密码和/或其他重要
// 信息。请在查看完此连接字符串并找到所有与安全
// 有关的问题后移除 #error。可能需要将此密码存
// 储为其他格式或使用其他的用户身份验证。
CString CBook::GetDefaultConnect()
{
	return _T("DSN=book;UID=Admin;") ;
	//return _T("DSN=book;DBQ=G:\\VC\x8f6f\x4ef6\x5f00\x53d1\\\x5b9e\x4f8b\x6e90\x7801\\\x7b2c11\x7ae0\\ODBCSample\\release\\book.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString CBook::GetDefaultSQL()
{
	return _T("[book]");
}

void CBook::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[编号]"), column1);
	RFX_Text(pFX, _T("[BookName]"), m_BookName);
	RFX_Text(pFX, _T("[Author]"), m_Author);
	RFX_Text(pFX, _T("[Publish]"), m_Publish);
	RFX_Long(pFX, _T("[Price]"), m_Price);

}
/////////////////////////////////////////////////////////////////////////////
// CBook 诊断

#ifdef _DEBUG
void CBook::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBook::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


