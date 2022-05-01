#include <bits/stdc++.h>
using namespace std;

#define VN first
#define VT second
#define ERROR -1

typedef pair<string,int> psi;  /* VN -> ͼ�������±� */
typedef pair<string,char> psc;

class NFA
{
public:
	friend class LA; 
	
	ofstream fout;
	bool isDefinated; 
	int tot, ftot; 				/* ״̬����, ��̬���� */
	string line, VT, VN;		/* �����С����ս�����ս�� */
	string Start;				/* ��̬ */
	vector<bool> FinalSet;		/* ��̬�� */
	set<char> CharSet;   		/* VT���� */
	map<string,int> Map;    	/* VN���� */
	vector<vector<psc> > List;  /* ״̬ת��ͼ */
	map<string,int> Tag;  		/* ���Ƽ��� */
	map<string,int> Symbol; 	/* ��״̬���Ʊ� */
	
	NFA();
	void Input( string file1 ); 
	void Print( string file2 );
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
};



class LA /* Lexical Analysis */
{
public:
	ofstream fout;
	string line;	/* ������ */
	string word;   /* ��ǰ���� */ 
	string T;		/* ��ǰ״̬ */
	int index;		/* ��ǰ״̬��ӦList���±� */
	int id;			/* ������ */
	
	LA();
	void analysis( NFA& nfa, string file3, string file2 );
	void read_next_VT( char ch, NFA& nfa, string file2 );
	void output( int tag, NFA& nfa, string file2 );
};

class isEqualALL 
{
public:
    explicit isEqualALL(char c) : ch(c) {}
    bool operator() (const std::pair<string, char>& element) const {
        return element.second == ch;
    }
private:
    const char ch;
};
