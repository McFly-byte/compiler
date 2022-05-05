#include "LR.h"

void LR1::build_DFA()
/* ��дLR1����Ŀ���淶��Ĺ���, ����DFA��Action��Goto */
{
	generate_firstset();
	
	/* vector<vector<pair<int,char> > > DFA; */
	vector<clcn> Collections;	/* ��Ŀ���淶��, �����״̬ */
	stack<clcn> Stack; 			/* ����ջ */
	clcn Closure; 				/* ĳһ״̬(��Ŀ��) */
	Project pro;
	string ex;

	for ( int i = 0; i < Productions.size(); i++ ) /* ���ó�̬ */
	{
		if( Productions[i][0] == Start ) 
		{
			pro.express = Productions[i];
			pro.lookahead = "#";
			pro.pos = 1;
			Closure.push_back( pro );
		}
	}
	enclosure( Closure );
	
	vector<pic> tmp; 
	DFA.push_back( tmp );
	Collections.push_back( Closure );
	Stack.push( Closure ); 
	
	while ( !Stack.empty() )
	{
		Closure = Stack.top();
//		print_state( Closure );
		Stack.pop();
		unordered_map<char,int> Vis;
		
		/* �������ݷ����±� */
		int index = find( all(Collections), Closure ) - Collections.begin();
		
		/* ������ת�Ƶ�V,��ֹ��Vt */
		for ( int i = 0; i < Closure.size(); i++ ) 
		{
			string ex = Closure[i].express;
			if( Closure[i].pos >= ex.length() ) continue;	/* ���ò���ʽδ��ͷ�� */
			char ch;
			ch = ex[Closure[i].pos]; 
			if( Vis.count(ch) != 0 ) continue;
			Vis[ch] = 1;
			clcn NewClosure;
			/* �ҵ�ͬһClosure����������ch��ת�� */
			for ( int j = i; j < Closure.size(); j++ ) 
			{
				if( Closure[j].pos < Closure[j].express.length() 
				 && Closure[j].express[Closure[j].pos] == ch )   
				{
					Project tmp;
					tmp.express = Closure[j].express;
					tmp.pos = Closure[j].pos+1;
					tmp.lookahead = Closure[j].lookahead;
					NewClosure.push_back( tmp );
				}
			}
			enclosure( NewClosure ); /* ������Զ����� */
			if( find( all(Collections), NewClosure ) == Collections.end() ) 
			{
				Collections.push_back( NewClosure ); 
				DFA.push_back( tmp );	/* ͬ��push_back,�±��Ӧ*/
				Stack.push( NewClosure );
			}
			/* ����ӵ�DFA�� */		
			int New_index =  find( all(Collections), NewClosure ) - Collections.begin();
			DFA[index].push_back( {New_index, ch} );
		}
	}
	
	/* ������ */
	/* vector<vector<pair<int,char> > > DFA; */
	cout << "��״̬" << endl;
	for ( int i = 0; i < DFA.size(); i++ )
	{
		cout << i << " : \n";
		print_state( Collections[i] );
	}
	cout << "\nDFA" << endl;
	for ( int i = 0; i < DFA.size(); i++ )
	{
		cout << i << " : ";
		for ( int j = 0 ; j < DFA[i].size(); j++ )
		{
			cout << " ( " << DFA[i][j].second << ',' << DFA[i][j].first << " ) ";
		}
		cout << "\n------------------------\n";
	}

	build_table( Collections );
	cout << "Action��" << endl; 
	for ( int i = 0; i < Action.size(); i++ )
	{
		cout << "״̬��" << i << "��";
		for ( int j = 0; j < Action[i].size(); j++ )
		{
			cout << "��" << Action[i][j].first << "��" << Action[i][j].second << "�� ";
		}
		cout << endl; 
	}
	cout << endl;
	cout << "Goto��" << endl;
	for ( int i = 0; i < Goto.size(); i++ )
	{
		cout << "״̬��" << i << "��";
		for ( int j = 0; j < Goto[i].size(); j++ )
		{
			cout << "��" << Goto[i][j].first << "��" << Goto[i][j].second << "�� "; 
		}
		cout << endl; 
	}
	cout << endl;
}

void LR1::build_table( vector<clcn>& Collections )
/* ����Action��Goto��*/
{
	/* vector<vector<pair<int,char> > > DFA; */
	/* vector<clcn> Collections;	 */
	/* vector<vector<pair<char,int> > > Action;	 */
	/* vector<vector<pair<char,int> > > Goto;	 */
	
	int n = DFA.size();
	Action.resize( n );
	Goto.resize( n );
	
	int state;
	char v;
	
	for ( int i = 0; i < n; i++ )
	{
		/* �ƽ� */
		for ( int j = 0; j < DFA[i].size(); j++ )
		{
			state = DFA[i][j].first;	
			v = DFA[i][j].second; 	
			if( isalpha( v ) == 1 ) /* ��д���ս�� */
			{
				Goto[i].push_back( {v,state} );
			}
			else /* ���ս�� */
			{
				Action[i].push_back( {v,state} ); 
			}
		}
		/* ��Լ */
		string expression;
		for ( int j = 0; j < Collections[i].size(); j++ )
		{
			if( Collections[i][j].pos >= Collections[i][j].express.length() )
			{
				expression = Collections[i][j].express;
				int pos = find( all(Productions), expression ) - Productions.begin(); /* �ڼ�������ʽ */
				pos = -pos; /* ��״̬�ĸ�����ʾ��Լ���޿ղ���ʽ����state==0ʱ�����ܹ�Լ��*/
				for ( int k = 0; k < Collections[i][j].lookahead.length(); k++ )
				{
					Action[i].push_back( {Collections[i][j].lookahead[k], pos} );
				} 
			}
		}
	}
}

void LR1::enclosure( clcn& Closure )
{
	bool flag = true;
	while ( flag )
	{
		flag = false;
		for ( int i = 0; i < Closure.size(); i++ )
		{
			string ex = Closure[i].express;
			if( Closure[i].pos >= ex.length() ) continue;
			char v = ex[Closure[i].pos]; /* ��������һ���ַ� */
			if( find( all(VnSet), v ) != VnSet.end() ) /* �Ƿ��ս�� */
			{
				for ( int j = 0; j < Productions.size(); j++ ) /* �������в���ʽ���Ѵ�Vn��ͷ�Ķ��Ž��� */
				{
					if( Productions[j][0] != v ) continue;
					
					Project tmp; 
					tmp.express = Productions[j];
					tmp.pos = 1;	
					tmp.lookahead = this->first( ex.substr(Closure[i].pos+1), Closure[i].lookahead );
					if( find( all(Closure), tmp ) == Closure.end() )
					{
						flag = true; 
						Closure.push_back( tmp ); 
					}
				}
			}
			
		}
	}
	sort( all(Closure) );  /* ������Զ����� */
}

string LR1::first( string remainder, string lookahead )
/* ������Ŀ�ָ����ұ߲����Լ�����Ŀ��ǰ��������������ִ���first��  */
{
	char ch;
	string ret = "";
	if( remainder.length() == 0 ) 
	{
		ret = lookahead;
	}
	else 
	{
		for ( int i = 0 ; i < remainder.length(); i++ ) /* ��������ÿ���ַ� */
		{
			ch = remainder[i];
			if(	find( all(VnSet), ch ) != VnSet.end() ) /* ���ս�� */
			{
				string tmp = First[ch];
				int pos = find(all(tmp),'$') - tmp.begin();
				if( pos < tmp.length() ) /* ���Ƴ��� */
				{
					tmp.erase(pos,1);
					ret += tmp;
					sort( all( ret ) );
					ret.erase( unique(all(ret)), ret.end() ); 	
					if( i == remainder.length()-1 ) /* �����Ƴ��� */
					{
						ret += lookahead;
						 sort( all( ret ) );
						ret.erase( unique(all(ret)), ret.end() ); 	
					}
				}
				else /* �����Ƴ��� */
				{
					ret += tmp;
					sort( all( ret ) );
					ret.erase( unique(all(ret)), ret.end() ); 
					break;	
				} 
			}
			else /* �ս�� */
			{
				if( find( all(ret), ch ) == ret.end() )
				{
					ret += ch;
					sort( all( ret ) );
				}
				break; 
			}
		}
	}
	return ret;
}

void LR1::generate_firstset()
/* ����ÿ�����ս����First���ļ��� */
{
	for ( int i = 0 ; i < VnSet.size(); i++ )
	{
		First[VnSet[i]] = "";
	} 
	
	bool flag = true; /* First�޸Ĺ��ͱ��ó�true */
	char ch;
	while ( flag ) 
	{
		flag = false;
		for ( int i = 0; i < VnSet.size(); i++ ) /* ����ÿ�����ս�� */
		{
			ch = VnSet[i];
			for ( int j = 0; j < Productions.size(); j++ ) /* �Ӳ���ʽ�����ҵ�����ch�� */
			{
				if( Productions[j][0] != ch ) continue;
				int k = 1;
				while ( k < Productions[j].length() )
				{
					/* ���� �ս�� �� �Ʋ����յķ��ս�� ���˳�ѭ�� */
					/* �����first�� */
					if( find(all(VtSet), Productions[j][k] ) == VtSet.end() ) /* ���ս�� */
					{
						
						string tmp = First[Productions[j][k]];
						int pos = find(all(tmp),'$') - tmp.begin();
						if( pos < tmp.length() ) /* ���ղ���ʽ�� ֻ�д���������������ʽ��һ���ַ�  */
						{
							tmp.erase( pos, 1 );
							if( !contains( First[ch], tmp ) )
							{
								First[ch] += tmp;
								sort( all( First[ch] ) );
								First[ch].erase( unique(all(First[ch])), First[ch].end() ); 
								flag = true;
							} 
							if( k == Productions[j].length()-1 && First[ch].find( "$" ) == string::npos ) 
							{
								First[ch] += '$';
								sort( all( First[ch] ) );
								First[ch].erase( unique(all(First[ch])), First[ch].end() ); 
								flag = true;
							}
						}
						else /* �����ղ���ʽ  */
						{
							if( !contains( First[ch], tmp ) )
							{
								First[ch] += tmp;
								sort( all( First[ch] ) );
								First[ch].erase( unique(all(First[ch])), First[ch].end() ); 
								flag = true;
							} 
							break;
						}
					} 
					else /* �ս�� */ 
					{
						/* ��first�� �ǲ���$������ֱ�Ӽ� */
						if( find( all( First[ch] ) , Productions[j][k] ) == First[ch].end() )
						{
							First[ch] += Productions[j][k];
							sort( all( First[ch] ) );
							flag = true; 
						} 
						break;	
					} 
					k++;
				}
			}
		}
		
	}
//	cout << "First�� " << endl; 
//	for ( const auto p : First ) 
//	{
//		cout << p.first << ' ' << p.second << endl;
//	}
//	cout << "-------------------" << endl;
}

bool LR1::contains( string a, string b )
/* ��b�е��ַ��Ƿ��ڴ�a�г��� */
{
	bool flag = true;
	for ( int j = 0; j < b.length(); j++ )
	{
		if( find( all(a), b[j] ) == a.end() )
		{
			flag = false;
			break;
		}
	}
	return flag;
}

int LR1::verify_conflict( clcn Closure )
/* ����Ƿ��г�ͻ */
{
	int shift = 0, reduce = 0;
	for ( int i = 0; i < Closure.size(); i++ )
	{
		if( Closure[i].pos >= Closure[i].express.length() ) reduce++;
		else shift++; 
	}
	if( reduce > 0 )
	{
		if( reduce > 1 ) return RRConflict;		 /* ��Լ-��Լ ��ͻ */
		else if( shift != 0 ) return SRConflict; /* �ƽ�-��Լ ��ͻ */
	}
	return 0;
}

void LR1::print_state( clcn C )
/* �����Ŀ���� */
{
	for ( int i = 0; i < C.size(); i++ )
	{
		
		for ( int j = 0; j < C[i].express.length(); j++ )
		{
			if( j == 1 ) cout << "->";
			if( j == C[i].pos ) cout << "��";
			cout << C[i].express[j]; 
		}
		if( C[i].pos >= C[i].express.length() ) cout << "��";
		cout << " , " << C[i].lookahead << "\n";
	}
	int error = verify_conflict( C );
	if( error ) 
		report( error );
	cout << "----------------------------------------------------\n" ;
}

void LR1::input( string File )
{
	LR::input( File );
	/* ����ʵ� */
	string line;
	getline( cin, line );
	int i = 0;
	bool t = false;
	string key;
	char value;
	while ( i < line.length() )
	{
		if( line[i] == ' ' || line[i] == ':' )
		{
			i++;
			continue;
		}
		else
		{	
			if( t ) 
			{
				value = line[i];
				i++;
				t = false;
				Dic[key] = value;
			}
			else 
			{
				int j = i;
				while( line[i] != ' ' && line[i] != ':' )
				{
					i++;
				}
				key = line.substr( j, i-j );
				t = true;
			}
		}
	}
	Dic["#"] = '#';
//	for ( const auto p : Dic )
//	{
//		cout << p.first << ' ' << p.second << endl;
//	}
}

void LR1::analysis( string FileIn, string FileOut )
/* ����ʷ���������������﷨��������� */
// undo: д�ò���ʽ���پ�����ô���� 
{
	cin.clear();
	freopen( FileIn.c_str(), "r", stdin );
	ofstream fout;
	fout.open( FileOut.c_str() );
	
	string line;
	int index;
	string word, label;
	
	stack<int> state;
	stack<char> sign;
	int st;
	
	state.push( 0 );
	
	/*
	map<string,string> Dic;
	vector<vector<pair<char,int> > > Action;
	vector<vector<pair<char,int> > > Goto;		
	*/
	
	bool f = false;
	while ( f || getline( cin, line) )
	{
		
		/* ���� */
		if( !f )
		{
			int i = 1;
			while ( line[i] != ',' ) i++;
			index = atoi( line.substr(1,i-1).c_str() );
			int j = ++i;
			while ( line[i] != ',' ) i++;
			if( i == j ) i++;
			word = line.substr( j, i-j );
			j = ++i;
			while ( line[i] != ')' ) i++;
			label = line.substr( j, i-j );
		}
		f = false;
		
		cout << index << ' ' << word << ' ' << label << endl;
		if( word == " " || word == "\t" || word == "\n" ) continue;

		if( word == "#" && state.top() == 0 ) 
		{
			cout << "Yes" << endl;
			return ;
		}
		
		/* ���� */ 
		char sn;
		if( Dic.count(word) != 0 ) sn = Dic[word];
		else if( label == "Token" )
		{
			sn = Dic["Token"];
		}
		else if( label == "Constant" )
			sn = Dic["Constant"];
			
		st = state.top();
		cout << index << ' ' << sn << ' ' << label << endl;
		if( find(all(VnSet),sn) != VnSet.end() ) /* ���ս�� - Goto */
		{
			bool t = true;
			for ( int k = 0; k < Goto[st].size(); k++ )
			{
				if( Goto[st][k].first == sn ) /* �ƽ� */
				{
					state.push( Goto[st][k].second );
					sign.push( sn );
//					cout << state.top() << '	' << sign.top() << endl;
					t = false;
					break;
				}
			}
			if( t ) 
			{
				report( WRONGSTATE );
				cout << "        Informations in State Stack(top to bottom): ";
				stack<int> tmp;
				while ( !state.empty() )
				{
					cout << state.top() << ' ';
					tmp.push( state.top());
					state.pop();
				}
				cout << endl;
				while ( !tmp.empty() )
				{
					state.push( tmp.top() );
					tmp.pop();
				}
			}
		}
		else 
		{
			bool t = true;
			for ( int k = 0; k < Action[st].size(); k++ )
			{
				if( Action[st][k].first == sn ) 
				{
					if( Action[st][k].second >= 0 ) /* �ƽ� */
					{
						state.push( Action[st][k].second );
						sign.push( sn );	
//						cout << state.top() << '	' << sign.top() << endl;
					}
					else /* ��Լ */
					{
						int pos = -Action[st][k].second;
						string express = Productions[pos];
						int num = express.length()-1;
						while ( num-- )
						{
//							cout << state.top() << '	' << sign.top() << endl;
							sign.pop();
							state.pop();
						}
						sign.push( express[0] );
						st = state.top();
						for ( int k = 0; k < Goto[st].size(); k++ )
						{
							if( Goto[st][k].first == express[0] ) /* �ƽ� */
							{
								state.push( Goto[st][k].second );
								break;
							}
						}
//						cout << state.top() << '	' << sign.top() << endl;
						f = true;
					}
					t = false;
					break;
				}
			}
			if( t )  /* ���󱨸� */
			{
				report( WRONGSTATE );
				cout << "        Informations in State Stack(top to bottom): ";
				stack<int> tmp;
				while ( !state.empty() )
				{
					cout << state.top() << ' ';
					tmp.push( state.top());
					state.pop();
				}
				cout << endl;
				while ( !tmp.empty() )
				{
					state.push( tmp.top());
					tmp.pop();
				}
			}
		}
		
	}
	cout << "No" << endl;
}

/* ---------------------------------------------------------------------------------------- */

void LR::collection_2_dfa()
/* ������Ŀ���淶��õ�DFA��Action��Goto */
{
	/* vector<vector<pair<int,char> > > DFA; */
	vector<vector<psi> > States; /* �����״̬ */
	vector<psi> Closure; /* ĳһ״̬ */
	stack<vector<psi> > Stack; 
	string expression;
	char ch;
	
	expression = Productions[0];
	Closure.push_back( {expression,1} );
	enclosure( Closure );
	States.push_back( Closure ); /* ͬһ״̬��DFA��States���±���ͬ */
	vector<pic> tmp; 
	DFA.push_back( tmp );
	
	Stack.push( Closure );
	
	while ( !Stack.empty() )
	{
		Closure = Stack.top();
		Stack.pop();
		unordered_map<char,int> Vis;
		
		/* �������ݷ����±� */
		int index = find( all(States), Closure ) - States.begin();
		
		/* ������ת�Ƶ�V,��ֹ��Vt */
		for ( int i = 0; i < Closure.size(); i++ ) 
		{
			if( Closure[i].second >= Closure[i].first.length() ) continue;	/* ���ò���ʽδ��ͷ�� */
			ch = Closure[i].first[Closure[i].second]; 
			if( Vis.count(ch) != 0 ) continue;
			Vis[ch] = 1;
			vector<psi> NewClosure;
			/* �ҵ�ͬһClosure����������ch��ת�� */
			for ( int j = i; j < Closure.size(); j++ ) 
			{
				if( Closure[j].second < Closure[j].first.length() 
				 && Closure[j].first[Closure[j].second] == ch )   
				{
					NewClosure.push_back( {Closure[j].first, Closure[j].second+1} );
				}
			}
			enclosure( NewClosure );
			if( find( all(States), NewClosure ) == States.end() ) 
			{
				States.push_back( NewClosure ); 
				DFA.push_back( tmp );	/* ͬ��push_back,�±��Ӧ*/
				Stack.push( NewClosure );
			}
			/* ����ӵ�DFA�� */		
			int New_index =  find( all(States), NewClosure ) - States.begin();
			DFA[index].push_back( {New_index, ch} );
		}
	}

	build_table( States );
	/* ������� */
//	for ( int i = 0; i < DFA.size(); i++ )
//	{
//		cout << i << endl;
//		for ( int j = 0; j < DFA[i].size(); j++ )
//		{
//			cout << " (" << DFA[i][j].first << " , " << DFA[i][j].second << ") ";
//		}
//		cout << endl;
//	 } 
}

void LR::build_table( vector<vector<psi> >& States )
/* ����DFA����Action���Goto�� */
/* �涨0״̬����̬���� */
/* Action����pci.second >0ʱΪ�ƽ���<0ʱΪ��Լ */
{
//	vector<vector<pair<int,char> > > DFA;		/* DFA */
//	vector<vector<pair<char,int> > > Action;	/* Action�� */
//	vector<vector<pair<char,int> > > Goto;		/* Goto�� */

	int n = DFA.size();
	Action.resize( n );
	Goto.resize( n );
	
	int state;
	char v;
	
	for ( int i = 0; i < n; i++ )
	{
		/* �ƽ� */
		for ( int j = 0; j < DFA[i].size(); j++ )
		{
			state = DFA[i][j].first;	
			v = DFA[i][j].second; 	
			if( isalpha( v ) == 1 ) /* ��д���ս�� */
			{
				Goto[i].push_back( {v,state} );
			}
			else /* ���ս�� */
			{
				Action[i].push_back( {v,state} ); 
			}
		}
		/* ��Լ */
		string expression;
		for ( int j = 0; j < States[i].size(); j++ )
		{
			if( States[i][j].second >= States[i][j].first.length() )
			{
				expression = States[i][j].first;
				int pos = find( all(Productions), expression ) - Productions.begin();
				pos = -pos;
				for ( int k = 0; k < VtSet.size(); k++ )
				{
					Action[i].push_back( {VtSet[k], pos} );
				} 
			}
		}
	}
}

void LR::enclosure( vector<psi>& Closure )
/* �ѱհ����ݹ������䣬���ı���״̬ */
{
	bool flag = true;
	unordered_map<char,int> Vis;
	while ( flag )
	{
		flag = false;
		for ( int i = 0; i < Closure.size(); i++ )
		{
			char v = Closure[i].first[Closure[i].second]; /* ��������һ���ַ� */
//			cout << "��ǰ�����������׸��ַ���" << v << endl;
			if( Vis.count(v) != 0 ) continue;
			Vis[v] = 1;
			if( find( all(VnSet), Closure[i].first[Closure[i].second] ) != VnSet.end() )
			{
				for ( int j = 0; j < Productions.size(); j++ ) /* �������в���ʽ���Ѵ�Vn��ͷ�Ķ��Ž��� */
				{
					if( Productions[j][0] != v ) continue;
					Closure.push_back( {Productions[j],1 } ); 
					flag = true; 
				}
			}
		}
	}
	sort( all(Closure) ); 
	int error = verify_conflict( Closure );
	if( error ) 
		report( error );
} 

int LR::verify_conflict( vector<psi> Closure )
{
	int shift = 0, reduce = 0;
	for ( int i = 0; i < Closure.size(); i++ )
	{
		if( Closure[i].second >= Closure[i].first.length() ) reduce++;
		else shift++; 
	}
	if( reduce > 0 )
	{
		if( reduce > 1 ) return RRConflict;		 /* ��Լ-��Լ ��ͻ */
		else if( shift != 0 ) return SRConflict; /* �ƽ�-��Լ ��ͻ */
	}
	return 0;
}

void LR::report( int error )
{
	time_t now = time(nullptr);    
	char* curr_time = ctime(&now); // Tue Apr 05 15:23:17 2022   
	if( error == RRConflict )
	{
		cout << "[ERROR] Reduce-Reduce Conflict found while compling." << endl;
		cout << "        " << ctime(&now) ;
	}
	else if( error == SRConflict )
	{
		cout << "[ERROR] Shift-Reduce Conflict found while compling." << endl;
		cout << "        " << ctime(&now) ;	
	}	
	else if( error == WRONGSTATE )
	{
		cout << "[ERROR] Wrong state visited while compling." << endl;
		cout << "        " << ctime(&now) ;	
	} 
} 

void LR::print_state( vector<psi> C )
{
	cout << "\n";
	for ( int i = 0; i < C.size(); i++ )
	{
		for ( int j = 0; j < C[i].first.length(); j++ )
		{
			if( j == 1 ) cout << "->";
			if( j == C[i].second ) cout << "��";
			cout << C[i].first[j]; 
		}
		cout << "\n";
	}
	cout << "------------------------\n" ;
}

void LR::input( string File )
/* ����2���ķ�����ʽ */
/* ��Ϊ��2���ķ����ʹ涨ÿ��Vt��Vn���ȶ�Ϊ1 */
/* Production��0λ�ǲ���ʽ�󲿣������ǲ���ʽ�Ҳ�  */
//undo: �ʷ������Ľ�����2���ķ�����δƥ������ 
{
	cin.clear();
	freopen( File.c_str(), "r", stdin );
	string line;
	bool b = true; 
	Productions.push_back( "" );
	while ( getline(cin,line) )
	{
		if( line.length() == 0 ) break; 
		if( b ) 
		{
			this->Start = line[0];
			b = false;
		}
		int i = 1, j = 1;
		char start = line[0];
		while( i < line.size() )
		{
			if ( line[i] == ' ' || line[i] == '>' || line[i] == '-' ) 
			{
				line.erase(i,1);
				continue;
			}
			else if( line[i] == '|' )
			{
				Productions.push_back( start+line.substr(j,i-j) );
				line.erase( i,1 );
				j = i;
				continue;
			}
			else 
			{
				if( line[i] >= 'A' && line[i] <= 'Z' ) /* ��д��ĸ�Ƿ��ս�� */
				{
					if( find( all(VnSet), line[i] ) == VnSet.end() )
					{
						VnSet.push_back( line[i] );
					 } 
				}
				else 
				{
					if( find( all(VtSet), line[i] ) == VtSet.end() )
					{
						VtSet.push_back( line[i] );
					 }
				}
			}
			i++;
		}
		Productions.push_back( start+line.substr(j,i-j) );
	}
}
