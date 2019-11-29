#include <bits/stdc++.h>
using namespace std;
#include "Trie.cpp"
#include "Request.h"
#include "Reply.h"
#include "Message.h"

int main(){
	Trie t;
	ifstream input("10000_formas.TXT");
	string token;
	int i = 0;
	while(input >> token){
		if(i >= 4 && i % 4 == 0){
			t.InsertWord(token);
			cout << token << "\n";
		}
		i++;
	}
	return 0;
}
