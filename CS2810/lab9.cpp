#include <bits/stdc++.h>
using namespace std;

class GraphAlgorithm{
public: 
    virtual ~GraphAlgorithm() {} // destructor
    virtual void Query(vector<set<int>> adjList) = 0;   // virtual function for queries
    virtual void set_hypeScores(vector<int> hype){};    // will be used in maxHype class
};

class isCycle : public GraphAlgorithm{
public:
    bool dfs(int node, vector<int>& visited, vector<int>& curPath, vector<set<int>>& adjList){
        visited[node] = 1;
        curPath[node] = 1;
        for(int neigh : adjList[node]){
            if(!visited[neigh] && dfs(neigh, visited, curPath, adjList)) return true;
            else if(curPath[neigh]) return true;
        }
        curPath[node] = 0;
        return false;
    }

    void Query(vector<set<int>> adjList){
        int n = adjList.size();
        vector<int> visited(n,0), curPath(n,0);
        for(int i=0; i<n; i++){
            if(!visited[i] && dfs(i, visited, curPath, adjList)){
                cout << "YES" << endl;
                return;
            }
        }
        cout << "NO" << endl;
    }
    void set_hypeScores(vector<int> hype){} //no use of this function here
};

class indepComponent : public GraphAlgorithm{
public:
    void dfs(int node, vector<set<int>>& adjList, vector<int>& visited, stack<int>& stk){
        visited[node] = 1;
        for(int neighbor : adjList[node]){
            if(!visited[neighbor]) dfs(neighbor, adjList, visited, stk);
        }
        stk.push(node);
    }

    void dfsTranspose(int node, vector<set<int>>& adjList, vector<int>& visited, int& count){
        visited[node] = 1;
        count++;
        for(int neighbor : adjList[node]){
            if(!visited[neighbor]) dfsTranspose(neighbor, adjList, visited, count);
        }
    }

    void Query(vector<set<int>> adjList){
        int n = adjList.size(), maxCardinality = 0, SCC = 0;
        stack<int> finishStack;
        vector<int> visited(n, 0);

        for(int i = 0; i < n; i++){
            if(!visited[i]) dfs(i, adjList, visited, finishStack);
        }

        vector<set<int>> transposed(n);
        for(int u = 0; u < n; u++){
            for(int v : adjList[u]) transposed[v].insert(u);
        }

        fill(visited.begin(), visited.end(), 0);
        while(!finishStack.empty()){
            int node = finishStack.top(); 
            finishStack.pop();
            if(!visited[node]){
                int count = 0;
                dfsTranspose(node, transposed, visited, count);
                SCC++;
                maxCardinality = max(maxCardinality, count);
            }
        }
        cout << SCC << " " << maxCardinality << endl;
    }
    void set_hypeScores(vector<int> hype){} // no use of this function here
};

class validOrder : public GraphAlgorithm{
public:
    void Query(vector<set<int>> adjList){ // finds valid topological order, print the node names (1-N)
        int n = adjList.size();
        // find indegree
        vector<int> indeg(n, 0);
        for(int i=0; i<n; i++){
            for(int x : adjList[i]){
                indeg[x]++;
            }
        }
        // use priority queue to store vertices 
        priority_queue<int, vector<int>, greater<int>> minHeap;
        for(int i=0; i<n; i++){
            if(indeg[i]==0) minHeap.push(i);
        }

        vector<int> order;  // contains the nodes in topological order
        while(!minHeap.empty()){
            int top = minHeap.top();
            minHeap.pop();
            order.push_back(top+1);   // node number = index+1
            for(int neigh : adjList[top]){
                indeg[neigh]--;
                if(indeg[neigh] == 0) minHeap.push(neigh);
            }
        }

        if((int)order.size() != n) cout << "NO" << endl; // no topo order
        else{
            for(int x : order) cout << x << " ";
            cout << endl;
        }

        return;
    }  
    void set_hypeScores(vector<int> hype){} // no use of this function here
};

class maxHype : public GraphAlgorithm{
public:
    void set_hypeScores(vector<int> hype){
        hypeScores = hype;
    }

    void Query(vector<set<int>> adjList){
        int n = adjList.size();
        vector<int> order, component(n, -1), visited(n, 0);
        stack<int> finishStack;

        for(int i = 0; i < n; i++){ // first DFS to get finish times
            if(!visited[i]) dfs1(i, adjList, visited, finishStack);
        }

        vector<set<int>> transposed(n); // reverse the graph
        for(int u = 0; u < n; u++){
            for(int v : adjList[u]) transposed[v].insert(u);
        }

        // second DFS to identify SCCs
        fill(visited.begin(), visited.end(), 0);
        int SCC = 0;
        vector<int> sccHype; // stores total hype for each SCC
        unordered_map<int, vector<int>> sccNodes; // maps SCC no. to nodes that are in it

        while(!finishStack.empty()){
            int node = finishStack.top(); 
            finishStack.pop();
            if(!visited[node]){
                sccHype.push_back(0);
                sccNodes[SCC] = {};
                dfs2(node, transposed, visited, component, SCC, sccHype, sccNodes);
                SCC++;
            }
        }

        // condensed SCC graph
        vector<set<int>> scc_graph(SCC);
        for(int u = 0; u < n; u++){
            for(int v : adjList[u]){
                if(component[u] != component[v]){
                    scc_graph[component[u]].insert(component[v]);
                }
            }
        }

        // find the longest path 
        vector<int> topoOrder, dp(SCC, 0);
        fill(visited.begin(), visited.end(), 0);

        for(int i = 0; i < SCC; i++){
            if(!visited[i]) topoSort(i, scc_graph, visited, topoOrder);
        }

        reverse(topoOrder.begin(), topoOrder.end());

        // find max path sum
        int maxHypeScore = 0;
        for(int node : topoOrder){
            dp[node] += sccHype[node];
            maxHypeScore = max(maxHypeScore, dp[node]);
            for(int neigh : scc_graph[node]){
                dp[neigh] = max(dp[neigh], dp[node]);
            }
        }
        cout << maxHypeScore << endl;
    }

private:
    vector<int> hypeScores;
    // helper functions
    void dfs1(int node, vector<set<int>>& adjList, vector<int>& visited, stack<int>& finishStack) {
        visited[node] = 1;
        for(int neigh : adjList[node]){
            if (!visited[neigh]) dfs1(neigh, adjList, visited, finishStack);
        }
        finishStack.push(node);
    }

    void dfs2(int node, vector<set<int>>& transposed, vector<int>& visited, vector<int>& component, 
              int sccId, vector<int>& sccHype, unordered_map<int, vector<int>>& sccNodes){
        visited[node] = 1;
        component[node] = sccId;
        sccHype[sccId] += hypeScores[node];
        sccNodes[sccId].push_back(node);
        for(int neigh : transposed[node]){
            if(!visited[neigh]) dfs2(neigh, transposed, visited, component, sccId, sccHype, sccNodes);
        }
    }

    void topoSort(int node, vector<set<int>>& dag, vector<int>& visited, vector<int>& order){
        visited[node] = 1;
        for(int neigh : dag[node]){
            if (!visited[neigh]) topoSort(neigh, dag, visited, order);
        }
        order.push_back(node);
    }
};


int main(){
    int N,M;
    cin >> N >> M;

    vector<int> hype_scores(N);
    for(int i=0; i<N; i++){
        cin >> hype_scores[i];
    }

    // add edges to the graph
    vector<set<int>> adjList(N);
    int u,v;
    for(int i=0; i<M; i++){
        cin >> u >> v;
        adjList[u-1].insert(v-1); // assuming its within bounds
    }

    GraphAlgorithm* ptr = NULL;  // ptr to base class

    int Q, qtype; // number of queries and query type
    cin >> Q;
   

    for(int i=0; i<Q; i++){
        cin >> qtype;
        delete ptr;

        if(qtype == 1){
            ptr = new isCycle();  // assign ptr to object based on the type of query
        }
        else if(qtype == 2){
            ptr = new indepComponent();
        }
        else if(qtype == 3){
            ptr = new validOrder();
        }
        else if(qtype == 4){
            ptr = new maxHype();
            ptr->set_hypeScores(hype_scores);
        }
        else continue;
        
        ptr->Query(adjList);
    }
    
    if(ptr) delete ptr;
    return 0;
}