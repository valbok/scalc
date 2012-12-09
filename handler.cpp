/** 
 * @file handler.cpp
 * @class CHandler
 */
#include "handler.h"

// Should we include it like that?
#include "actionhandler.cpp"
#include "converter.cpp"
#include "parser.cpp" 
#include "executer.cpp" 
#include "validator.cpp" 

/**
 * @brief Calculates the passed expression
 * @return result of the calculation
 */
double CHandler::calculate( string rawExpression )
{
	// 1. Parse elements: numbers, actions and separators
    CParser parser;
	CActionHandler actionHandler;
	parser.setActionHandler( &actionHandler );
	TElementMap elementMap = parser.parse( rawExpression );
	
	if ( parser.errorExists() ) 
	{
		this->setErrorList( parser.getErrorList() );
		this->_success = false;
		return 0;
	}

	// 2. Validate parsed elements
	CValidator validator;
	validator.setActionHandler( &actionHandler );
	if ( !validator.validate( &elementMap ) )
	{
		this->setErrorList( validator.getErrorList() );
		this->_success = false;
		return 0;
	}
	
	// 3. Calculate
	CExecuter executer;
	executer.setActionHandler( &actionHandler );
	executer.setElementMap( &elementMap );
	double result = executer.execute();

	return result;
}

/**
 * @brief Sets error list
 */
void CHandler::setErrorList( TErrorList errorList )
{
	this->_errorList = errorList;
}

/**
 * @return success of calculation, true if no errors
 */
bool CHandler::success() const
{
	return this->_success;
}

/**
 * @brief Parse the errors to one str
 * @return error string with all errors
 */
string CHandler::getErrors() const
{
	string resultStr = "";
	for( unsigned i = 0; i < _errorList.size(); i++ )
	{
		resultStr += _errorList.at( i ) + "\n";
	}
	
	return resultStr;
}
