#include <iostream>
#include <sstream>
#include <vector>

class Pair {
public:
    std::string key{};
    int value{};
};

struct ListNode {
    int tail{}, head{}, weight{};
    ListNode *next {nullptr};
};

template <typename K, typename V>
struct Node {
    K key;
    V value;
    Node *next;
};

template <typename K, typename V>
class UnorderedMap
{

friend std::ostream &operator<<(std::ostream &os, const UnorderedMap<K,V> &umap);

private:
    const long prime = 7321;  //  Prime Number
    const long starting_char {32};  // For SPACE
    const long csize {90};  //  32-SPACE to 122-z (ASCII values)
    
    Node<K,V> **arr = nullptr;
    long size;
public:
    UnorderedMap();
    ~UnorderedMap();

    void insert(K key, V value);
    void erase(K key);
    bool find(K key);
    V &operator[](K key);

    void clear();
    long hasher(std::string key);
    void display();
    void toArray(std::vector<Pair> &array);
};

template <typename K, typename V>
UnorderedMap<K, V>::UnorderedMap()
    : size {0}
    {
        arr = new Node<K,V>*[prime];
    }

template <typename K, typename V>
UnorderedMap<K, V>::~UnorderedMap()
{
    delete arr;
}


template <typename K, typename V>
long UnorderedMap<K,V>::hasher(std::string key){
    // Karp-Rabin approach + Chaining
    long sum {0};
    size_t len {key.size()};
    size_t i{0};
    do{
        char ch = key.at(i);
        sum *= csize % prime;
        // sum += (ch - starting_char) % prime;
        sum += (ch - '0') % prime;
        i++;
    }while(i < len);

    return sum % prime;
}

template <typename K, typename V>
void UnorderedMap<K,V>::insert(K key, V value) {
    std::string key_string;
    std::ostringstream oss;
    oss << key;
    key_string = oss.str();

    long index = hasher(key_string);

    Node<K,V> *p = nullptr;
    p = arr[index];
    if(p != nullptr){
        while(p->key != key && p->next != nullptr){
            p = p->next;
        }
        if(p->key == key){
            p->value = value;
        }
        else{
            Node<K,V> *node = new Node<K,V>;
            node->key = key;
            node->value = value;
            node->next = nullptr;
            p->next = node;
            size++;
        }
    }
    else{
        Node<K,V> *node = new Node<K,V>;
        node->key = key;
        node->value = value;
        node->next = nullptr;
        arr[index] =  node;
        size++;
    }
}


template <typename K, typename V>
bool UnorderedMap<K,V>::find(K key) {
    std::string key_string;
    std::ostringstream oss;
    oss << key;
    key_string = oss.str();

    long index = hasher(key_string);

    Node<K,V> *p = arr[index];
    if(p != nullptr){
        while(p && p->key != key){
            p = p->next;
        }

        if(p && p->key == key){
            return true;
        }
    }

    return false;
}

template <typename K, typename V>
V &UnorderedMap<K,V>::operator[](K key) {
    std::string key_string;
    std::ostringstream oss;
    oss << key;
    key_string = oss.str();

    long index = hasher(key_string);

    Node<K,V> *p = arr[index];
    if(p != nullptr){
        while(p && p->key != key){
            p = p->next;
        }

        if(p && p->key == key){
            return p->value;
        }
        else{
            // Key doesn't exist in the chain
            insert(key, {});
            return operator[](key);
        }
    }
    else{
        // Key doesn't exist
        insert(key, {});   
        return operator[](key);
    }
}

template <typename K, typename V>
void UnorderedMap<K,V>::display() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Size = " << size << std::endl;
    std::cout << "- - - - - - - - - - - - - - - - - -" << std::endl;
    for(long i{0}; i<prime; i++){
        if(arr[i] != nullptr){
            Node<K,V> *p = arr[i];
            while(p!=nullptr){
                std::cout << "Key: " << p->key << " | Value: " << p->value << std::endl;
                p = p->next;
            }
        }
    }
    std::cout << "-----------------------------------" << std::endl;
}

template <typename K, typename V>
void UnorderedMap<K,V>::toArray(std::vector<Pair> &array) {
    for(long i{0}; i<prime; i++){
        if(arr[i] != nullptr){
            Node<K,V> *p = arr[i];
            while(p!=nullptr){
                Pair pair{};
                pair.key = p->key;
                pair.value = p->value;
                array.push_back(pair);
                p = p->next;
            }
        }
    }
}

void findPaths(UnorderedMap<std::string, int> &umap, ListNode **list, ListNode *node, std::string key, int weight, bool *visited) {
    if(node == nullptr){
        return;
    }

    int v = node->tail;
    // key += std::to_string(v);
    // weight += node->weight;

    ListNode *p = list[v];
    while(p!=nullptr) {
        if(!visited[p->head]){
            std::string temp_key = key+std::to_string(p->head);
            std::string reverse_key {};
            for(auto ch : temp_key){
                reverse_key = ch + reverse_key;
            }

            if(umap.find(temp_key) == false && umap.find(reverse_key) == false){
                umap.insert(temp_key, weight+p->weight);
            }
            visited[p->head] = true;
            findPaths(umap, list, list[p->head], temp_key, weight+p->weight, visited);
            visited[p->head] = false;
        }
        p = p->next;
    }
}

int main() {

    int n{}, m{};
    std::cin >> n >> m;

    ListNode *list[n];
    for(int i{0}; i<n; i++){
        list[i] = nullptr;
    }

    for(int i{0}; i<m; i++){
        int u, v, w;
        std::cin >> u >> v >> w;
        
        // (u,v)
        ListNode *e1 = new ListNode;
        e1->tail = u;
        e1->head = v;
        e1->weight = w;
        e1->next = nullptr;

        if(list[u]==nullptr){
            list[u] = e1;
        }
        else{
            e1->next = list[u];
            list[u] = e1;
        }

        // (v,u)
        ListNode *e2 = new ListNode;
        e2->tail = v;
        e2->head = u;
        e2->weight = w;
        e2->next = nullptr;

        if(list[v]==nullptr){
            list[v] = e2;
        }
        else{
            e2->next = list[v];
            list[v] = e2;
        }
    }

    int k{};
    std::cin >> k;

    UnorderedMap<std::string, int> umap{};

    bool *visited = new bool[n]();
    for(int i{0}; i<n; i++){
        visited[i]=true;
        findPaths(umap, list, list[i], std::to_string(i), 0, visited);
        visited[i]=false;
    }

    // umap.display();
    std::vector<Pair> vec{};
    umap.toArray(vec);
    // for(auto a : vec){
    //     std::cout << a.key << "==" << a.value << std::endl;
    // }

    std::sort(vec.begin(), vec.end(), [](Pair a, Pair b){ 
        if(a.value == b.value){
            return a.key < b.key;
        }
        else{
            return a.value < b.value;
        }
    });

    // std::cout << "=========After sort=========" << std::endl;
    // for(auto a : vec){
    //     std::cout << a.key << "==" << a.value << std::endl;
    // }

    int idx{0};
    while(idx < k && idx < vec.size()){
        std::string str {vec[idx].key};
        for(auto c : str)
            std::cout << c << " ";
        std::cout << std::endl;
        idx++;
    }
    

    return 0;
}