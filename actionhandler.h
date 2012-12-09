/**
 * @file actionhandler.h
 * @class CActionHandler
 * @brief The action handler is simple handler for actions like '+', '-' or custom actions like 'sqrt' or 'cos'
 */

#include <math.h>

class CActionHandler
{
    /* List of action priorpoties */
    enum EActionPriority { E_Low = 0, E_High = 1, E_Custom = 2 };
    typedef EActionPriority TActionPriority;

    /* Action item */
    struct SAction
    {
        string key;
        TActionPriority priority;
        SAction() {};
        SAction( string name ) : key( name ), priority( E_Low ) {};
        SAction( string name, TActionPriority priority ) : key( name ), priority( priority ) {};
    };

    typedef SAction TAction;
    typedef vector< TAction > TActionList;
    typedef TActionList::iterator TActionListIterator;

    /* List of supported actions */
    TActionList _actionList;

    /* Variables contain the first and the last separators like '(' or ')' */
    string _openSeparator;
    string _closedSeparator;

    void fillSeparators();
    void fillActionList();

    /* List of actions */
    inline double add( double firstParam, double lastParam ) const;
    inline double sub( double firstParam, double lastParam ) const;
    inline double mul( double firstParam, double lastParam ) const;
    inline double div( double firstParam, double lastParam ) const;
    inline double mypow( double firstParam, double lastParam ) const;
    inline double sqrt( double firstParam ) const;
    inline double sqr( double firstParam ) const;

public:
    /* Constructor: fill separators and fill actions */
    CActionHandler() { fillSeparators(); fillActionList(); };

    inline bool isOpenSeparator( string s ) const; // should it be public???
    inline bool isClosedSeparator( string s ) const;
    inline bool isSeparator( string s ) const;

    void addAction( string actionStr, TActionPriority type );
    unsigned getPriorityByAction( string actionStr );
    bool isAction( string actionKey ) const;
    bool isCustomAction( string actionKey ) const;

    /* Execute action with params */
    double executeAction( string actionStr, double firstParam, double lastParam ) const;
};
