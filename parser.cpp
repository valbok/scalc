/**
 * @file parser.cpp
 * @class CParser
 */

#include "parser.h"

/**
 * @brief Sets action handler
 * @param action handler
 */
void CParser::setActionHandler( CActionHandler *actionHandler )
{
    this->_actionHandler = actionHandler;
}

/**
 * @brief Tries to find action with actionHandler by position
 * @return action string
 */
string CParser::getActionByPosition( string actionStr, unsigned int pos ) const
{
    string elem, currentChar = "", nextChar= "";
    
    while ( pos < actionStr.length() )
    {
        currentChar = actionStr.at( pos );
        elem += currentChar;
        
        // If elem is action
        if ( this->_actionHandler->isAction( elem )  )
        {
            // Try to check if elem is not custom action
            if ( !this->_actionHandler->isCustomAction( elem ) )
            {
                return elem;
            }
            else
            {
                if ( ( pos + 1 ) < actionStr.length() )
                {
                    // If elem is custom action it means next char after current pos should be open separator
                    // If next char is not open separator (for example '(') we should continue the parsing
                    nextChar = actionStr.at( pos + 1 );
                    if ( this->_actionHandler->isOpenSeparator( nextChar ) )
                    {
                        return elem;
                    }
                }
            }
        }

        ++pos;
    }

    return "";
}

/**
 * @brief Addes element to element map
 * @param \a value is string value of element
 * @param \a type is type of element
 * @param \a pos is position of value in raw expression string
 */
void CParser::addElement( string value, TElementType type, unsigned int pos )
{
    if ( value.empty() )
    {
        return;
    }

    TElement elem( value, type, pos );
    this->_elementMap[pos] = elem;
}

/**
 * @brief Parses actions and separators to element map
 * @param \a actionStr in which string we should find the actions
 * @param \a currentPos is the position of first element of \a actionStr in raw expression string 
 */
void CParser::parseActions( string actionStr, unsigned currentPos )
{
    // Parsed action str if it contains more than one action should be splitted, for example '+(' => '+' and '('    
    string actionStrSplitted;
    // After parsing of actions we should check for errors
    // i.e. unexpected elements, we just concat all parsed actions to one str
    // and check difference between base action str actionStrSplitted,
    // if there is a difference so unneeded elements exist
    string actionStrJoined = "";
    // Current action char, contains char for current iterator
    string currentActionChar = "";
    // Separator string \sa fillSeparators()
    string separatorStr = "";
    unsigned pos = 0;
    // If any error exists
    bool errorExists = false;
    // If some errors write error to this var
    string errorStr;
    
    while ( pos < actionStr.length() )
    {
        currentActionChar = actionStr.at( pos );
        
        // If current action char is separator
        if ( _actionHandler->isSeparator( currentActionChar ) )
        {
            // Add it to splitted string but it will not be added to element list afterwards
            actionStrSplitted = currentActionChar;
            // Store it for adding as a seperator to elements list
            separatorStr = currentActionChar;
        }
        else
        {
            // Try to parse actions
            actionStrSplitted = this->getActionByPosition( actionStr, pos );
        }
        
        if ( !separatorStr.empty() )
        {
            // Store the separator to elements list
            this->addElement( separatorStr, E_Separator, currentPos + pos );
            separatorStr = "";
        }
        else
        {
            // If the separator doesn't exist and we found an action, store this action
            if ( !actionStrSplitted.empty() )
            {
                // We should not add the action if it is located in the end of the expression string
                if ( currentPos < this->_rawExpression.length() )
                {
                    this->addElement( actionStrSplitted, E_Action, currentPos + pos );
                }
                else
                {
                    // If we tried to add the action is located in the end, we should set the error
                    // We want to provide errors to the user about this problem
                    errorExists = true;
                }
            }
        }
        
        // Prevent unneeded iterations
        pos = !actionStrSplitted.empty() ? pos + actionStrSplitted.length() : ++pos;
        
        // If there are some errors we should not add splitted str to joined str
        // We need to provide all errors to the user about parsing of expression
        if ( !errorExists )
        {
            actionStrJoined += actionStrSplitted;
        }
    }

    // Check for errors
    // If these two strings are not equal so a problem exists
    if ( actionStrJoined != actionStr )
    {
        errorStr = NSParserErrors::UNKNOWN_SYMBOLS + "; " + NSParserErrors::CHECK_RIGHT_SYNTAX_NEAR + " '" + actionStr + "' at pos " + CConverter::intToStr( currentPos );
        this->addError( errorStr );
    }
}

/**
 * @brief Parses all elements and stores to element map
 */
void CParser::parseElements()
{
    unsigned int currentPos = 0, pos = 0;;
    // An action is parsed as one str like: '+', '-', or '-(' etc
    string actionStr;
    // String with parsed numeric elements
    string numStr = "";
    // Current char in global loop
    char currentChar;
    string &rawExpression = this->_rawExpression;
    // Skip spaces
    rawExpression = CParser::trim( rawExpression );
    
    while ( currentPos < rawExpression.length() )
    {
        currentChar = rawExpression.at( currentPos );
        
        if ( CConverter::isNumeric( currentChar ) )
        {
            numStr += currentChar;
            
            // Try to fetch an action
            // If actionStr is not empty so in previous iteration an action has been found
            if ( !actionStr.empty() )
            {
                this->parseActions( actionStr, currentPos - actionStr.length() );
            }
        
            actionStr = "";
        }
        else
        {
            actionStr += currentChar;
            if ( !numStr.empty() )
            {
                this->addElement( numStr, E_Number, currentPos - numStr.length() );
            }

            numStr = "";
        }

        currentPos++;
    }
    
    // If the expresion contains some numbers in the end
    if ( !numStr.empty() )
    {
        this->addElement( numStr, E_Number, currentPos - numStr.length() );
    }
    
    // If there are some elements in the end of expressions like closed separator ')' or some actions putted by mistake
    if ( !actionStr.empty() )
    {
        this->parseActions( actionStr, currentPos );
    }
}

/**
 * @brief Sets user expression should be parsed
 */
inline void CParser::setExpression( string expression )
{
    this->_rawExpression = expression;
}

/**
 * @brief Parses the raw expression 
 * @param User native raw expression string like "1+2*3"
 * @return element map of parsed elements
 */
TElementMap CParser::parse( string rawExpression )
{
    this->setExpression( rawExpression );
    this->parseElements();

    return this->_elementMap;
}

/**
 * @brief Addes error to error list
 */
void CParser::addError( string value )
{
    if ( value.empty() )
    {
        return;
    }
    
    this->_errorList.push_back( value );
}

/**
 * @brief Checks if at least one error exists
 * @return true if at least one error exists
 */
bool CParser::errorExists() const
{
    return !this->_errorList.empty();
}

/**
 * @brief Goes for each chars and skips all spaces
 * @param base string with spaces
 * @return string without spaces
 */
string CParser::trim( string value )
{
    string result = "";
    for ( unsigned i = 0; i < value.length(); ++i )
    {
        if ( value.at( i ) != ' ' )
        {
            result += value.at( i );
        }
    }

    return result;
}

/**
 * @return error list
 */
TErrorList CParser::getErrorList() const
{
    return this->_errorList;
}
