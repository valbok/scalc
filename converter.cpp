/**
 * @file converter.cpp
 * @class CConverter
 */

#include "converter.h"

/**
 * @brief Convertes double to string
 */
inline string CConverter::doubleToStr( double x )
{
    ostringstream o;

    if ( ( o << x ) )
    {
        return o.str();
    }

    return 0;
}

/**
 * @brief Convertes string to double
 */
inline double CConverter::strToDouble( string str )
{
    istringstream stm;
    stm.str( str );
    double d;
    stm >> d;

    return d;
}

/**
 * @brief Convertes integer to string
 */
inline string CConverter::intToStr( int i )
{
    stringstream out;
    out << i;
    string result = out.str();

    return result;
}

/**
 * @brief Checks if passed char is part of numeric
 */
inline bool CConverter::isNumeric( char element )
{
    return ( element > 47 && element < 58 ) || element == '.' ? true : false;
}
