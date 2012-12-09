/**
 * @file executer.h
 * @class CExecuter
 * @brief CExecuter class is the simple hanlder of actions execution, 
 *        it parses and goes for each action and tries to execute it with action handler
 */

class CExecuter
{
	TElementMap *_elementMap;
	CActionHandler *_actionHandler;
	
	struct SAction 
	{ 
		string key;
		unsigned pos;
		unsigned priority;

        SAction() {};
        SAction( string key, unsigned pos, unsigned priority ) : key( key ), pos( pos ), priority( priority ) {};
	};

	typedef SAction TActionElem;
	typedef vector< TActionElem > TActionElemList;

	double getParam( unsigned actionPos, bool leftDirection ); //?
	static bool comparePriority( TActionElem e1, TActionElem e2 );

public:	
	void executeActions( TActionElemList *actionList, unsigned actionPriorPos );
	
	void setElementMap( TElementMap *elementMap );
	void setActionHandler( CActionHandler *actionHandler );
	
	double execute();
};
