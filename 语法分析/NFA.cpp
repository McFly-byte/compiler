#include "NFA.h"

void LA::analysis( NFA& nfa, string file3, string file2 )
{
	cin.clear(); 
	freopen( file3.c_str(), "r", stdin );
	int n;
	while ( getline( cin, line ) )
	{
		cout << line << endl;
		n = line.length();
		for ( int i = 0; i < n; i++ )
		{	
			read_next_VT( line[i], nfa, file2 );
		} 
		output( nfa.Symbol[T], nfa, file2 );
		T = nfa.Start;
		index = 0;
		word = "";
	}
	fclose(stdin);
}

void LA::read_next_VT( char ch, NFA& nfa, string file2 )
{
	cout << "T: " << T << "	ch: " << ch << " word: " << word << endl;
	auto it = std::find_if( nfa.List[index].begin(), nfa.List[index].end(), isEqualALL(ch));
	if( it == nfa.List[index].end() )
	{
		if( nfa.FinalSet[index] ) 
		{
			output( nfa.Symbol[T], nfa, file2 );
			T = nfa.Start;
			index = 0;
			word = "";
			read_next_VT( ch, nfa, file2 ); 
		} 
		else 
		{
			output( ERROR, nfa, file2 );
		}
	}
	else {
		T = it->first;
		index = nfa.Map[T];
		word += ch;
	}
	
}

void LA::output( int tag, NFA& nfa, string file2 )
{
	fout.open( file2.c_str(), ios::app );
	if( tag == ERROR )
	{
		index = nfa.Map[T];
		auto it = std::find_if(nfa.Tag.begin(),nfa.Tag.end(),[tag](const std::map<std::string,int>::value_type item )->bool{return item.second == tag;});
		fout << "ERROR(" << tag << "): While tackling " << it->first << ", " << word << " is not a " << it->first << endl; 
		system("pause"); 
	}
	else 
	{
		auto it = std::find_if(nfa.Tag.begin(),nfa.Tag.end(),[tag](const std::map<std::string,int>::value_type item )->bool{return item.second == tag;});  /* ����value����key */
		int len = word.length();
		fout << "| " << std::right << setw(3) << id++ << " | " << std::right << setw(10) << it->first << " | ";
		fout << std::right << setw(10) << word.substr(0,len/2);
		fout << std::left  << setw(10) << word.substr(len/2,len-len/2) << " |" << endl;
	cout << id-1 << ' ' << it->first << ' ' << word << endl;
	}
	fout.close();
}

LA::LA()
{
	index = 0;
	id = 0;
	word = "";
	T = "";
}

// --------------------------------------------------------------------------------------------

void NFA::nfa_2_dfa()
/* NFA��ͼת��Ϊ DFA��ͼ */
{
	map<string,int> NewMap;    	/* VN���� */
	vector<vector<psc> > NewList;  /* ״̬ת��ͼ */
	map<string,int> NewSymbol; 	/* ��״̬���Ʊ� */
	
	vector<vector<string> > NewStates;  /* ��״̬���� */
	
	int n, id, t = 0;
	string start = "T", NewStart;
	vector<string> NewStartSet;
	
	e_closure( Start, NewStartSet ); /* ״̬V��$�հ� */
	sort( NewStartSet.begin(), NewStartSet.end() );
	NewStates.push_back( NewStartSet );
	NewStart = start+to_string(t);
	NewMap[start+to_string(t)] = t++;
	
	for ( int map_id = 0; map_id < t; map_id++ )
	{
		n = NewStates[map_id].size(); /* ��ǰ����״̬�� */
		vector<psc> list_arcs; /* �ɱ���״̬�����Ļ� */
		unordered_map<char,int> AlreadyViewed; 
		
		for ( int i = 0; i < n; i++ ) /* ��������̬ÿһ��VN�� ��������VT����һ�� */
		{
			int index = Map[NewStates[map_id][i]], m = List[index].size();
			for ( int j = 0; j < m; j++ ) /* ����һ��VN��ȫ������VN--vt-->anotherVN�� */
			{
				char vt = List[index][j].second;
				if ( vt == '$' ) continue; /* �������ַ� */ 
				if( AlreadyViewed.count(vt) != 0 ) continue; /* ��֤ÿ����״̬ÿ���ս��ֻ��һ�� */
				else 
				{
					vector<string> T; /* T������״̬ */
					vt_move( vt, NewStates[map_id], T ); /* ͨ���հ�ת����ö�Ӧ��״̬*/
					AlreadyViewed[vt] = 1;  /* ��ֹ��ԭ״̬�����жϸ��ս�� */
					
					set<string>tmp ( T.begin(), T.end() );
    				T.assign( tmp.begin(), tmp.end() ); /* Tȥ�ء����� */
    				
    				int pos = find( NewStates.begin(), NewStates.end(), T ) - NewStates.begin();
    				if( pos == t ) /* �����״̬���в�����ǰ��״̬������ӽ�ȥ */
					{
						NewStates.push_back( T );
						NewMap[start+to_string(t)] = t++; 
						
						for ( int k = 0; k < T.size(); k++ ) /* NewSymbol */
						{
							if( Symbol[T[k]] != 0 ) 
								NewSymbol[start+to_string(t-1)] = Symbol[T[k]];
						}
					}
					list_arcs.push_back( { start+to_string(pos), vt } ); /* ���ܴ���״̬�Ƿ��Ѿ����ֹ�����Ҫ��������»� */			
							
				}
			} 
		} 
		NewList.push_back( list_arcs );
	}
	
	/* ��д MAP, LIST, Symbol, Start, FinalSet */
	Map.swap( NewMap ); 
	List.swap( NewList );
	Symbol.swap( NewSymbol );
//	Tag.swap( NewTag ); /* Tag ���ɸı� */
	tot = t;
	Start = start + "0";
	FinalSet.resize( tot, 0 );
	for ( int i = 0; i < t; i++ ) FinalSet[i] = false;
	for ( int i = 0; i < t; i++ )
	{
		bool flag = false;
		for ( int j = 0; j < NewStates[i].size(); j++ ) 
		{
			if( NewStates[i][j].find( "Final" ) == string::npos ) continue;
			else 
			{
				flag = true;
				break;
			}
		}
		if( flag )
			FinalSet[i] = true;
	}
	
	
	isDefinated = true; 

	/* ����鿴��״̬ */
//	for ( int i = 0 ; i < NewStates.size(); i++ )
//	{
//		cout << Map[start+to_string(i)] << " : ";
//		for ( int j = 0 ;  j < NewStates[i].size(); j++ )
//		{
//			cout << NewStates[i][j] << ' ';
//		}
//		cout << endl; 
//	} 
}

void NFA::e_closure( string status, vector<string>& T )
/* ����һ��״̬����$�հ� */
{

	if( Map.count( status ) == 0 ) 
	{
		cout << "ERROR: No such status!" << endl;
		return ;
	}
	
	int id, n;
	string tmp;
	stack<string> Stack;
	Stack.push( status );
	T.push_back( status );
	while ( !Stack.empty() ) 
	{
		tmp = Stack.top();
		Stack.pop();
		id = Map[tmp];
		n = List[id].size();
		for ( int i = 0; i < n; i++ )
		{
			if( List[id][i].second == '$' ) 
			{
				if( find( T.begin(), T.end(), List[id][i].first ) == T.end() )
				{
					Stack.push( List[id][i].first );
				    T.push_back( List[id][i].first );
				}	
			}	
		} 
	}
}

void NFA::vt_move( char vt, vector<string> V, vector<string>& T )
/* �ڱհ�V���ҵ�vt��ת���� ����״̬����T�� */
{
	int id, n;
	for ( int i = 0; i < V.size(); i++ )
	{
		id = Map[V[i]];
		n = List[id].size();
		for ( int j = 0 ; j < n; j++ )
		{
			if( List[id][j].second == vt )
			{
				e_closure( List[id][j].first, T );
				break;
			}
		  }  
	 } 
}

void NFA::print_state( vector<string> state )
/* ��DFA���������״̬������֤, ���������̨ */
{
	for ( int i = 0 ; i < state.size(); i++ ) 
		cout << state[i] << ' ' ;
	cout << endl; 
}

void NFA::cope_line( string line )
/* ÿ�δ���һ�У������ж�������ʽ*/
{
	if( line[0] == '/' && line[1] == '/' ) return ;
	if( line.find('|') == string::npos ) 
		cope_formula( line );
	else {
		string subhead, subrear;
		int pos = line.find( '>' ), j = pos+1;
		subhead = line.substr( 0, pos+1 ); 
		while ( line.find( '|', pos+1 ) != string::npos ) 
		{
			pos = line.find( '|', pos+1 );
			subrear = line.substr( j, pos-j ); 
			j = pos+1;
			cope_formula( subhead + subrear );
		}
		subrear = subrear = line.substr( j );
		cope_formula( subhead + subrear );
	}
}

void NFA::cope_formula( string formula ) 
/* ÿ�δ���һ������ʽ  */
{
	VN = get_next_VN( formula );
	VT = get_next_VT( formula );
	CharSet.insert( VT[0] );
	
	if( Map.count( VN ) == 0 ) 
		attach( VN );
	if( Map.count( formula ) == 0 ) 
		attach( formula );
	int id = Map[VN];
	List[id].push_back( psc(formula,VT[0]) );
	
	if( VN == Start ) return;
	if( Symbol.count(VN) == 0 && Tag.count(VN) ) 
		Symbol[VN] = Tag[VN];
	if( Symbol.count(formula) == 0 )
		Symbol[formula] = Symbol[VN];
}

void NFA::attach( string VN )
/* ��״̬�ӵ���ϵ�� */
{
	vector<psc> t;
	List.push_back( t );
	Map[VN] = tot++;
}

string NFA::get_first_VN( string formula )
/* �������ֻ�ȡ��һ��VN��������ʼ�� */
{
	int i = 0, j = 0;
	while ( j < formula.length() && formula[j] != ' ' && formula[j] != '-' )	j++;
	string sub = formula.substr( i,j );
	return sub;
}

string NFA::get_next_VN( string & formula )
/* �������еõ���һ��VN������line��ȥ��������֮���-> */
{
	int i = 0, j = 0;
	while ( j < formula.length() && formula[j] != ' ' && formula[j] != '-' )	j++;
	string sub = formula.substr( i,j );
	while ( j < formula.length() && ( formula[j] == ' ' || formula[j] == '-' || formula[j] == '>' ) ) 
	{
		j++;
		if( formula[j-1] == '>' ) break; 
	}
	while ( j < formula.length() && formula[j] == ' ' ) j++;
	formula.erase( i,j );
	return sub;
} 
 
string NFA::get_next_VT( string & formula )
/* �������еõ���һ��VT */
{
	int i = 0, j = 0;
	string sub;
	if( isalpha(formula[j]) == 1 )  /* �ж��ǲ��Ǵ�д��ĸ */
		sub = "$";
	else if( formula[j] == '\\' )
	{
		j++;
		if( formula[j] == '0' )
			sub = " ";
		else if( formula[j] == 't' )
			sub = "\t";
		else if( formula[j] == 'n' )
			sub = "\n";
		j++;
		formula.erase( i,j );
	} 
	else 
	{
		j++;
		sub = formula.substr( i,j );
		formula.erase( i,j );
	}
		

	if( formula == "" ) 
	{
		formula = "Final"+to_string(ftot); 
		ftot++;
	}
	return sub;
}

void NFA::get_Symbols( string line )
{
	int i = 0, id = 1;
	while( line[i] != '>' ) i++;
	i++;
	while( line[i] == ' ' ) i++;
	while( i < line.length() )
	{
		int j = i;
		while ( line[i] != '|' && i < line.length() ) i++;
		string tmp = line.substr( j, i-j );
		Tag[tmp] = id++;
		i++;
	}
}

void NFA::Print( string file2 )
{
	if( isDefinated ) 
		fout.open( file2, ios::app );
	else 
		fout.open( file2 );
	if( isDefinated ) fout << "DFA\n";
	else fout << "NFA\n";
	fout << "VN ( nonterminal character ) \n\t\t";
	for ( const auto p : Map )
		fout << p.first <<  ' ' ;
	
	fout << "\nVT ( terminal character ) \n\t\t";
	for ( const auto p : CharSet ) 
		fout << p << ' ';
	
	fout << "\nStart ( initial state ) \n\t\t" << Start;
	fout << "\nFinal ( final state ) \n\t\t";
	for ( int i = 0 ; i < tot; i++ ) 
	 	if( FinalSet[i] == true ) 
	 	{
	 		auto it = std::find_if(Map.begin(),Map.end(),[i](const std::map<std::string,int>::value_type item )->bool{return item.second == i;});
			fout << it->first << '\t';
		}
	fout << "\nSymbol \n\t\t" ;
	for ( const auto p : Tag ) fout << p.first << ' ' << p.second << '\t';
	fout << "\n\t\t";
	for ( const auto p : Symbol ) fout << p.first << ' ' << p.second << '\t';
	fout << "\n\n";
	
	int n, i;
	for ( const auto p : Map )
	{
		if( p.first == "" ) continue; 
		fout << p.first << ": ";
		i = p.second;
		n = List[i].size();
		if( n == 0 ) fout << "NULL";
		for ( int j = 0 ; j < n; j++ )  
		{
			fout << "( " << List[i][j].second << " , " << List[i][j].first << " ); ";
		}
		fout << "\n";
	}
	for ( int i = 0; i < 200; i++ ) fout << '-';
	fout << "\n";
	fout.close();
}

void NFA::Input( string file1 )
{
	freopen( file1.c_str(), "r", stdin );
	getline( cin, line );
	Start = get_first_VN( line );
	get_Symbols( line );
	do {
		cope_line( line );
		line = "";
		getline( cin, line );	
	} while ( line != "" );
	FinalSet.resize( tot, false ) ;
	for ( int i = 0; i < ftot; i++ )
		FinalSet[Map["Final"+to_string(i)]] = true;
//	fclose(stdin);
}

NFA::NFA() 
{
	tot = 0;
	ftot = 0;
	isDefinated = false;
}

