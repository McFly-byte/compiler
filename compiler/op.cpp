/* 将文法的语言描述改写为机器易识别的符号 */
#include "op.h"

string UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;
 
	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);
 
	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);
 
	result = szRes;
	delete[]strSrc;
	delete[]szRes;
 
	return result;
}

void turn_to_2NF( string In, string Out )
{
	cin.clear();
	freopen( In.c_str(), "r", stdin );
	ofstream fout;
	fout.open( Out.c_str() );
	map<string,char> mp;
	string str;
	char Vn = 'A', Vt = 'a';
	string arrow = "→";
	cout << "\n输入产生式\n" ;
	while ( getline( cin , str ) )
	{
		std::string s = UTF8ToGB(str.c_str()).c_str();
//		string stack;
	cout << s << endl;
		int i = 0;
		while ( i < s.length() )
		{
			if( s[i] == arrow[0] && s[i+1] == arrow[1] )
			{
				fout << "->";
				i = i+2;
			}
			if( s[i] == ' ' ) 
			{
				i++;
			}
			else if( s[i] == '<' )
			{
				i++;
				int j = i;
				bool f = false;
				while ( s[j] != '>' )
				{
					if( s[j] == '|' )
					{
						f = true;
						break;
					 } 
					j++;
				}
				if( f )
				{
					string stack = s.substr( i-1, j-i );
					i = j+1;
					if( mp.count(stack) == 0 ) 
					{
						mp[stack] = char(Vt++);
					}
					fout << mp[stack];
					continue;
				}
//				cout << endl << i << ' ' << j << endl;
				string stack = s.substr(i,j-i); 
				
//				cout << "stack: " << stack << endl;
				i = j+1;
				if( mp.count(stack) == 0 ) 
				{
					mp[stack] = char(Vn++);
				}
				fout << mp[stack];
			}
			else if( s[i] == '|' )
			{
				fout << s[i];
				i++;
			 } 
			else if( s[i] == '(' && find( all(s.substr(i)), '|' ) != s.substr(i).end() )
			{
//				cout << '	' << s.substr(i) << endl;
				i++;
				while ( s[i] != ')' )
				{
					if( s[i] == ' ' ) i++;
					else if( s[i] == '|' )
					{
						i++;
					 } 
					else
					{
						int j = i;
						while ( s[j] != ' ' && s[j] != '|' && s[j] != ')' )
						{
							j++;
						}
						string stack = s.substr(i,j-i);
						i = j+1;
						if( mp.count(stack) == 0 ) 
						{
							mp[stack] = char(Vt);
						}
					}
				}
				i++;
				fout << Vt;
				Vt++; 
			}
			else 
			{
				int j = i;
				while ( s[j] != ' ' )
				{
					j++;
				}
				string stack = s.substr(i,j-i);
				i = j+1;
				if( mp.count(stack) == 0 ) 
				{
					mp[stack] = char(Vt++);
				}
				fout << mp[stack];
				stack = "";
			}
		}
		fout << endl;
	}
	fout << endl;
	for ( const auto p : mp )
	{
		fout << p.first << ':' << p.second << " ";
	}
	
	fclose(stdin);
	fout.close();
	cout << endl;
}
