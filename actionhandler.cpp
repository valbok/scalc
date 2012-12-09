/**
 * @file actionhandler.cpp
 * @class CActionHandler 
 */

#include "actionhandler.h"

/**
 * @brief Filles the separators
 */
void CActionHandler::fillSeparators()
{
    this->_openSeparator = "(";
    this->_closedSeparator = ")";
}

/**
 * @brief Checks if passed string is open separator
 */
inline bool CActionHandler::isOpenSeparator( string s ) const
{
    return s == this->_openSeparator;
}

/**
 * @brief Checks if passed string is closed separator
 */
inline bool CActionHandler::isClosedSeparator( string s ) const
{
    return s == this->_closedSeparator;
}

/**
 * @brief Checks if passed string is a separator
 */
inline bool CActionHandler::isSeparator( string s ) const
{
    return this->isOpenSeparator( s ) || this->isClosedSeparator( s );
}

/**
 * @brief Addes an action to action list
 */
void CActionHandler::addAction( string actionStr, TActionPriority type )
{
	// The action should not already exist
	// and should not be a separator
    if ( !this->isAction( actionStr ) && !this->isSeparator( actionStr ) )
    {
		TAction action( actionStr, type );
        this->_actionList.push_back( action );
    }
}

/**
 * @brief Filles the action list
 */
void CActionHandler::fillActionList()
{
    this->addAction( "+", E_Low );
    this->addAction( "-", E_Low );
    this->addAction( "/", E_High );
    this->addAction( "*", E_High );
    this->addAction( "^", E_High );
    this->addAction( "sqr", E_Custom );
    this->addAction( "sqrt", E_Custom );
}

/**
 * @brief Checks if passed string is an action
 */
bool CActionHandler::isAction( string actionKey ) const
{
    // TODO: implement with stl find() and iterators
    
    for ( unsigned int i = 0; i < this->_actionList.size(); i++ )
    {
        if ( this->_actionList[i].key == actionKey )
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Checks if passed string is custom action
 */
bool CActionHandler::isCustomAction( string actionKey ) const
{
    // TODO: implement with stl find() and iterators
    
    for ( unsigned int i = 0; i < this->_actionList.size(); i++ )
    {
        if ( this->_actionList[i].key == actionKey && this->_actionList[i].priority == E_Custom )
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Executes the action
 * @param \a actionStr is action string should be executed
 * @param \a firstParam, \a lastParam are first and las params should be passed to fucn to action execution
 * @return result of executaion
 */
double CActionHandler::executeAction( string actionStr, double firstParam, double lastParam ) const
{
	if ( actionStr == "+" )
	{
		return this->add( firstParam, lastParam );
	}

	if ( actionStr == "-" )
	{
		return this->sub( firstParam, lastParam );
	}
	
	if ( actionStr == "*" )
	{
		return this->mul( firstParam, lastParam );
	}

	if ( actionStr == "/" )
	{
		return this->div( firstParam, lastParam );
	}

	if ( actionStr == "sqr" )
	{
		return this->sqr( lastParam );
	}

	if ( actionStr == "sqrt" )
	{
		return this->sqrt( lastParam );
	}

	if ( actionStr == "^" )
	{
		return this->mypow( firstParam, lastParam );
	}

	return 0;
}

/**
 * @brief sqr()ing
 */
inline double CActionHandler::sqr( double lastParam ) const
{
	return lastParam * lastParam;
}

/**
 * @brief sqrt()ing
 */
inline double CActionHandler::sqrt( double lastParam ) const
{
	return pow( lastParam, 0.5 );
}

/**
 * @brief Adding
 */
inline double CActionHandler::add( double firstParam, double lastParam ) const
{
	return firstParam + lastParam;
}

/**
 * @brief Subtraction
 */
inline double CActionHandler::sub( double firstParam, double lastParam ) const
{
	return firstParam - lastParam;
}

/**
 * @brief Pow
 */
inline double CActionHandler::mypow( double firstParam, double lastParam ) const
{
	return pow( firstParam, lastParam );
}

/**
 * @brief Division
 */
inline double CActionHandler::div( double firstParam, double lastParam ) const
{
	if ( lastParam == 0 )
	{
		return 0;
	}

	return firstParam / lastParam;
}

/**
 * @brief Multiplication
 */
inline double CActionHandler::mul( double firstParam, double lastParam ) const
{
	return firstParam * lastParam;
}

/**
 * @brief Gets action priority 
 * @return priority of passed action
 */
unsigned CActionHandler::getPriorityByAction( string actionStr )
{
	if ( !this->isAction( actionStr ) )
	{
		return 0;
	}

    for ( unsigned int i = 0; i < this->_actionList.size(); i++ )
    {
        if ( this->_actionList.at( i ).key == actionStr )
        {
            return _actionList.at( i ).priority;
        }
    }

	return 0;
}
