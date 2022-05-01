#include <bits/stdc++.h>
using namespace std;

#define VN first
#define VT second
#define ERROR -1

typedef pair<string,int> psi;  /* VN -> 图中数字下标 */
typedef pair<string,char> psc;

class NFA
{
public:
	friend class LA; 
	
	ofstream fout;
	bool isDefinated; 
	int tot, ftot; 				/* 状态总数, 终态总数 */
	string line, VT, VN;		/* 输入行、非终结符、终结符 */
	string Start;				/* 初态 */
	vector<bool> FinalSet;		/* 终态集 */
	set<char> CharSet;   		/* VT集合 */
	map<string,int> Map;    	/* VN集合 */
	vector<vector<psc> > List;  /* 状态转换图 */
	map<string,int> Tag;  		/* 令牌集合 */
	map<string,int> Symbol; 	/* 各状态令牌表 */
	
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
	string line;	/* 输入行 */
	string word;   /* 当前处理串 */ 
	string T;		/* 当前状态 */
	int index;		/* 当前状态对应List中下标 */
	int id;			/* 输出编号 */
	
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
