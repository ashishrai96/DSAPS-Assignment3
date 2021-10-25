#include <iostream>

struct Edge {
    int tail;
    int head;
    int weight;
};

struct ListNode {
    Edge *e;
    ListNode *next {nullptr};
};

void minHeapify(int *dist, int *pos, int *loc, int n, int i){
    int l = 2*i + 1;
    int r = 2*i + 2;

    int smallest = i;
    if(l < n && dist[pos[l]] < dist[pos[i]]){
        smallest = l;
    }

    if(r < n && dist[pos[r]] < dist[pos[smallest]]){
        smallest = r;
    }

    if(smallest != i){
        int t = pos[i];
        pos[i] = pos[smallest];
        pos[smallest] = t;

        loc[pos[i]] = i;
        loc[pos[smallest]] = smallest;
        minHeapify(dist, pos, loc, n, smallest);
    }
}

void buildMinHead(int *dist, int *pos, int *loc, int n){
    for(int i{n/2}; i>=0; i--){
        minHeapify(dist, pos, loc, n, i);
    }
}

int extractMin(int *dist, int *pos, int *loc, int &n){
    if(n < 0){
        return -1;
    }

    int min = pos[0];
    pos[0] = pos[n-1];
    loc[pos[0]] = 0;
    (n)--;
    minHeapify(dist,pos, loc, n, 0);
    return min;
}

void decreaseKey(int *dist, int *pos, int *loc, int n, int i) {
    while(i>0 && dist[pos[i/2]] > dist[pos[i]]){
        int t = pos[i];
        pos[i] = pos[i/2];
        pos[i/2] = t;

        loc[pos[i]] = i;
        loc[pos[i/2]] = i/2;
        i = i/2;
    }
}

void dijkstra(ListNode **list, int *dist, int *parent, int n, int source) {
    int *pos = new int[n];
    int *loc = new int[n];
    for(int i{0}; i<n; i++){
        dist[i] = i==source?0:INT_MAX;
        parent[i] = i==source?-1:INT_MAX;
        pos[i] = i;
        loc[i] = i;
    }

    int m {n};
    buildMinHead(dist, pos, loc, m);

    while(m > 0){
        int u = extractMin(dist, pos, loc, m);
        ListNode *p = list[u];
        while(p!=nullptr){
            if(dist[p->e->head] >= dist[u] + p->e->weight){
                if(dist[p->e->head] > dist[u] + p->e->weight || (dist[p->e->head] == (dist[u] + p->e->weight) && parent[p->e->head] > u)){
                    parent[p->e->head] = u;
                }
                dist[p->e->head] = dist[u] + p->e->weight;
                decreaseKey(dist, pos, loc, m, loc[p->e->head]);
            }
            p = p->next;
        }
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
        
        //  (u,v)
        Edge *a = new Edge;
        a->tail = u;
        a->head = v;
        a->weight = w;

        //  (v,u)
        Edge *b = new Edge;
        b->tail = v;
        b->head = u;
        b->weight = w;

        //  Adjaceny List
        ListNode *p = list[u];
        ListNode *n1 = new ListNode;
        n1->e = a;
        n1->next = nullptr;
        if(p==nullptr){
            list[a->tail] = n1;
        }
        else{
            n1->next = p;
            list[a->tail] = n1;
        }

        p = list[v];
        ListNode *n2 = new ListNode;
        n2->e = b;
        n2->next = nullptr;
        if(p==nullptr){
            list[b->tail] = n2;
        }
        else{
            n2->next = p;
            list[b->tail] = n2;
        }
    }

    int des{};
    std::cin >> des;

    int *dist = new int[n];
    int *parent = new int[n];
    dijkstra(list, dist, parent, n, des);

    // for(int i{0}; i<n; i++){
    //     std::cout << "Dist - " << dist[i] << " parent - " << parent[i] << std::endl;
    // }

    for(int i{0}; i<n; i++){
        if(i == des){
            continue;
        }

        int j {i};
        do{
            std::cout << j << " ";
            j = parent[j];
        }while(j != -1);
        std::cout << std::endl;
    }
    
    delete [] parent;
    delete [] dist;
    delete [] *list;

    return 0;
}