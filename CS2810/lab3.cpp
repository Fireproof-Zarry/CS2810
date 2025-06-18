#include <bits/stdc++.h>
using namespace std;

class Graph{
public:
    Graph(int N){ 
        adjList.resize(N);  // N vertices in the graph
    }

    vector<set<int>> getEdges(){
        return adjList; 
    }

    void addEdge(int u, int v){
        int n = adjList.size();
        if (u >= n || v >= n || u < 0 || v < 0) return; //out of bounds
        adjList[u].insert(v);
        adjList[v].insert(u);
    }

    void removeEdge(int u, int v){
        int n = adjList.size();
        if (u >= n || v >= n || u < 0 || v < 0) return; //out of bounds
        adjList[u].erase(v);
        adjList[v].erase(u);
    }

    //operator overloading
    friend istream& operator>>(istream& istream, Graph& g){ // creating the graph by getting the edges
        int M;
        istream >> M;
        for(int i = 0; i < M; i++) {
            int u, v;
            istream >> u >> v;
            g.addEdge(u, v);
        }
        return istream;
    }

    friend ostream& operator<<(ostream& ostream, Graph& g){ // used to print the contents of the graph
        for(int i = 0; i < (int)g.adjList.size(); i++) {
            ostream << "Vertex " << i << ": ";
            for(int v : g.adjList[i]) {
                ostream << v << " ";
            }
            ostream << endl;
        }
        return ostream;
    }

    Graph& operator +(Graph &g2){ // union of the two graphs
        vector<set<int>> G2 = g2.getEdges();
        int n1 = adjList.size();
        int n2 = G2.size();

        if(n2 > n1){  
            adjList.resize(n2); //since V = V1 U V2
        }
        
        for(int i=0; i<n2; i++){ // we iterate over the vertices in G2
            for(int num : G2[i]){ // edge between i and G2[i] 
                if(i>=n1 || adjList[i].find(num) == adjList[i].end()){ // edges in G2 that are not in G1 are added to G1
                    addEdge(i, num);
                }
                
            }
        }
        
        return *this;
    }

    Graph& operator -(Graph &g2){ // intersection of the two graphs
        vector<set<int>> G2 = g2.getEdges();
        int n1 = adjList.size();
        int n2 = G2.size();

        if(n2 > n1){
            adjList.resize(n2); //V = V1 U V2
        }
        
    
        for(int i=0; i<n1; i++){ 
            vector<int> toRemove;
            
            for(int num : adjList[i]){  // we iterate through the edges in G1 and check whether they are present in G2 or not
                if(i >= n2 || G2[i].find(num) == G2[i].end()) toRemove.push_back(num);  //we remove it from G1 if it is not present in G2
            }
            
            for(int val : toRemove){ //removing the required edges
                removeEdge(i, val); 
            }
        }
        
        return *this;
    }

    Graph operator !(){ // complement of the graph
        int n = adjList.size();
        vector<set<int>> complementList(n); 

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j && adjList[i].find(j) == adjList[i].end()) {  // edge that is not present in G1
                    complementList[i].insert(j);
                }
            }
        }

        adjList = complementList;   //update the adjList to the new set of edges
        return *this;
    }

    bool bfs(int u, int v){ // for finding if we can reach a vertex from a starting vertex
        int n = adjList.size();
        if(u<0 || u >= n || v<0 || v >= n) return false; // out of bounds
        if(u == v) return true;  // vertex is reachable from itself

        // breadth first search
        vector<bool> visited(adjList.size(), false);
        queue<int> q;
        q.push(u);
        visited[u] = true;

        while(!q.empty()){
            int node = q.front();
            q.pop();

            for(int neighbor : adjList[node]){
                if(neighbor == v) return true;
                if(!visited[neighbor]){
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        return false; // not reachable
    }


    void isReachable(int u, int v){
        if(bfs(u,v)) cout << "Yes" << endl;
        else cout << "No" << endl;
        return;
    }


private:
    vector<set<int>> adjList; // THe graph is represented as its adjacency list
};

int main(){
    string t, T;
    cin >> t;
    int N;
    if(t == "Graph"){
        cin >> N;
        Graph g1(N);
        cin >> g1;  // creating G1

        while(cin >> t && t != "end"){
            if(t == "union"){
                int n1;
                cin >> T; 
                if(T == "Graph"){
                    cin >> n1; 
                    Graph g2(n1);
                    cin >> g2; // creating G2
                    g1 = g1 + g2; 
                }
            }
            else if(t == "intersection"){
                cin >> T; 
                if(T == "Graph"){   
                    int n2;
                    cin >> n2;
                    Graph g2(n2);
                    cin >> g2;
                    g1 = g1 - g2;
                }
            }
            else if(t == "complement"){
                g1 = !g1;
            }
            else if(t == "isReachable"){
                int u,v;
                cin >> u >> v;
                g1.isReachable(u, v);

            }
            else if(t == "printGraph"){
                cout << g1;
            }
            else if(t == "add_edge"){
                int u,v;
                cin >> u >> v;
                g1.addEdge(u, v);
            }
            else if(t == "remove_edge"){
                int u,v;
                cin >> u >> v;
                g1.removeEdge(u, v);
            }
        }
    }

    
    
    return 0;
}
