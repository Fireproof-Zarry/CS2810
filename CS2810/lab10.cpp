#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define ld long double

// fwd declarations
template <typename T>
class Multiplication;
template <typename T>
class Differentiation;
template <typename T>
class Evaluation;

template <typename T>
class Polynomial{ 
public: 
    Polynomial<T>(vector<T> p) : poly(p) {}
    // methods
    vector<T> get_polynomial(){
        return poly;
    }
    void print() {
        for (T val : poly) cout << val << " ";
        cout << endl;
    }
    // call respective functions
    void operator*(vector<T> B){
        poly = Multiplication<T>().Multiply(poly, B);
    }
    void Diff(){
        poly = Differentiation<T>().diff(poly);
    }
    T Evaluate(int val){
        return Evaluation<T>().evaluate(poly, val);
    }
private:
    vector<T> poly; 
};

class Complex{
public:
    ll real;
    ll img;
    //constructors
    Complex() : real(0), img(0) {}
    Complex(ll r, ll i) : real(r), img(i) {}
    //functions for complex numbers
    Complex operator+(const Complex &c2) const{
        return Complex(real + c2.real, img + c2.img);
    }
    Complex operator-(const Complex &c2) const{
        return Complex(real - c2.real, img - c2.img);
    }
    Complex operator*(const Complex &c2) const{ 
        return Complex(real*c2.real - img*c2.img, real*c2.img + img*c2.real);
    }
    friend ostream& operator<<(ostream& os, const Complex& c){ // for printing
        os << c.real << " " << c.img;
        return os;
    }
};
// helper for removing trailing zeroes
template <typename T>  
bool is_zero(const T& x){ //ll
    return x == T();  
}
template <> 
bool is_zero<ld>(const ld& x){ //ld
    return abs(x) < 1e-8; 
}
template <>
bool is_zero<Complex>(const Complex& c){ // complex
    return c.real == 0 && c.img == 0;  
}

template <typename T>
class Multiplication{
public:
    Multiplication(){} 
    
    vector<T> karatsuba(vector<T> A, vector<T> B, int n){ 
        /*  A = A_u*x^m + A_l   B = B_u*x^m + B_l
            AB = (A_u*B_u)x^2m + ((A_u+A_l)(B_u+B_l)-(A_l*B_l)-(A_u*B_u))x^m + (A_l*B_l)
        */
        if(n == 1) return {A[0]*B[0]};  // base case
        int m = (n+1)/2;                // mid index
        
        // dividing the vectors into 2 halves
        vector<T> A_lower(A.begin(), A.begin()+m);
        vector<T> A_upper(A.begin()+m, A.end());
        vector<T> B_lower(B.begin(), B.begin()+m);
        vector<T> B_upper(B.begin()+m, B.end());
        // finding the 3 products recursively
        vector<T> P1 = karatsuba(A_lower, B_lower, m);
        vector<T> P2 = karatsuba(A_upper, B_upper, m);
        
        vector<T> A_sum(m, T()), B_sum(m, T()); // T() are the default values
        for(size_t i=0; i<A_lower.size(); i++) A_sum[i] = A_lower[i] + A_upper[i];
        for(size_t i=0; i<B_lower.size(); i++) B_sum[i] = B_lower[i] + B_upper[i];
   
        vector<T> P3 = karatsuba(A_sum, B_sum, m);
    
        for(size_t i=0; i<min({P3.size(), P1.size(), P2.size()}); i++) P3[i] = P3[i] - P1[i] - P2[i];
        // final result
        vector<T> result(2*n-1, T());
        for(size_t i=0; i < P1.size(); i++) result[i] = result[i] + P1[i];
        for(size_t i=0; i < P3.size(); i++) result[i + m] = result[i + m] + P3[i];
        for(size_t i=0; i < P2.size(); i++) result[i + 2*m] = result[i + 2*m] + P2[i];

        return result;
    }

    vector<T> Multiply(vector<T> A, vector<T> B){
        int n1 = A.size(), n2 = B.size();
        // multiplying by a constant - brute force
        if(n1 == 1 || n2 == 1){ 
            vector<T> C(n1 + n2 - 1, T());
            for(int i = 0; i < n1; i++){
                for(int j = 0; j < n2; j++){
                    C[i + j] = C[i + j] + A[i]*B[j];
                }
            }
            return C;
        }
        // else do recursion
        int n=1;
        while(n < n1+n2) n *= 2;
        // resize A and B to same even size - n
        A.resize(n, T());
        B.resize(n, T());
        // call the function
        vector<T> C = karatsuba(A,B,n);
        // remove trailing zeroes
        while((int)C.size()>(n1+n2-1) && is_zero(C.back())){
            C.pop_back();
        }        
        return C;
    }
};

template <typename T>
class Differentiation{
public:
    vector<T> diff(vector<T> &A){
        int n = A.size();
        if(n <= 1) return {0}; // differentiation of constant

        vector<T> result(n-1);
        for(int i=1; i<n; i++){
            result[i-1] = A[i]*i;
        }
        return result;
    }
};

template <typename T>
class Evaluation{
public:
    T evaluate(vector<T> A, int val){
        T result = 0;
        for(int i = A.size() - 1; i >= 0; i--){
            result = result*val + A[i];  // horner's method
        }
        return result;
    }
};

template <>
class Evaluation<string>{
public:
    string evaluate(vector<string> &A, int val){
        string result = "";
        int n = A.size()-1;
        int y = 1; 

        for(int k = 0; k < n; k++){  // y = val^n
            y *= val;   
        }
        for(int k=n; k >= 0; k--){ // y = val^k
            for(int j=0; j<y; j++){
                result += A[k];
            }
            y /= val;  
        }
        return result;
    }
};



int main(){
    cout << fixed << setprecision(6); 
    int q; cin >> q; // no.of queries
    
    for(int i=0; i<q; i++){
        int op; cin >> op;
        string type; cin >> type;
        
        if(op == 1){ // multiplication
            int deg1; cin >> deg1;
            int deg2; 
        
            if(type == "integer"){
                // getting input
                vector<ll> poly1(deg1);
                for(int i=0; i<deg1; i++){
                    cin >> poly1[i];
                }
                cin >> deg2;
                vector<ll> poly2(deg2);
                for(int i=0; i<deg2; i++){
                    cin >> poly2[i];
                }
                Polynomial<ll> p(poly1); // class object
                p*poly2; // call function 
                p.print(); // print the polynomial
            }
            else if(type == "float"){
                vector<ld> poly1(deg1);
                for(int i=0; i<deg1; i++){
                    cin >> poly1[i];
                }
                cin >> deg2;
                vector<ld> poly2(deg2);
                for(int i=0; i<deg2; i++){
                    cin >> poly2[i];
                }
                Polynomial<ld> p(poly1);
                p*poly2;
                p.print();
            }
            else if(type == "complex"){
                vector<Complex> poly1;
                ll r, img;
                for(int i=0; i<deg1; i++){
                    cin >> r >> img;
                    poly1.push_back(Complex(r,img));
                }
                cin >> deg2;
                vector<Complex> poly2;
                for(int i=0; i<deg2; i++){
                    cin >> r >> img;
                    poly2.push_back(Complex(r,img));
                }
                Polynomial<Complex> p(poly1);
                p*poly2;
                p.print();
            }
        }
        else if(op == 2){
            int deg,val;
            cin >> deg;
            if(type == "integer"){
                vector<ll> poly(deg);
                for(int i=0; i<deg; i++){
                    cin >> poly[i];
                }
                cin >> val;
                Polynomial<ll> p(poly);
                cout << p.Evaluate(val) << endl;
            }
            else if(type == "float"){
                vector<ld> poly(deg);
                for(int i=0; i<deg; i++){
                    cin >> poly[i];
                }
                cin >> val;
                Polynomial<ld> p(poly);
                cout << p.Evaluate(val) << endl;
            }
            else if(type == "string"){
                vector<string> poly(deg);
                for(int i=0; i<deg; i++){
                    cin >> poly[i];
                }
                cin >> val;
                Polynomial<string> p(poly);
                cout << p.Evaluate(val) << endl;
            }
        }
        else if(op == 3){
            int deg; cin >> deg;
            if(type == "integer"){
                vector<ll> poly(deg);
                for(int i=0; i<deg; i++){
                    cin >> poly[i];
                }
                Polynomial<ll> p(poly);
                p.Diff();
                p.print();
            }
            else if(type == "float"){
                vector<ld> poly(deg);
                for(int i=0; i<deg; i++){
                    cin >> poly[i];
                }
                Polynomial<ld> p(poly);
                p.Diff();
                p.print();
            }
        }
        else continue;
    }
    return 0;
}