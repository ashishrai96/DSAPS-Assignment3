#include <iostream>
#include <queue>

class Node {
public:
    char ch;
    Node *chars[26];
    bool isEnd {false};

    Node(){
        for(int i{0}; i<26; i++)
            chars[i] = nullptr;
    }
};

void insertToTrie(std::string str, Node *root) {
    if(str.length() == 0){
        return;
    }

    char ch = str[0];
    int idx = ch-'a';
    if(root->chars[idx] != nullptr) {

    }
    else {
        Node *p = new Node;
        p->ch = ch;
        root->chars[idx] = p;
    }

    if(str.length()==1){
        root->chars[idx]->isEnd = true;
        return;
    }

    insertToTrie(str.substr(1), root->chars[idx]);
}

void solve(int i, int j, int r, int c, char **arr, Node *root, std::string str, std::string *ans, unsigned long long* count) {

    char ch = arr[i][j];
    int idx = ch-'a';
    if(ch == '$' || idx<0 || root == nullptr) {
        return;
    }

    if(root->chars[idx] == nullptr){
        return;
    }

    if(root->chars[idx]->isEnd){
        // str += ch;
        ans[*count] = str+ch;
        (*count)++;
    }

    arr[i][j] = '$';

    if(i>0){
        solve(i-1, j, r, c, arr, root->chars[idx], (str + ch), ans, count);
    }
    if(i<r-1){
        solve(i+1, j, r, c, arr, root->chars[idx], (str + ch), ans, count);
    }
    if(j>0){
        solve(i, j-1, r, c, arr, root->chars[idx], (str + ch), ans, count);
    }
    if(j<c-1){
        solve(i, j+1, r, c, arr, root->chars[idx], (str + ch), ans, count);
    }

    arr[i][j] = ch;
}

void display(std::queue<Node*> que) {
    std::queue<Node*> q{};
    while(!que.empty()) {
        Node *p = que.front();
        que.pop();

        std::string res{};
        for(int i{0}; i<26; i++){
            if(p->chars[i] != nullptr) {
                res += p->chars[i]->ch;
                res = res + " (" + (p->chars[i]->isEnd?"1":"0") + ") ";
                q.push(p->chars[i]);
            }
        }

        if(res.length()!=0)
            std::cout << " [ " << res << "] (" << p->isEnd << ") ";
    }
    std::cout << "\n" << std::endl;
    if(!q.empty())
        display(q);
}

void merge(std::string *ans, int s, int m, int e) {
    int i {s}, j{m+1};
    std::string tmp[e+1];
    int k {0};

    while(i<=m && j<=e){
        if(ans[i] < ans[j]){
            std::string s = ans[i];
            tmp[k] = ans[i];
            i++;
        }
        else{
            std::string s = ans[j];
            tmp[k] = ans[j];
            j++;
        }
        k++;
    }

    if(i>m){
        while(j<=e){
            std::string s = ans[j];
            tmp[k] = ans[j];
            k++;
            j++;
        }
    }
    else{
        while(i<=m){
            std::string s = ans[i];
            tmp[k] = ans[i];
            i++;
            k++;
        }
    }

    for(int l{0}; l<k; l++)
        ans[l+s] = tmp[l];
}

void mergeSort(std::string *ans, int s, int e){
    if(s < e){
        int mid = s + (e-s)/2;
        mergeSort(ans, s, mid);
        mergeSort(ans, mid+1, e);
        merge(ans, s, mid, e);
    }
}

int main() {
    int r {}, c {};
    char **arr;
    int x {};
    std::string *words;

    std::cin >> r >> c;
    
    arr = new char*[r];
    for(int i{0}; i<r; i++){
        arr[i] = new char[c];
        for(int j{0}; j<c; j++){
            std::cin >> arr[i][j];
        }
    }

    std::cin >> x;
    words = new std::string[x];
    for(int i{0}; i<x; i++){
        std::cin >> words[i];
    }


    Node *root = new Node;
    root->ch = '/';
    for(int i{0}; i<x; i++){
        insertToTrie(words[i], root);
    }

    std::string *ans = new std::string[x];
    unsigned long long count {0};
    for(int i{0}; i<r; i++){
        for(int j{0}; j<c; j++){
            solve(i,j,r,c,arr,root, {}, ans, &count);
        }
    }
    
    mergeSort(ans, 0, count-1);

    int index {0};
    std::string result[count];
    for(int i{0};i<count;i++){
        if(i==0 || result[index-1] != ans[i]){
            result[index] = ans[i];
            index++;
        }
    }
    
    for(int i{0}; i<count; i++){
        std::cout << result[i] << " ";
    }std::cout << std::endl;
}