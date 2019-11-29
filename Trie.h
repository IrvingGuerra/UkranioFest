#include <bits/stdc++.h>

struct Node{
  	bool isWord = false;
	std::unordered_map<char, Node*> letters;
};

class Trie{
    public:
        Trie();
        inline bool exists(Node * actual, const char & c);
        void InsertWord(const std::string& word);
        bool FindWord(const std::string& word);
    private:
        Node* root;
};
