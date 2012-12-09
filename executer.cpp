/**
 * @file executer.cpp
 * @class CExecuter
 */

#include "executer.h" 

/**
 * @brief Sets action handler 
 */
void CExecuter::setActionHandler( CActionHandler* actionHandler )
{
    this->_actionHandler = actionHandler;
}

/**
 * @brief Parses parameters from element map using direction: 
 *               in which way we should parse param: flush right or left from action pos
 * @param \a actionPos is position of action in element map
 * @param \a leftDirection is the left direction or right from action pos
 * @return double value of param
 */
double CExecuter::getParam( unsigned actionPos, bool leftDirection = true )
{
    double result = 0;

    TElementMapIterator it, it2;
    it = this->_elementMap->find( actionPos );
    if ( it == this->_elementMap->end() )
    {
        return result;
    }

    if ( leftDirection )
    {
        --it;
    }
    else
    {
        ++it;
    }
    
    string elem = it->second.value;
    if ( !_actionHandler->isAction( elem ) && !_actionHandler->isSeparator( elem ) )
    {
        result = CConverter::strToDouble( elem );
    }

    return result;
}

/**
 * @brief Sets element map
 */
void CExecuter::setElementMap( TElementMap *elementMap )
{
    this->_elementMap = elementMap;
}

/**
 * @brief Executes all actions
 * @param \a actionList is sorted list of actions
 * @param \a actionPriorPos is pos of the first action
 */
void CExecuter::executeActions( TActionElemList *actionList, unsigned actionPriorPos = 0 )
{
    if ( actionPriorPos >= actionList->size() )
    {
        return;
    }

    unsigned actionPos = actionList->at( actionPriorPos ).pos;
    string actionKey = (*this->_elementMap)[actionPos].value;
    bool customAction = this->_actionHandler->isCustomAction( actionKey );
    
    // If current action is custom action, we should pass to func "right" parameter only
    double leftParam = !customAction ? this->getParam( actionPos ) : 0;
    double rightParam = this->getParam( actionPos, false );

    double result = _actionHandler->executeAction( actionKey, 
                                                   leftParam, 
                                                   rightParam );
    
    TElementMapIterator it;
    
    // If current action is custom we should not delete left/previous value
    if ( !customAction )
    {
        it = this->_elementMap->find( actionPos );
        this->_elementMap->erase( --it );
    }
    
    // Erase next value in elements map after current action
    it = this->_elementMap->find( actionPos );
    // Prevent an error
    if ( it != this->_elementMap->end() )
    {
        this->_elementMap->erase( ++it );
    }
    
    // Store result value to element map
    TElement elem( CConverter::doubleToStr( result ), E_Number, actionPos );
    (*this->_elementMap)[actionPos] = elem;
    
    // Recursive execution
    this->executeActions( actionList, ++actionPriorPos );
}

/**
 * @brief Executes the calulations
 * @return result of the calculation
 */
double CExecuter::execute()
{
    double result = 0;
        
    /* 1. Calculate all expressions within separators: */
    
    // Count of separators after the first found
    unsigned separatorCount = 0;
    TElementMapIterator it;

    // List of iterators should be deleted from element map
    vector< int > purgeList;
    for ( it = this->_elementMap->begin(); it != this->_elementMap->end(); it++ )
    {
        string value = it->second.value;
        if ( this->_actionHandler->isOpenSeparator( value ) ) 
        {
            ++separatorCount;
            string subValue;
            TElementMap sepMap;
            // Positions of closed and open separators
            unsigned openSepPos = 0, closedSepPos = 0;
            openSepPos = it->second.pos;
            
            TElementMapIterator it2 = ++it;
            while ( separatorCount > 0 && it2 != this->_elementMap->end() )
            {    
                subValue = it2->second.value;
                if ( this->_actionHandler->isOpenSeparator( subValue ) )
                {
                    ++separatorCount;
                }
                else
                {
                    if ( this->_actionHandler->isClosedSeparator( subValue ) )
                    {
                        --separatorCount;
                        closedSepPos = it2->second.pos;
                    }
                }
                // Create list of elements should be calculated
                if ( separatorCount != 0 )
                {
                    TElement elem( subValue, it2->second.type, it2->second.pos );
                    sepMap[it2->second.pos] = elem;
                }

                ++it2;
            }
            
            // Try to calculate simple parsed expression
            CExecuter *executer = new CExecuter();
            executer->setActionHandler( _actionHandler );
            executer->setElementMap( &sepMap );
            double subResult = executer->execute();

            TElement elem( CConverter::doubleToStr( subResult ), E_Number, openSepPos );
            (*this->_elementMap)[openSepPos] = elem;

            TElementMapIterator itTmp, itOpenErase, itClosedErase;
            itOpenErase = this->_elementMap->find( openSepPos );
            itClosedErase = this->_elementMap->find( closedSepPos );    
            itOpenErase++;
            
            for ( itTmp = itOpenErase; itTmp != itClosedErase; ++itTmp )
            {    
                purgeList.push_back( itTmp->first );
            }

            purgeList.push_back( itClosedErase->first );

            delete executer;
        }
    }

    // Purge elemets within separators in element map, because we have already calculated it
    for ( unsigned i = 0; i < purgeList.size(); i++ )
    {
        this->_elementMap->erase( purgeList[i] );
    }

    /* 2.  Calculate simpled expression like 1+2/3 */
    
    // Create list of priorities for further sorting
    TActionElemList priorElemList;
    for ( it = this->_elementMap->begin(); it != this->_elementMap->end(); it++ )
    {
        if ( this->_actionHandler->isAction( it->second.value ) )
        {
            TActionElem elem( it->second.value, 
                              it->first, 
                              this->_actionHandler->getPriorityByAction( it->second.value ) );
            priorElemList.push_back( elem );
        }
    }

    // Sort actions by priority
    // In the beginning of the list will be actions with the highest priority
    sort( priorElemList.begin(), priorElemList.end(), CExecuter::comparePriority );

    // Execute parsed actions
    this->executeActions( &priorElemList );
    
    // If no problems the element map will contain one result element - our goal
    it = this->_elementMap->begin();
    result = CConverter::strToDouble( it->second.value );
    
    return result;
}

/**
 * @brief Compares the priorities of two actions, is used in sorting
 * @return result of comparsion
 */
bool CExecuter::comparePriority( TActionElem e1, TActionElem e2 )
{
    return e1.priority > e2.priority;
}
