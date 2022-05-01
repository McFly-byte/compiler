#include <bits/stdc++.h>

using namespace std;

void a()
{
	ofstream fout;
	fout.open( "test_output.txt", ios::app );
	fout << "a" << endl;
}

void b()
{
	ofstream fout;
	fout.open( "test_output.txt", ios::app );
	fout << "b" << endl;
}

int main()
{
	a();
	b();
 } 
