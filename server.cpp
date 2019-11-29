#include <bits/stdc++.h>
using namespace std;
#include "Trie.h"
#include "Request.h"
#include "Reply.h"
#include "Message.h"

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
		if (i >= 4 && i % 4 == 0)
		{
			t.InsertWord(token);
			cout << token << "\n";
		}
		i++;
	}
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
