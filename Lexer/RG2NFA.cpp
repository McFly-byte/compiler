/**
 *    author:  ������-919106840423 
 *    created: 2022 
 *	  describ:  
 			    ���������ķ������������ķ�����nfa 
				nfaתΪdfa 
**/ 

#include <bits/stdc++.h>
using namespace std;

#define VN first
#define VT second

typedef pair<string,int> psi;  /* VN -> ͼ�������±� */
typedef pair<string,char> psc;

class NFA
{
public:
	int tot, f_t; 				/* ״̬����, ��̬���� */
	string line, VT, VN;		/* �����С����ս�����ս�� */
	string Start;				/* ��̬ */
	vector<bool> FinalSet;		/* ��̬�� */
	set<char> CharSet;   		/* VT���� */
	map<string,int> Map;    	/* VN���� */
	vector<vector<psc> > List;  /* ״̬ת��ͼ */
	map<string,int> Tag;  		/* ���Ƽ��� */
	map<string,int> Symbol; 	/* ��״̬���Ʊ� */
	
	NFA();
	void Input(); 
	void Print();
	void cope_line( string line );
	void cope_formula( string formula );
	string get_first_VN( string line );
	string get_next_VN( string& line );
	string get_next_VT( string& line );
	void get_Symbols( string line );
	void attach( string VN );
	void nfa_2_dfa();
	void e_closure( string status, vector<string>& V );
	void vt_move( char vt, vector<string> V_closure, vector<string>& T ); 
	void print_state( vector<string> state );
}nfa;

ofstream fout;

int main()
{
	freopen( "test_input.txt", "r", stdin );

    fout.open ("test_output.txt");
	
	nfa.Input();
	nfa.Print();
	nfa.nfa_2_dfa();
	nfa.Print();
}

// --------------------------------------------------------------------------------------------

void NFA::nfa_2_dfa()
/* NFA��ͼת��Ϊ DFA��ͼ */
{
	map<string,int> NewMap;    	/* VN���� */
	vector<vector<psc> > NewList;  /* ״̬ת��ͼ */
	
	vector<vector<string> > NewStates;  /* ��״̬���� */
	
	int n, id, t = 0;
	string start = "T";
	vector<string> closure;
	
	e_closure( Start, closure ); /* ״̬V��$�հ� */
	sort( closure.begin(), closure.end() );
	NewStates.push_back( closure );
	NewMap[start+to_string(t)] = t++;
	
	for ( int map_id = 0; map_id < t; map_id++ )
	{
		vector<psc> list_arcs;

		n = NewStates[map_id].size(); /* ��ǰ����״̬�� */
		unordered_map<char,int> AlreadyViewed;
		for ( int i = 0; i < n; i++ ) /* ������״̬ÿһ��VN�� ��������VT����һ�� */
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
					vt_move( vt, NewStates[map_id], T ); 
					AlreadyViewed[vt] = 1;
					
					set<string>tmp ( T.begin(), T.end() );
    				T.assign( tmp.begin(), tmp.end() ); /* Tȥ�ء����� */
    				
    				int pos = find( NewStates.begin(), NewStates.end(), T ) - NewStates.begin();
    				if( pos == t )
					{
						NewStates.push_back( T );
						NewMap[start+to_string(t)] = t++;
					}
					list_arcs.push_back( { start+to_string(pos), vt } );				
							
				}
			} 
		} 
		NewList.push_back( list_arcs );
	}
	
	/* ��д MAP �� LIST */
	Map.swap( NewMap ); 
	List.swap( NewList );
	tot = t;
	Start = start + "0";
	FinalSet.resize( tot, 0 );
	for ( int i = 0; i < t; i++ ) FinalSet[i] = false;
	for ( int i = 0; i < t; i++ )
	{
		if( find( NewStates[i].begin(), NewStates[i].end(), "Final" ) == NewStates[i].end() ) 
			continue;
		else 
		{
			FinalSet[i] = true;
		}
	}
	
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
	if( isalpha(formula[j]) != 1 ) {
		j++;
		sub = formula.substr( i,j );
		formula.erase( i,j );
	}
	else 
		sub = "$";

	if( formula == "" ) formula = "Final";
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

void NFA::Print()
{
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
	fout << "\nTag \n\t\t" ;
	for ( const auto p : Tag ) fout << p.first << ' ' << p.second << '\t';
	fout << "\n";
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
}

void NFA::Input()
{
	getline( cin, line );
	Start = get_first_VN( line );
	get_Symbols( line );
	do {
		cope_line( line );
		line = "";
		getline( cin, line );	
	} while ( line != "" );
	FinalSet.resize( tot, false ) ;
	FinalSet[Map["Final"]] = true;
}

NFA::NFA() 
{
	tot = 0;
	attach( "Final" );
}
