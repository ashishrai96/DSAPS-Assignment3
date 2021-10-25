#include <iostream>
#include <vector>
#include <iomanip>

struct Node{
    int row;
    int col;
};

void solve(std::vector<std::vector<int>> &vec, int i, int j, int r, int c, std::vector<std::vector<bool>> &visited) {

    // Update value
    if(i>0 && vec[i-1][j] != -1 && !visited[i-1][j]){
        vec[i-1][j] = std::min(vec[i][j]+1, vec[i-1][j]);
    }
    
    if(i<r && vec[i+1][j] != -1 && !visited[i+1][j]){
        vec[i+1][j] = std::min(vec[i][j]+1, vec[i+1][j]);
    }
    
    if(j>0 && vec[i][j-1] != -1 && !visited[i][j-1]){
        vec[i][j-1] = std::min(vec[i][j]+1, vec[i][j-1]);
    }
    
    if(j<c && vec[i][j+1] != -1 && !visited[i][j+1]){
        vec[i][j+1] = std::min(vec[i][j]+1, vec[i][j+1]);
    }

    //  Call others
    if(i>0 && vec[i-1][j] != -1 && !visited[i-1][j]){
        visited[i][j] = true;
        solve(vec, i-1, j, r, c, visited);
        visited[i][j] = false;
    }
    if(i<r && vec[i+1][j] != -1 && !visited[i+1][j]){
        visited[i][j] = true;
        solve(vec, i+1, j, r, c, visited);
        visited[i][j] = false;
    }
    
    if(j>0 && vec[i][j-1] != -1 && !visited[i][j-1]){
        visited[i][j] = true;
        solve(vec, i, j-1, r, c, visited);
        visited[i][j] = false;
    }
    
    if(j<c && vec[i][j+1] != -1 && !visited[i][j+1]){
        visited[i][j] = true;
        solve(vec, i, j+1, r, c, visited);
        visited[i][j] = false;
    }
}

int main() {

    size_t m{}, n{};

    std::cin >> n >> m;
    std::vector<int> sub(m, 0);
    std::vector<std::vector<int>> vec(n, sub);
    
    std::vector<bool> vis(m, false);
    std::vector<std::vector<bool>> visited(n, vis);

    std::vector<Node*> riders{};
    Node *a = new Node;
    
    for(size_t i{0}; i<n; i++){
        std::string str{};
        std::cin >> str;
        for(size_t j{0}; j<m; j++){
            char ch {str[j]};

            if(ch=='#'){
                vec[i][j] = -1;
            }
            else if(ch=='.'){
                vec[i][j] = INT_MAX;
            }
            else if(ch=='A'){
                vec[i][j] = INT_MAX;
                a->row = i;
                a->col = j;
            }
            else if(ch=='R'){
                vec[i][j] = INT_MAX;
                Node *p = new Node;
                p->row = i;
                p->col = j;
                riders.push_back(p);
            }

        }
    }

    vec[1][0] = 0;
    
    solve(vec, 1, 0, n, m, visited);

    // for(size_t i{0}; i<n; i++){
    //     for(size_t j{0}; j<m; j++){
    //         std::cout << std::setw(15) << std::left << vec[i][j] << " ";
    //     }std::cout << std::endl;
    // }

    int a_dist {vec[a->row][a->col]};
    bool isReachable {true};

    if(a_dist == INT_MAX) {
        isReachable = false;
    }
    else{
        for(auto &r : riders) {
            if(a_dist > vec[r->row][r->col]){
                isReachable = false;
                break;
            }
        }
    }

    if(isReachable){
        std::cout << "Yes\n" << a_dist << std::endl;
    }
    else{
        std::cout << "No" << std::endl;
    }


    for(auto &r : riders)
        delete r;
    
    delete a;

    return 0;
}