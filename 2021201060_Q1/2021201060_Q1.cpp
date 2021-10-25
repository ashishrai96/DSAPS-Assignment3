#include <iostream>
#include <map>
#include <queue>
#include <vector>

struct Node {
    std::map<char, Node*> chars {};
    bool isEnd {false};
};

void insertIntoTrie(std::string word, Node* root){
    char ch = word[0];
    auto it = root->chars.find(ch);

    if(word.length() == 0){
        root->isEnd = true;
        return;
    }

    if(it != root->chars.end()){
        //  Char exists
        insertIntoTrie(word.substr(1), it->second);
    }
    else{
        //  char doesnot exist
        Node *q = new Node;
        root->chars.insert(std::make_pair(ch, q));
        insertIntoTrie(word.substr(1), q);
    }
}

int spellCheck(std::string str, Node *root){

    if(str.length() == 0){
        return (root->isEnd? 1 : 0);
    }

    auto it = root->chars.find(str[0]);
    if(it != root->chars.end()){
        //  Char exists
        return spellCheck(str.substr(1), it->second);
    }
    else{
        //  Char doesn't exist
        return 0;
    }
}

void dfs(std::vector<std::string> &vec, std::string str, Node *root){

    if(root->isEnd){
        vec.push_back(str);
        return;
    }

    auto it = root->chars.begin();
    while(it != root->chars.end()) {
        // str += it->first;
        dfs(vec, str+it->first, it->second);
        it++;
    }
}

std::vector<std::string> autoComplete(std::string str, Node *root) {
    std::vector<std::string> vec{};

    Node *p = root;
    size_t i {0};
    while(p != nullptr && i < str.length()){
        auto it = p->chars.find(str[i]);
        if(it != p->chars.end()){
            //  Char matches
            p = it->second;
            i++;
        }
        else{
            //  Char doesn't match
        }
    }

    dfs(vec, str, p);
    return vec;
}

std::vector<std::string> autoCorrect(std::string str, Node *root) {

}

void displayMenu() {
    std::cout << "\n-----------------------------" << std::endl;
    std::cout << "Press 1 for spell check" << std::endl;
    std::cout << "Press 2 for auto complete" << std::endl;
    std::cout << "Press 3 for auto correct" << std::endl;
    std::cout << "Press any num to exit" << std::endl;
    std::cout << "-----------------------------\n" << std::endl;
}

void display(std::queue<Node*> que) {
    std::queue<Node*> q{};
    while(!que.empty()) {
        Node *p = que.front();
        que.pop();

        std::cout << " [ ";
        auto it = p->chars.begin();
        while(it != p->chars.end()) {
            std::cout << it->first << " ";
            q.push(it->second);
            it++;
        }std::cout << "] ";
    }
    std::cout << "\n" << std::endl;
    if(!q.empty())
        display(q);
}

int main() {
    size_t n{};
    Node *trie = new Node;
    std::cin >> n;
    for(size_t i{0}; i<n; i++) {
        std::string word{};
        std::cin >> word;
        insertIntoTrie(word,  trie);
    }

    int ai;
    std::string ti {};
    do{
        displayMenu();
        std::cin >> ai;
        switch(ai){
            case 1:
            {
                std::cin >> ti;
                std::cout << "Probability of word's existence = " << spellCheck(ti,trie) << std::endl;
            }
            break;
            case 2:
            {
                std::cin >> ti;
                std::vector<std::string> output =  autoComplete(ti, trie);
                std::cout << "The suggestions :" <<std::endl;
                for(auto a:output){
                    std::cout << a << " ";
                }std::cout <<std::endl;
            }
            break;
            case 3:
            {
                std::cin >> ti;
                std::vector<std::string> output = autoCorrect(ti, trie);
                std::cout << "The suggestions :" <<std::endl;
                for(auto a:output){
                    std::cout << a << " ";
                }std::cout <<std::endl;
            }
            break;
            default:
                exit(0);
            break;
        }
    }while(true);
    
    std::queue<Node*> q;
    q.push(trie);
    display(q);
    return 0;
}