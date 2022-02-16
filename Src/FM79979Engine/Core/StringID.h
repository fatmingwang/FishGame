#ifndef _STRING_ID_H_
#define _STRING_ID_H_
//#pragma warning(disable:4127)
#include <list>



//-----------------------------------------------------------------------------
// Name: StringID
// Desc: Memory management for strings- strings will be inserted into a hash
//       table uniquely, and can be referenced by pointer.  If you want to 
//       insert a string case-insensitively, use SetCaseInsensitive
//-----------------------------------------------------------------------------    

// This is the number of lists in the string hashtable - should be prime
const int StringID_HASHSIZE = 61;

class StringID
{
public:
    // Constructors
    StringID();
    StringID( CONST WCHAR* strString );
    StringID( CONST StringID& other );
	~StringID();

    // Assignment
    StringID& operator= ( CONST StringID& RHS ) { m_strString = RHS.m_strString; return *this; }    
    StringID& operator= ( CONST WCHAR* strRHS ) { m_strString = AddString( strRHS ); return *this; }

    // Comparison
    bool operator== ( CONST StringID& RHS ) CONST { return m_strString == RHS.m_strString; }    
    bool operator== ( CONST WCHAR* strRHS ) CONST;
    bool IsEmptyString() const { return m_strString == s_EmptyString; }

    // Casting
    operator CONST WCHAR* () CONST { return m_strString; }
    CONST WCHAR* GetSafeString() CONST { return ( m_strString ? m_strString : L"null" ); }
    
    // Hash lookup function
    static DWORD        HashString( CONST WCHAR* strString );
	static std::list<CONST WCHAR *>* m_spStringLists;
	static void						 DestoryStringTable();
protected:
    static CONST WCHAR* AddString( CONST WCHAR* strString );
    static std::list<CONST WCHAR *>* GetStringTable();

protected:
    CONST WCHAR*                    m_strString;               
    static CONST WCHAR*             s_EmptyString;
};

#endif