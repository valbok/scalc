/**
 * @file types.h
 * @brief list of all needed types and structures
 */

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <algorithm>

using namespace std;

enum EElementType { E_Action, E_Number, E_Separator, E_Undef };

typedef EElementType TElementType;

struct SElement
{
	string value;
	TElementType type;
	unsigned int pos;
    SElement() {};
    SElement( string value, EElementType type, unsigned int pos ) : type( type ), value( value ), pos( pos ) {};
};

typedef SElement TElement;
typedef map< unsigned int, TElement > TElementMap;
typedef TElementMap::iterator TElementMapIterator; 
typedef vector< string > TErrorList;
