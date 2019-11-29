#include "Trie.h"
using namespace std;

Trie::Trie()
{
	root = new Node();
}

inline bool Trie::exists(Node *actual, const char &c)
{
	return actual->letters.find(c) != actual->letters.end();
}

void Trie::InsertWord(const string &word)
{
	Node *current = root;
	for (auto &c : word)
	{
		if (!exists(current, c))
			current->letters[c] = new Node();
		current = current->letters[c];
	}
	current->isWord = true;
}

bool Trie::FindWord(const string &word)
{
	Node *current = root;
	for (auto &c : word)
	{
		if (!exists(current, c))
			return false;
		current = current->letters[c];
	}
	return current->isWord;
}