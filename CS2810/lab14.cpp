#include <bits/stdc++.h>
using namespace std;

class Point{ // represents a point(x,y)
public:
    Point(int x=0, int y=0) : x(x), y(y) {} // constructor
    // overloading stream insertion and extraction
    friend istream& operator >>(istream& stream, Point& p){
        return stream >> p.x >> p.y;
    }
    friend ostream& operator <<(ostream& stream, Point& p){
        return stream << p.x << " " << p.y << "\n";
    }
    // distance between two points
    long double dist(Point& p){ 
        long double X = p.x - this->x;
        long double Y = p.y - this->y;
        X*=X; Y*=Y;
        return sqrtl(X + Y);
    }
    
    int x;
    int y;
};

class Island{
public:
    // virtual methods
    virtual istream& input(istream& stream) = 0;
    virtual ostream& output(ostream& stream) = 0;
    virtual Point& get_centre()=0;            // centre of the shape 
    virtual long double get_farthestDist()=0; // farthest point from the centre of the shape
    // overloading stream insertion and extraction
    friend istream& operator >>(istream& stream, Island* c){
        return c->input(stream);
    }
    friend ostream& operator <<(ostream& stream, Island *c){
        return c->output(stream);
    }
};

class Rectangle : public Island{
public:
    Rectangle(Point p1=Point(), Point p2=Point(), Point p3=Point(), Point p4=Point(), string id="") : p1(p1), p2(p2), p3(p3), p4(p4), ID(id) {} // constructor
    istream& input(istream& stream){
        stream >> ID >> p1 >> p2 >> p3 >> p4;
        centre = Point((p1.x+p2.x+p3.x+p4.x)/4, (p1.y+p2.y+p3.y+p4.y)/4);
        return stream;
    }
    ostream& output(ostream& stream){
        return stream << ID << "\n";
    }
    long double get_farthestDist(){
        // two corners will be the farthest - longer diagonal
        Point centre = get_centre();
        long double max_dist = max(centre.dist(p1), centre.dist(p2));
        max_dist = max(max_dist, centre.dist(p3)); 
        return max_dist;
    }
    Point& get_centre(){ 
        return centre;
    }
private:
    Point p1, p2, p3, p4, centre;
    string ID;
};

class Triangle : public Island{
public:
    Triangle(Point p1=Point(), Point p2=Point(), Point p3=Point(), string id="") : p1(p1), p2(p2), p3(p3), ID(id) {}
    istream& input(istream& stream){
        stream >> ID >> p1 >> p2 >> p3;
        centre = Point((p1.x+p2.x+p3.x)/3, (p1.y+p2.y+p3.y)/3); 
        return stream;
    }
    ostream& output(ostream& stream){
        return stream << ID << "\n";
    }
    long double get_farthestDist(){
        Point centre = get_centre();
        long double max_dist = max(centre.dist(p1), centre.dist(p2));
        max_dist = max(max_dist, centre.dist(p3));
        return max_dist;
    }
    Point& get_centre(){
        return centre;
    }
private:
    Point p1, p2, p3, centre;
    string ID;
};

class Circle : public Island{
public:
    Circle(Point p=Point(), int r=0, string id="") : centre(p), radius(r), ID(id) {}
    istream& input(istream& stream){
        return stream >> ID >> centre >> radius;
    }
    ostream& output(ostream& stream){
        return stream << ID << "\n";
    }
    long double get_farthestDist(){
        return (long double)radius;
    }
    Point& get_centre(){
        return centre;
    }
private:
    Point centre;
    int radius;
    string ID;
};

class IslandMethods{
public:
    IslandMethods(vector<Island*> vec) : islands(vec) {}

    int countBits(int mask){ // counts the number of 1's in the string - no.of islands in the path
        int count = 0;
        while(mask){
            count += (mask & 1);
            mask >>= 1;
        }
        return count;
    }

    pair<int, vector<int>> VisitEveryIsland(){
        int n = islands.size();
        vector<vector<int>> adjList(n);
        // create the adjacency list of graph - add edges if two islands overlap
        for(int i = 0; i < n-1; i++){
            for(int j = i+1; j < n; j++){
                if(isOverlapping(islands[i], islands[j])){
                    adjList[i].push_back(j);
                    adjList[j].push_back(i);
                }
            }
        }
        /*
        dp[mask][u] = true if there is a path mask that ends at u
        parent[mask][u] = v -> v led to u in the current path mask
        */
        vector<vector<bool>> dp(1 << n, vector<bool>(n, false)); 
        vector<vector<int>> parent(1 << n, vector<int>(n, -1));

        for (int i = 0; i < n; ++i){
            dp[1 << i][i] = true; // start a path at each island
        }

        for(int mask = 1; mask < (1 << n); ++mask){ // iterate through every subset
            for(int u = 0; u < n; ++u){             // ending at every island
                if(!dp[mask][u]) continue;          
                for(int v : adjList[u]){           
                    if(!(mask & (1 << v))){            // v is adjacent to u and v is not in path
                        int newMask = mask | (1 << v); // add v to the path
                        if(!dp[newMask][v]){ 
                            dp[newMask][v] = true;     // now we have path to v using islands in newMask
                            parent[newMask][v] = u;    // u led to v so parent of v is u in this path
                        }
                    }
                }
            }
        }

        int max_len = -1, end_node = -1, best_mask = -1;
        for(int mask = 0; mask < (1 << n); ++mask){
            for(int u = 0; u < n; ++u){
                if(dp[mask][u]){ // if path exists
                    int len = countBits(mask); // no.of islands in the path
                    if(len > max_len){
                        max_len = len;
                        end_node = u;
                        best_mask = mask;
                    }
                }
            }
        }

        vector<int> path;
        int curr = end_node;
        int mask = best_mask;

        while(curr != -1){ // start at end and follow the path using the parent of the node
            path.push_back(curr);
            int next = parent[mask][curr];
            mask ^= (1 << curr); // destroy current island in the path
            curr = next;
        }

        reverse(path.begin(), path.end()); // correct order of exploring the islands

        return {max_len, path};
    }
    void printLongestPath(){
        auto [max_len, path] = VisitEveryIsland();

        if(path.size() == islands.size()){ 
            cout << "YES\n";
        } 
        else{
            cout << "NO\n";
            cout << max_len << "\n"; 
        }
        for(int i : path) cout << islands[i]; // print the path
    }
    bool isOverlapping(Island* i1, Island* i2){
        double dist_c = i1->get_centre().dist(i2->get_centre()); // find the distance between two centres
        double dist = i1->get_farthestDist() + i2->get_farthestDist(); 
        return dist_c <= dist; 
    }
private:
    vector<Island*> islands;  // stores the pointer to all islands
};

int main(){
    int n; cin >> n;
    vector<Island*> islands;

    for(int i=0; i<n; i++){
        string shape, islandID; 
        cin >> shape;
        Island* ptr = nullptr;

        if(shape == "RECTANGLE"){
            ptr = new Rectangle();
        }
        else if(shape == "TRIANGLE"){
            ptr = new Triangle();
        }
        else if(shape == "CIRCLE"){
            ptr = new Circle();
        }
        else continue;

        cin >> ptr;
        islands.push_back(ptr);
    }

    IslandMethods island(islands);
    island.printLongestPath(); // this function will call VisitEveryIsland() and then print accordingly

    return 0;
}

