#include <bits/stdc++.h>
using namespace std;

class Comparator{ 
public:
    Comparator(bool isAscending) : ascending(isAscending) {}
    bool ascending;
    bool operator()(long long a, long long b) const{
        return ascending ? (a < b) : (a > b);
    }
};

class Point{ 
public:
    int x, y; 
    int index;
}; 

static bool compareByX(const Point& a, const Point& b) {
    return a.x < b.x;
}

static bool compareByY(const Point& a, const Point& b) {
    return a.y < b.y;
}

class GC{ 
public:
    long long countAndMerge(vector<long long>& arr, int l, int m, int r, const Comparator &comp) {
        int n1 = m - l + 1, n2 = r - m;

        vector<long long> left(n1), right(n2);
        for(int i = 0; i < n1; i++)
            left[i] = arr[i + l];
        for(int j = 0; j < n2; j++)
            right[j] = arr[m + 1 + j];

        long long res = 0;
        int i = 0, j = 0, k = l;
        if(comp.ascending){
            while(i < n1 && j < n2){
                if(left[i] <= right[j]){
                    arr[k++] = left[i++];
                }
                else{
                    arr[k++] = right[j++];
                    res += (n1 - i);
                }
            }
        }
        else{
            while(i < n1 && j < n2){
                if(left[i] >= right[j]){
                    arr[k++] = left[i++];
                }
                else{
                    arr[k++] = right[j++];
                    res += (n1 - i);
                }
            }
        }
        
        while(i < n1) arr[k++] = left[i++];
        while (j < n2) arr[k++] = right[j++];

        return res;
    }
    long long mergeSort(vector<long long>& arr, int l, int r, const Comparator& comp){
        long long res = 0;
        if(l < r){
            int m = (r + l)/2;

            res += mergeSort(arr, l, m, comp);
            res += mergeSort(arr, m + 1, r, comp);

            res += countAndMerge(arr, l, m, r, comp);
        }
        return res;
    }
    long long countInversions(){ // n -> length of flattened form of matrix
        vector<long long> flattened_matrix;
        for(int i=0; i < (int)QCB.size(); i++){
            for(int j=0; j < (int)QCB[i].size(); j++){
                flattened_matrix.push_back(QCB[i][j]); 
            }
        }
        
       return mergeSort(flattened_matrix, 0, flattened_matrix.size() - 1, Comparator(1));

    }
    
    void display(){
        for(int i=0; i < (int)QCB.size(); i++){
            for(int j=0; j < (int)QCB.size(); j++){
                cout << QCB[i][j] << " ";
            }
            cout << endl;
        }
    }

    long double dist(Point p1, Point p2){ 
        return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)); 
    } 

    long double bruteForce(vector<Point> &P, int n, pair<Point, Point>& bestPair){ 
        long double minDist = DBL_MAX; 
        for(int i = 0; i < n; ++i){
            for(int j = i+1; j < n; ++j){ 
                long double d = dist(P[i], P[j]);
                if(d < minDist){  
                    minDist = d;
                    bestPair = {P[i], P[j]};
                }
            }
        }
        return minDist; 
    }



    long double min(long double x, long double y){ 
        return (x < y)? x : y; 
    } 

    long double stripClosest(vector<Point>& strip, int size, double d, pair<Point, Point>& bestPair){ 
        sort(strip.begin(), strip.end(), compareByY); //sort by y value
        long double minDist = d; 

        for(int i = 0; i < size; ++i){
            for(int j = i+1; j < size && (strip[j].y - strip[i].y) < minDist; ++j){
                long double distVal = dist(strip[i], strip[j]);
                if(distVal < minDist){
                    minDist = distVal;
                    bestPair = {strip[i], strip[j]};
                }
            }
        }
        return minDist; 
    } 

    long double closestUtil(vector<Point>& P, int n, pair<Point, Point>& bestPair){ 
        if (n <= 3) 
            return bruteForce(P, n, bestPair); 

        int mid = n/2;  
        Point midPoint = P[mid]; 
        
        vector<Point> left(P.begin(), P.begin() + mid);
        vector<Point> right(P.begin() + mid, P.end());
    
        pair<Point, Point> leftPair, rightPair;
        long double dl = closestUtil(left, mid, leftPair); 
        long double dr = closestUtil(right, n - mid, rightPair); 

        long double d = min(dl, dr); 
        bestPair = (d == dl) ? leftPair : rightPair;
 
        vector<Point> strip; 
        for(int i = 0; i < n; i++){
            if(abs(P[i].x - midPoint.x) < d){
                strip.push_back(P[i]);
            }
        }

        pair<Point, Point> stripPair;
        long double stripMin = stripClosest(strip, strip.size(), d, stripPair);
    
        if(stripMin < d){
            bestPair = stripPair;
            return stripMin;
        }
        
        return d; 
    } 

    long double closest(vector<Point> &P){ 
        int n = P.size();
        sort(P.begin(), P.end(), compareByX); // sort by x
        pair<Point, Point> bestPair;

        long double minDist = closestUtil(P, n, bestPair);
        
        if(bestPair.first.index > bestPair.second.index) swap(bestPair.first, bestPair.second);

        cout << bestPair.first.x << " " << bestPair.first.y << " " << bestPair.second.x << " " << bestPair.second.y << endl;
        return minDist; 
    }      
      
    void closestPair(){
        int size = players_position.size();
        vector<Point> P(size);
        for(int i = 0; i < size; ++i){
            P[i].x = players_position[i][0];
            P[i].y = players_position[i][1];
            P[i].index = i;
        }
        closest(P);    
    }

    void set_board(vector<vector<long long>> A){
        QCB = A;
    }

    void set_position(vector<vector<int>> A){
        players_position = A;
    }

    
    void sortRows(const Comparator& comp){
        for (auto& row : QCB) {
            mergeSort(row, 0, row.size() - 1, comp);
        }
    }


private:
    vector<vector<long long>> QCB;              // Quantum Chess Board
    vector<vector<int>> players_position;       // Coordinates of the players
};

int main(){
    GC galactic_chess;
    string s = ""; 

    while(s != "END"){
        cin >> s;

        if(s == "CREATE_2D"){
            int size;
            cin >> size;
            vector<vector<long long>> A(size);
    
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    long long q_num;
                    cin >> q_num;
                    A[i].push_back(q_num);
                }
            }
    
            galactic_chess.set_board(A);
        }
        else if(s == "SORT_2D"){
            string type;
            cin >> type;
            bool isAscending;
            if(type == "ascending"){
                isAscending = true;
            }
            else if(type == "descending"){
                isAscending = false;
            }
            galactic_chess.sortRows(Comparator(isAscending));
        }
        else if(s == "INVERSION_2D"){
            cout << galactic_chess.countInversions() << endl;
        }
        else if(s == "DISPLAY_2D"){
            galactic_chess.display();
        }
        else if(s == "CLOSEST_2D"){
            int num_points;
            cin >> num_points;
            vector<vector<int>> coords(num_points);

            for(int i=0; i < num_points; i++){
                int x, y;
                cin >> x >> y;
                coords[i] = {x,y};
            }

            galactic_chess.set_position(coords);
            galactic_chess.closestPair();

        }
        else continue;
    }

    
    return 0;
}
