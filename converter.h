/**
 * @file converter.h
 * @class CConverter
 * @brief CConverter class contains methods for convertion
 */

#include <sstream>

class CConverter
{
public:
    inline static string intToStr( int i );
    inline static double strToDouble( string str );
    inline static string doubleToStr( double x );
    inline static bool isNumeric( char element );
};
