/**
 *    author:  ÁõÃ÷³Ð-919106840423 
 *    created: 2022 
 *	  describ:  
 **/ 

#include "NFA.h"
 
int main()
{
	string file1 = "input.txt", file2 = "output.txt", file3 = "source code.txt";
	
	NFA nfa;
	
	nfa.Input( file1 );
	
	nfa.Print( file2 );
	
	nfa.nfa_2_dfa();
	
	nfa.Print( file2 );
	
	LA la;
	la.T = nfa.Start;
	
	la.analysis( nfa, file3, file2 );

	return 0;
}
