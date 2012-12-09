/**
 * @file parser.h
 * @class CParser
 * @brief CParser parses raw native user string with an expression and return parsed element map for further calculation steps
 */

namespace NSParserErrors
{
    const string UNKNOWN_SYMBOLS = "Unknown symbols found";
    const string CHECK_RIGHT_SYNTAX_NEAR = "check the right syntax to use near";
};

class CParser
{
    TElementMap _elementMap;
    TErrorList _errorList;

    // The expression string should be parsed
    string _rawExpression;
    CActionHandler *_actionHandler;

    void addElement( string value, TElementType type, unsigned int pos );
    void parseActions( string actionStr, unsigned currentPos );

    inline void addError( string value );
    inline void setExpression( string expression );
    static string trim( string value );
    void parseElements();
    string getActionByPosition( string actionStr, unsigned int pos ) const;

public:
    inline bool errorExists() const;
    inline TErrorList getErrorList() const;

    void setActionHandler( CActionHandler *actionHandler );

    TElementMap parse( string rawExpression );
};
