#include "stdafx.h"
#include "StringID.h"

CONST WCHAR*				StringID::s_EmptyString = L"";
std::list<CONST WCHAR *>*	StringID::m_spStringLists = 0;
UINT64	g_ui64NumObjectInUsing = 0;

StringID::StringID()                          
{
	m_strString = s_EmptyString; 
	++g_ui64NumObjectInUsing;
}

StringID::StringID( CONST WCHAR* strString )
{
	++g_ui64NumObjectInUsing;
	m_strString = AddString( strString ); 
}

StringID::StringID( CONST StringID& other )
{
	++g_ui64NumObjectInUsing;
	m_strString = other.m_strString; 
}

StringID::~StringID()
{
	--g_ui64NumObjectInUsing;
	if( g_ui64NumObjectInUsing == 0 )
		StringID::DestoryStringTable();
}
//-----------------------------------------------------------------------------
// Name: StringID::GetStringTable
// Desc: returns static string table data- used to ensure initialization is done
//-----------------------------------------------------------------------------
std::list<CONST WCHAR *>* StringID::GetStringTable() 
{
	if(!StringID::m_spStringLists)
	{
		StringID::m_spStringLists = new std::list<CONST WCHAR *>[StringID_HASHSIZE];
	}
    return StringID::m_spStringLists;
}


void	StringID::DestoryStringTable()
{
	for( int i=0;i<StringID_HASHSIZE;++i )
	{
		std::list<CONST WCHAR*>& CurrentList = GetStringTable()[ i ];
		std::list<CONST WCHAR*>::iterator iter = CurrentList.begin();
		std::list<CONST WCHAR*>::iterator end = CurrentList.end();
		while( iter != end )
		{
			const WCHAR* strTest = *iter;
			++iter;
			delete strTest;
		}
		CurrentList.clear();
	}
	SAFE_DELETE_ARRAY(m_spStringLists);
}
//-----------------------------------------------------------------------------
// Name: StringID::operator==
// Desc: compare a string with a WCHAR 
//-----------------------------------------------------------------------------
bool StringID::operator== ( CONST WCHAR* strRHS ) CONST
{
    if( strRHS == NULL )
    {
        if( m_strString == s_EmptyString )
            return TRUE;
        return FALSE;
    }

    if( m_strString == strRHS )
        return TRUE;

    return ( wcscmp( m_strString, strRHS ) == 0 );
}

//-----------------------------------------------------------------------------
// Name: StringID::AddString 
// Desc: Add a string to the string table
//-----------------------------------------------------------------------------
CONST WCHAR* StringID::AddString( CONST WCHAR* strString )
{
    if( strString == NULL )
        return NULL;
    if( wcslen(strString) == 0 )
        return s_EmptyString;

    int uBucketIndex = HashString( strString ) % StringID_HASHSIZE;
    std::list<CONST WCHAR*>& CurrentList = GetStringTable()[ uBucketIndex ];

    std::list<CONST WCHAR*>::iterator iter = CurrentList.begin();
    std::list<CONST WCHAR*>::iterator end = CurrentList.end();

    while( iter != end )
    {
        CONST WCHAR* strTest = *iter;
        if( wcscmp( strTest, strString ) == 0 )
            return strTest;
        ++iter;
    }
    
    // $OPTIMIZE: use a fixed size allocator here
    DWORD bufferLength = wcslen( strString ) + 1;
    WCHAR* strCopy = new WCHAR[ bufferLength ];
	wcscpy(strCopy, strString);
    //wcscpy_s( strCopy, bufferLength, strString );
    CurrentList.push_back( strCopy );
    return strCopy;
}


//-----------------------------------------------------------------------------
// Name: StringID::HashString
// Desc: Create a hash value from a string
//-----------------------------------------------------------------------------
DWORD StringID::HashString( CONST WCHAR* strString )
{
    DWORD HashVal = 0;        
    CONST WCHAR *pChar;

    for ( pChar = strString; *pChar; pChar++ )
    {
        HashVal += *pChar * 193951;
        HashVal *= 399283;
    }
    return HashVal;
}
