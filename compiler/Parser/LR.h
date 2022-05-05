#include <bits/stdc++.h>
using namespace std;

#define SRConflict 1  /* Shift-Reduce-Conflict */
#define RRConflict 2  /* Reduce-Reduce-Conflict */
#define WRONGSTATE 3  /* 表中没有的状态 */
//#define all(X) X.begin(),X.end() 

typedef pair<string,string> pss;
typedef pair<string,int> psi;
typedef pair<int,char> pic;
typedef pair<char,char> pcc;
typedef pair<char,int> pci;
typedef vector<string> vecs;
typedef vector<char> vecc;
typedef vector<struct Project> clcn;

struct Project
{
	string express; 	/* 产生式 */
	int pos;			/* 位置 */		 
	string lookahead;	/* 向前搜索符 */
	bool operator < (const Project x) const 
	{  
        if( express == x.express )
        {
        	if( pos == x.pos ) 
    		{
    			return lookahead < x.lookahead;
			}
			return pos < x.pos;
		}
		return express < x.express;
    }   
	bool operator == ( const Project x ) const 
	{
		if( express == x.express && pos == x.pos && lookahead == x.lookahead ) return true;
		return false;
	} 
};

class LR
{
public:
	char Start;					/* 起始符 */
	vector<char> VtSet;				/* 终结符集 */
	vector<char> VnSet;				/* 非终结符集 */
	vector<string> Productions;		/* 产生式 */
	vector<vector<pair<int,char> > > DFA;		/* DFA */
	vector<vector<pair<char,int> > > Action;	/* Action表 */
	vector<vector<pair<char,int> > > Goto;		/* Goto表 */

	virtual void input( string File );
	void analysis( string FileIn, string FileOut );
	void enclosure( vector<psi>& Closure );
	void collection_2_dfa();
	int verify_conflict( vector<psi> Closure );
	void report( int Error );
	void print_state( vector<psi> C );
	void build_table( vector<vector<psi> >& States ); 
};

class LR1 : public LR 
{
public:
	map<char,string> First;	/* 每个非终结符的First集的集合 */
	map<string,char> Dic;
	
	void generate_firstset(); 
	bool contains( string a, string b );
	string first( string remaider, string lookahead );
	
	virtual void input( string File );
	void analysis( string FileIn, string FileOut );
	void build_DFA();
	void build_table( vector<clcn>& Collections ); 
	void enclosure( clcn& C );
	int  verify_conflict( clcn C );
	void print_state( clcn C );  
	
};

