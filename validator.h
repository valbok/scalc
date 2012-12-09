/**
 * @file validator.h
 * @class CValidator
 * @brief The validator should validate native not validated expression.
 *        The expression is created by parser, 
 *        the native expression string (for instance "1+2/3") (after the parsing)
 *        should be validated before calculations.
 */
namespace NSValidatorErrors
{
    const string ELEMENT_MAP_DOESNT_EXIST = "Parsed element map is not set";
    const string UNEXPECTED_NUMBER = "Unexpected NUMERIC element is found";
    const string UNEXPECTED_ACTION = "Unexpected ACTION element is found";
    const string UNEXPECTED_SEPARATOR = "Unexpected SEPARATOR element is found";
    const string UNEXPECTED_UNDEF = "Unexpected UNDEFINED element is found";
    const string WRONG_SEPARATOR_COUNT = "Couldn't find closed separator";
};

class CValidator
{
private:
    TElementMap *_elementMap;
    CActionHandler *_actionHandler;
    TErrorList _errorList;

    inline void addError( string value );
    void setElementMap( TElementMap *elementMap );

public:
    CValidator() {};
    CValidator( TElementMap *elementMap ) { this->setElementMap( elementMap ); };

    inline TErrorList getErrorList() const;

    inline bool errorExists() const;
    bool validate();
    bool validate( TElementMap *elementMap );

    void setActionHandler( CActionHandler *actionHandler );
};
