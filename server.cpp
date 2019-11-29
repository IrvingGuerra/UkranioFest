#include <bits/stdc++.h>
using namespace std;
#include "Trie.h"
#include "Request.h"
#include "Reply.h"
#include "Message.h"

string clearText(const string & s){
	string ans;
	for(unsigned char c : s){
		if('a' <= c && c <= 'z'){
			ans += c;
		}else if(c == 225) {
			ans += 'a';
		}else if(c == 233){
			ans += 'e';
		}else if(c == 237){
			ans += 'i';
		}else if(c == 243){
			ans += 'o';
		}else if(c == 250){
			ans += 'u';
		}else if(c == 241){
			ans += 'n';
		}
	}
	return ans;
}

int main(int argc, char *argv[])
{
	Message *msg;
	char respuesta = 1;

	Reply archivo(atoi(argv[1]));
	vector<string> libro;

	Trie t;
	ifstream input("10000_formas.TXT");
	string token;
	int i = 0;
	while (input >> token)
	{
		token = clearText(token);
		if (i >= 4 && i % 4 == 0)
		{
			t.InsertWord(token);
			cout << token << "\n";
		}
		i++;
	}
	return 0;
	while (1)
	{
		msg = archivo.getRequest();
		if (msg->operationId == Message::allowedOperations::book)
		{
			string seccionbook(msg->arguments);
			stringstream s(seccionbook);
			string palabra;
			// libro.clear();
			while (s >> palabra)
			{
				libro.push_back(palabra);
			}
			archivo.sendReply((char *)&respuesta, sizeof(respuesta));
		}
		else if (msg->operationId == Message::allowedOperations::newbook)
		{
			libro.clear();
			archivo.sendReply((char *)&respuesta, sizeof(respuesta));
		}
		else if (msg->operationId == Message::allowedOperations::count)
		{
			int *indices = (int*)msg->arguments;
			int contador = 0;
			for (int i= indices[0]; i<=indices[1]; i++){
				contador+= t.FindWord(libro[i]);
			}
			archivo.sendReply((char *)&contador, sizeof(contador));
		}
	}

	return 0;
}
