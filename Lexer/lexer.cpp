/**
 *    author:  ������-919106840423 
 *    created: 2022 
 *	  describ:  �ʷ������� Lexical Analysis 
 *				�ؼ���(keys)����ʶ��(token)�������(operator)�����(separator)������(constant) 
 *				û������ʽ����İ汾������Դ����ֱ�Ӹ���token�� 
**/

#include <bits/stdc++.h>
using namespace std;

#define ERROR -1 
#define KEY 1
#define TOKEN 2
#define OPERATOR 3
#define SEPARATOR 4
#define CONSTANT 5

void keys_init();		//���ùؼ���
void separators_init();	//���ý�� 
bool start_of_token( char c );  	// �ж��Ƿ��п���Ϊ ��ʶ�� 
bool start_of_operator( char c );	// �ж��Ƿ��п���Ϊ ����� 
bool start_of_separator( char c );	// �ж��Ƿ��п���Ϊ ���  
bool start_of_constant( char c );	// �ж��Ƿ��п���Ϊ ���� 
void scan_line( string line );	// һ�δ���һ������ 
void Print( string word, int identifier );
void Print( int er ); 

unordered_map<string, int>  Keys, Separators;
string line;
int line_id = 1;

int main()
{
	freopen( "source code.txt", "r", stdin );
	
	keys_init();
	separators_init();
	
	do {
		getline( cin, line );
		scan_line( line );
		line_id++;
	} while( line != "" );
	return 0;
 } 


/* -------------------------------------------function-----------------------------------*/

void scan_line( string line )
{
	int n = line.length(), tag = 0, j = 0, i = 0;
	string word = "";
	while ( i < n )
	{
		if( start_of_separator( line[i] ) ) //��� 
		{
			Print( line.substr(j,1), SEPARATOR );
			j = ++i;
			continue;
		}
		if( start_of_operator( line[i] ) ) //����� 
		{
			if( line[i] == '<' && line[i+1] == '<' || i+1 < n && line[i] == '>' && line[i+1] == '>')  
				i++, tag++;
			i++;
			if( line[i] == '=' ) 
			{
				Print( line.substr(j,2+tag), OPERATOR );
				j = ++i;
				continue;
			}
			else 
			{
				Print( line.substr(j,1+tag), OPERATOR );
				j = i;
				continue;
			}
		}
		if( start_of_constant( line[i] ) ) //���� 
		{
			if( isdigit( line[i] ) )
			{
				while ( i < n && isdigit( line[i] ) ) i++;
				if( line[i] == '.' ) 
				{
					++i;
					while ( i < n && isdigit( line[i] ) ) i++;
				}
				Print( line.substr(j,i-j), CONSTANT );
				j = i;
				continue ;
			}
			if( line[i] == '\'' ) 
			{
				i++;
				if( i+1 < n && line[i+1] == '\'' ) 
				{
					Print( line.substr(i,1), CONSTANT );
					i++;
					j = ++i;
					continue;
				}
				else 
					Print( ERROR );
			}
			if( line[i] == '\"' )
			{
				i++;
				j = i;
				while ( i < n && line[i] != '\"' ) i++;
				if( i >= n ) 
					Print( ERROR );
				else 
					Print( line.substr( j, i-j ), CONSTANT );
				j = ++i;
				continue;
 			}
 			
		}
		if( start_of_token( line[i] ) ) //��ʶ��(�ؼ���)
		{
			j = i++;
			while ( isdigit(line[i]) || isalpha(line[i]) || line[i] == '_' ) i++;
			string tmp = line.substr( j, i-j );
			if( Keys[tmp] == 1 ) 
				Print( tmp, KEY );
			else 
				Print( tmp, TOKEN );
			j = i; 
			continue;
		 } 
	}

} 



inline bool start_of_token( char c )
{
	if( c == '_' || isalpha( c ) ) return true;
	return false;
 } 

inline bool start_of_constant( char c )
{
	if( isdigit( c ) || c == '\"' || c == '\'' ) return true;
	return false;
}

inline bool start_of_separator( char c )
{
	string s(1,c);
	if( Separators.count( s ) != 0 ) return true;
	return false;
}
 
inline bool start_of_operator( char c )
{
	if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%' 
	 || c == '&' || c == '^' || c == '|' || c == '<' || c == '>' 
	 || c == '=' ) return true;
	return false;
}

void Print()
{
	//TODO
}

void Print( int er )
{
	cout << "( ERROR )" << endl;
	system("pause");
}

void Print( string word, int identifier )
{
	int &id = identifier, len = word.length();
	if( id == KEY )
	{
		cout << "| " << setw(3) << line_id << " | " << setw(9) << "KEY" << " | " << std::right << setw(10) << word.substr(0,len/2);
		cout << std::left << setw(10) << word.substr(len/2,len-len/2) << " |" << endl;
	}
	else if( id == TOKEN )
	{
		cout << "| " << setw(3) << line_id << " | " << setw(9) << "TOKEN" << " | " << std::right << setw(10) << word.substr(0,len/2);
		cout << std::left << setw(10) << word.substr(len/2,len-len/2) << " |" << endl;
	}
	else if( id == OPERATOR )
	{
		cout << "| " << setw(3) << line_id << " | " << setw(9) << "OPERATOR" << " | " << std::right << setw(10) << word.substr(0,len/2);
		cout << std::left << setw(10) << word.substr(len/2,len-len/2) << " |" << endl;
	}
	else if( id == SEPARATOR )
	{
		cout << "| " << setw(3) << line_id << " | " << setw(9) << "SEPARATOR" << " | " << std::right << setw(10) << word.substr(0,len/2);
		cout << std::left << setw(10) << word.substr(len/2,len-len/2) << " |" << endl;
	}
	else if( id == CONSTANT )
	{
		cout << "| " << setw(3) << line_id << " | " << setw(9) << "CONSTANT" << " | " << std::right << setw(10) << word.substr(0,len/2);
		cout << std::left << setw(10) << word.substr(len/2,len-len/2) << " |" << endl;
	}
}

void keys_init()	//���ùؼ���
{
	Keys["auto"] = 1;
//	Keys["bool"] = 1;
	Keys["break"] = 1;
	Keys["case"] = 1;
//	Keys["catch"] = 1;
	Keys["char"] = 1;
//	Keys["class"] = 1;
	Keys["const"] = 1;
	Keys["continue"] = 1;
	Keys["default"] = 1;
//	Keys["delete"] = 1;
	Keys["do"] = 1;	
	Keys["double"] = 1;
	Keys["else"] = 1;
	Keys["enum"] = 1;
//	Keys["false"] = 1;
	Keys["float"] = 1;
	Keys["for"] = 1;
//	Keys["friend"] = 1;/
	Keys["goto"] = 1;
	Keys["if"] = 1;
//	Keys["inline"] = 1;
	Keys["int"] = 1;
	Keys["long"] = 1;
	Keys["register"] = 1;
	Keys["return"] = 1;
	Keys["short"] = 1;
	Keys["signed"] = 1;
	Keys["sizeof"] = 1;
	Keys["static"] = 1;
	Keys["struct"] = 1;
	Keys["switch"] = 1;
	Keys["typedef"] = 1;
	Keys["union"] = 1;
	Keys["unsigned"] = 1;
	Keys["void"] = 1;
	Keys["volatile"] = 1;
	Keys["while"] = 1;
}	
	
void separators_init()	//���ý�� 
{
	Separators["("] = 1;
	Separators[")"] = 1;
	Separators["{"] = 1;
	Separators["}"] = 1;
	Separators[","] = 1;
	Separators[";"] = 1;
	Separators[" "] = 1;
	Separators["\n"] = 1;
}
