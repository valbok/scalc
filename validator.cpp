/**
 * @file validator.cpp
 * @class CValidator
 */

#include "validator.h"

/**
 * @brief Sets action handler
 */
void CValidator::setActionHandler( CActionHandler *actionHandler )
{
	this->_actionHandler = actionHandler;
}

/**
 * @brief Sets element map
 */
void CValidator::setElementMap( TElementMap *elementMap )
{
	this->_elementMap = elementMap;
}

/**
 * @brief Addes an error
 */
void CValidator::addError( string value )
{
    if ( value.empty() )
    {
        return;
    }
    
    this->_errorList.push_back( value );
}

/**
 * @brief Checks if at least one error exists
 */
bool CValidator::errorExists() const
{
    return !this->_errorList.empty();
}

/**
 * @return error list
 */
TErrorList CValidator::getErrorList() const
{
    return this->_errorList;
}

/**
 * @brief Validates the element map
 * @return true if no errors found
 */
bool CValidator::validate()
{
	if ( this->_elementMap == NULL )
	{
		this->addError( NSValidatorErrors::ELEMENT_MAP_DOESNT_EXIST );
		return false;
	}
	
	string currentElem;
	TElementMapIterator it;

	for ( it = this->_elementMap->begin(); it != this->_elementMap->end(); it++ )
	{
		TElementMapIterator nextIt = it;
		nextIt++;
		// If it's the last iteration
		if ( nextIt == this->_elementMap->end() )
		{
			break;
		}

		// Try to determine what's kind of current and next elements
		switch ( it->second.type )
		{
			case E_Number:
			{
				switch ( nextIt->second.type )
				{
					case E_Number:
					{
						this->addError( NSValidatorErrors::UNEXPECTED_NUMBER + ": '" + nextIt->second.value + "' at pos " + CConverter::intToStr( nextIt->first ) );
					} break;

					case E_Separator:
					{
						if ( _actionHandler->isOpenSeparator( nextIt->second.value ) )
						{
							this->addError( NSValidatorErrors::UNEXPECTED_SEPARATOR + ": '" + nextIt->second.value + "' at pos " + CConverter::intToStr( nextIt->first ) );\
						}
					} break;
				}
			} break;
			
			case E_Action:
			{
				// If we found action in the first position in expression we should throw an error
				if ( it == this->_elementMap->begin() && !this->_actionHandler->isCustomAction( it->second.value ) )
				{
					this->addError( NSValidatorErrors::UNEXPECTED_ACTION + ": '" + it->second.value + "' at pos " + CConverter::intToStr( it->first ) );
				}

				switch ( nextIt->second.type )
				{
					case E_Action:
					{
						if ( !_actionHandler->isCustomAction( nextIt->second.value ) )
						{
							this->addError( NSValidatorErrors::UNEXPECTED_ACTION + ": '" + nextIt->second.value + "' at pos " + CConverter::intToStr( nextIt->first ) );
						}
					} break;
					
					case E_Separator:
					{
						if ( _actionHandler->isClosedSeparator( nextIt->second.value ) )
						{
							this->addError( NSValidatorErrors::UNEXPECTED_SEPARATOR + ": '" + nextIt->second.value + "' at pos " + CConverter::intToStr( nextIt->first ) );
						}
					} break;

				}
			} break;

			case E_Separator:
			{
				if ( _actionHandler->isOpenSeparator( it->second.value ) )
				{
					/* Determine the count of separators */

					TElementMapIterator nextIt2 = nextIt;
					unsigned separatorCount = 1;
					string subValue;
					unsigned closedSepPos;
					
					while ( separatorCount > 0 )
					{	
						if ( nextIt2 == this->_elementMap->end() )
						{
							break;
						}

						subValue = nextIt2->second.value;
						if ( this->_actionHandler->isOpenSeparator( subValue ) )
						{
							++separatorCount;
						}
						else
						{
							if ( _actionHandler->isClosedSeparator( subValue ) )
							{
								--separatorCount;
								closedSepPos = nextIt2->second.pos;
							}
						}

						++nextIt2;
					}

					// If the separator count is not 0 so some separators have not been closed, throw an error
					if ( separatorCount > 0 )
					{
						this->addError( NSValidatorErrors::WRONG_SEPARATOR_COUNT + ": '" + it->second.value + "' at pos " + CConverter::intToStr( it->first ) );
					}

					switch ( nextIt->second.type )
					{
						case E_Action:
						{
							if ( !_actionHandler->isCustomAction( nextIt->second.value ) && nextIt->second.value != "-" )
							{
								this->addError( NSValidatorErrors::UNEXPECTED_ACTION + ": '" + nextIt->second.value + "' at pos " + CConverter::intToStr( nextIt->first ) );
							}
						} break;
					}
				}
				else
				{
					switch ( nextIt->second.type )
					{
						case E_Number:
						{
							this->addError( NSValidatorErrors::UNEXPECTED_NUMBER + ": '" + nextIt->second.value + "' at pos " + CConverter::intToStr( nextIt->first ) );
						} break;

						case E_Action:
						{
							if ( _actionHandler->isCustomAction( nextIt->second.value ) )
							{
								this->addError( NSValidatorErrors::UNEXPECTED_ACTION + ": '" + nextIt->second.value + "' at pos " + CConverter::intToStr( nextIt->first ) );
							}
						} break;
					}
				}
			} break;

			case E_Undef:
			default:
			{
				this->addError( NSValidatorErrors::UNEXPECTED_UNDEF + ": '" + it->second.value + "' at pos " + CConverter::intToStr( it->first ) );
			} break;
		}
	}
	
	return !this->errorExists();
}

/**
 * @brief Validates the element map
 */
bool CValidator::validate( TElementMap *elementMap )
{
	this->setElementMap( elementMap );

	return this->validate();
}
