#include <bits/stdc++.h>
using namespace std;

#define SRConflict 1  /* Shift-Reduce-Conflict */
#define RRConflict 2  /* Reduce-Reduce-Conflict */
#define WRONGSTATE 3  /* ����û�е�״̬ */
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
	string express; 	/* ����ʽ */
	int pos;			/* λ�� */		 
	string lookahead;	/* ��ǰ������ */
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
	char Start;					/* ��ʼ�� */
	vector<char> VtSet;				/* �ս���� */
	vector<char> VnSet;				/* ���ս���� */
	vector<string> Productions;		/* ����ʽ */
	vector<vector<pair<int,char> > > DFA;		/* DFA */
	vector<vector<pair<char,int> > > Action;	/* Action�� */
	vector<vector<pair<char,int> > > Goto;		/* Goto�� */

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
	map<char,string> First;	/* ÿ�����ս����First���ļ��� */
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

