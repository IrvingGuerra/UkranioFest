struct Node{
  	bool isWord = false;
	unordered_map<char, Node*> letters;
};

struct Trie{
	Node* root;

	Trie(){
		root = new Node();
	}

	inline bool exists(Node * actual, const char & c){
		return actual->letters.find(c) != actual->letters.end();
	}

	void InsertWord(const string& word){
		Node* current = root;
		for(auto & c : word){
			if(!exists(current, c))
				current->letters[c] = new Node();
			current = current->letters[c];
		}
		current->isWord = true;
	}

	bool FindWord(const string& word){
		Node* current = root;
		for(auto & c : word){
			if(!exists(current, c))
				return false;
			current = current->letters[c];
		}
		return current->isWord;
	}
};