/**
 * @file calc.cpp
 * @brief It's just an example how to use simple math processor
 * @sa handler.cpp
 */
 
#include "handler.cpp"

int main( int argc, char* argv[] )
{
	if ( argc == 1 )
	{
		string help( "This is a simple mathematical processor (version 0.1)" );
		string usage( "* Usage: " );
		usage = usage + argv[0] + " \"sqr( ( 2.5 * 80 - 10 ^ 0.5 ) / 2 + 0.1 )\"";
		cout << help << endl << usage << endl; 
		return 0;
	}

	string rawExpression = argv[1];
	CHandler *handler = new CHandler;

	double result = handler->calculate( rawExpression );
	
	if ( !handler->success() )
	{
		string errors = handler->getErrors();
		cout << "Errors:\n" << errors << endl;
	}
	else
	{
		cout << result << endl;
	}

	delete handler;

	return 0;
}
