#include <bits/stdc++.h>
using namespace std;

template <typename T>
class PQ{ // priority queue class
public:
    // constructor
    PQ(int n){ 
        heap.resize(n);
    }
    PQ(){} 
    // methods  
    const size_t size(){
        return heap.size();
    }
    void push(const T& val){
        heap.push_back(val);
        pushUp(heap.size() - 1);
    } 
    void pop(){
        if(heap.empty()) return;
        swap(heap[0], heap.back());
        heap.pop_back();
        if(!heap.empty()) pushDown(0);
    }
    const T& top() const{
        return heap[0]; 
    }
    const bool empty() const{
        return heap.empty();
    }
private:
    vector<T> heap; 
    // helper functions for push and pop
    void pushUp(int index){
        while(index > 0){
            int parent = (index - 1)/2;
            if(heap[index] < heap[parent]){
                swap(heap[index], heap[parent]);
                index = parent; // Move up
            } 
            else{
                break;
            }
        }
    }
    void pushDown(int index){
        int n = heap.size();
        while(true){
            // children of node index are left and right
            int left = 2*index + 1;
            int right = 2*index + 2;
            int smallest = index;

            if(left<n && heap[left]<heap[smallest]) smallest = left;
            if(right<n && heap[right]<heap[smallest]) smallest = right;
            if(smallest != index){
                swap(heap[index], heap[smallest]); // the node is pushed down the subtree that has the smaller value at the root of that subtree
                index = smallest; // Move down
            } 
            else{
                break;
            }
        }
    }
};

class Node{
public:
    Node(string s, long long time, int p) : parity(p), t(time), name(s) {}
    Node(){}
    // overloading comparator
    bool operator <(const Node& b){
        return (this)->get_time() <= b.get_time();
    }
    // getters
    long long get_time() const{
        return t;
    }
    string get_name() const{
        return name;
    }
    int get_parity() const{
        return parity;
    }
    // setters
    void set_time(long long t){
        this->t = t;
    }
private:
    int parity; // even - 0, odd - 1
    long long t;     // time taken to reach the node from given source
    string name;  // room ID
};

class shortestPath{
public:
    long long djikstra(unordered_map<string, vector<pair<string, int>>> graph, string src, string dest){
        unordered_map<string, vector<int>> dist; // stores the room ID and {even path length, odd path lenght}
        for(auto& entry : graph){
            string room = entry.first;
            dist[room] = vector<int>(2, INT_MAX);
        }
        
        PQ<Node> minHeap;
        minHeap.push(Node(src, 0, 0));  // start at (src, 0, even)
        dist[src][0] = 0;
        
        while(!minHeap.empty()){
            Node currNode = minHeap.top(); 
            minHeap.pop();
            string room = currNode.get_name();
            long long time = currNode.get_time();
            int parity = currNode.get_parity();
            
            if(room == dest && parity == 0) return time; // reach destination and even path length
            
            for(auto& entry : graph[room]){
                string neigh = entry.first;
                long long w = entry.second;
                long long new_time = time+w;
                int new_par = 1-parity;
                
                if(new_time <  dist[neigh][new_par]){ // update if you find a lesser distance
                    dist[neigh][new_par] = new_time;
                    minHeap.push(Node(neigh, new_time, new_par));   
                }   
            }
        }
        return -1;
    }

};

int main(){
    int n,m; 
    cin >> n >> m;
    
    unordered_map<string, vector<pair<string, int>>> graph; // adjacency list of graph
    vector<string> rooms(n);
    
    for(int i = 0; i < n; i++){
        cin >> rooms[i];
        graph[rooms[i]] = {}; 
    }
    
    for(int i = 0; i < m; i++){
        string a, b;
        int w;
        cin >> a >> b >> w;
        graph[a].emplace_back(b, w);
        graph[b].emplace_back(a, w);
    }
    
    string src_id, dest_id;
    cin >> src_id >> dest_id;
    
    shortestPath p;
    cout << p.djikstra(graph, src_id, dest_id) << endl;

    return 0;
}