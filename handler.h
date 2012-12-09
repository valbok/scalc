/** 
 * @file handler.h
 * @class CHandler
 * @brief Handler class is manager of calculation, the main class of expression calulactions
 */

#ifndef HANDLER_H
#include "types.h"
#define HANDLER_H
#endif /* HANDLER_H */

class CHandler
{
	TErrorList _errorList;
	bool _success;
	void setErrorList( TErrorList errorList );

public:
	CHandler() { this->_success = true; };
	double calculate( string rawExpression );
	inline bool success() const;
	
	string getErrors() const;
};
