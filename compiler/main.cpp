/**
 *    author:  ÁõÃ÷³Ð-919106840423 
 *    created: 2022 
 *	  describ:  
 **/ 

#include "Lexer/NFA.h"
#include "op.h"
#include "Parser/LR.h"
 
int main()
{
	string file1 = "Lexer/input.txt", file2 = "Lexer/output.txt", file3 = "Lexer/source code.txt", file4 = "Lexer/lexer.txt";
	string file5 = "Parser/describe.txt", file6 = "Parser/2NF grammar.txt";
	string file7 = "Parser/2NF grammar.txt", file8 = "Parser/parser.txt", file9 = "D:/MyRepository/compiler/compiler/Lexer/lexer.txt";
	
	NFA nfa;
	
	nfa.Input( file1 );
	
	nfa.Print( file2 );
	
	nfa.nfa_2_dfa();
	
	nfa.Print( file2 );
	
	LA la;
	la.T = nfa.Start;
	
	la.analysis( nfa, file3, file4 );
	
	turn_to_2NF( file5, file6 );
	
	LR1 a;
	
	a.input( file7 );
	
	a.build_DFA();
	
	a.analysis( file9, file8 );

	return 0;
}
