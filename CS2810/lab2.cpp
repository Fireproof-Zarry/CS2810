#include <bits/stdc++.h>
using namespace std;

class Library;

class Book{
public:
    //constructors
    Book(string t="UnknownTitle", string author_name="UnknownAuthor", string id="ISBN", int available = 0, int total = 5) : title(t), author(author_name), isbn(id), copiesAvailable(available), totalCopies(total) {}
    
    Book(const Book& b, string new_id) : title(b.title), author(b.author), isbn(new_id), copiesAvailable(b.copiesAvailable), totalCopies(b.totalCopies) {}
    
    friend class Library;
    //variables
    string title;
    string author;
    int MAX_COPIES_PER_BOOK = 15;
    
    //getters
    string get_isbn(){
        return isbn;
    }
    
    int get_availableCopies(){
        return copiesAvailable;
    }
    
    int get_totalCopies(){
        return totalCopies;
    }
    
    //setter
    void updateCopies(int count){
        if(copiesAvailable + count < 0){ //totalCopies >= copiesAvailable 
            cout << "Invalid request! Count becomes negative" << endl;
            return;
        }
        if(copiesAvailable + count > MAX_COPIES_PER_BOOK) return;

        copiesAvailable += count;
        totalCopies += count;
        return;
    }
    
    //utility methods
    bool borrowBook(){
        if(copiesAvailable <= 0){
            cout << "Invalid request! Copy of book not available" << endl;
            return false;
        }
        
        copiesAvailable--;
        return true;
    }
    
    bool returnBook(){
        if(copiesAvailable >= totalCopies){
            cout << "Invalid request! Copy of book exceeds total copies" << endl;
            return false;
        }
        
        copiesAvailable++;
        return true;
    }
    
    void printDetails(){
        cout << title << " " << author << endl;
    }
    
private:
    string isbn;
    int copiesAvailable;
    int totalCopies;  
    
};

class Member{
public:
    //constructor
    Member(string n, string id, int limit = 3) : name(n), memberID(id), borrowLimit(limit), borrowCount(0){}

    friend class Library;
    //variables
    string name;
    
    //utility methods
    bool borrowBook(string isbn){
        if(borrowCount >= borrowLimit){
            cout << "Invalid request! Borrow limit exceeded" << endl;
            return false;
        }
    
        borrowCount++;
        borrowedBooks[isbn]++;
        return true;
    }
    
    bool returnBook(string isbn){
        if(borrowedBooks.find(isbn) == borrowedBooks.end()){
            cout << "Invalid request! Book not borrowed" << endl;
            return false;
        }
        
        if(borrowedBooks[isbn] == 1) borrowedBooks.erase(isbn);
        else borrowedBooks[isbn]--;
        
        borrowCount--;
        return true;
    }
    
    void printDetails(){
        if(borrowedBooks.size() == 0) return;
        for(auto itr = borrowedBooks.begin(); itr != borrowedBooks.end(); itr++){
            cout << memberID << " " << name << " " << itr->first << " " << itr->second << endl;
        }
    }
    
private:
    string memberID;
    int borrowLimit;
    int borrowCount;
    map<string,int> borrowedBooks;
    
};

class Library{
public:
    int MAX_BOOKS = 50;
    int MAX_MEMBERS = 150;

    //helper functions
    Book* bookExists(string isbn){
        auto it = bookIndex.find(isbn);
        if (it != bookIndex.end()) {
            return &books[it->second];  
        }
        return nullptr;
    }

    Member* memberExists(string memberId){        
        auto it = memberIndex.find(memberId);
        if (it != memberIndex.end()) {
            return &members[it->second];  
        }
        return nullptr;
    }

    //utility methods
    bool addBook(Book& b){
        if((int)books.size() >= MAX_BOOKS){
            return false; //max book limit
        }

        if(bookExists(b.isbn) != nullptr){
            cout << "Invalid request! Book with same isbn already exists" << endl;
            return false;
        }

        books.push_back(b);
        bookIndex[b.isbn] = (int)books.size() - 1;
        return true;
    }
    
    bool registerMember(Member& m){

        if((int)members.size() >= MAX_MEMBERS){
            return false; //max members limit
        }
        if(memberExists(m.memberID) != nullptr){
            cout << "Invalid request! Member with same id already exists" << endl;
            return false;
        }
        members.push_back(m);
        memberIndex[m.memberID] = (int)members.size() - 1;
        return true;
    }
    
    bool borrowBook(string memberId, string isbn){
        Member *m = memberExists(memberId);
        Book *b = bookExists(isbn);
        
        if(b->get_availableCopies() <= 0){
            cout << "Invalid request! Copy of book not available" << endl;
            return false;
        }
        else{
            if(m->borrowBook(isbn)){
                b->borrowBook();
                return true;
            }
        }
        return false;        
    }
    
    bool returnBook(string memberID, string isbn){
        Member *m = memberExists(memberID); 
        Book *b = bookExists(isbn);
        
        if(m == nullptr || b == nullptr){ //member or book is not in the respective vectors
            return false;
        }
        
        if(b->copiesAvailable >= b->totalCopies){
            cout << "Invalid request! Copy of book exceeds total copies" << endl;
            return false;
        }
        else{
            if(m->returnBook(isbn)){
                b->returnBook(); 
                return true;
            }
        }

        return false; 
    }
 
    void updateCopiesCount(string isbn, int count){
        Book* b = bookExists(isbn);
        if(b == nullptr) return;
        b->updateCopies(count);
    }
 
    void printBook(string isbn){
        Book* b = bookExists(isbn);
        if(b == nullptr) return;
        b->printDetails();
    }

    void printMember(string memberId){
        Member *m = memberExists(memberId);
        if(m == nullptr) return;
        m->printDetails();
    }

    void printLibraryDetails(){
        for(Book& book : books){
            cout << book.title << " " << book.author << " " << book.copiesAvailable << endl;
        }
        
        for(Member& member : members){
            cout << member.memberID << " " << member.name << endl;
        }
    }
        
private:
    vector<Book> books;
    vector<Member> members;
    
    unordered_map<string, int> bookIndex;   //stores the isbn and the index of that book obj in vector books
    unordered_map<string, int> memberIndex; //stores the memberID and the index of that member obj in vector members

};

int main() {
    Library lib;
    string inp;
    while(inp != "Done"){
        cin >> inp;
        if(inp == "Book"){
            string type;
            cin >> type;

            if(type == "None"){
                Book b;
                lib.addBook(b);
            }

            else if(type == "ExistingBook"){
                string oldIsbn, newIsbn;
                cin >> oldIsbn >> newIsbn;
                
                Book* b = lib.bookExists(oldIsbn);

                if(b == nullptr) continue;
                else{
                    Book newBook(*b, newIsbn);  
                    lib.addBook(newBook);
                }
            }
            else{ //type is the title of the book
                string author, isbn;
                int available, total;
                cin >> author >> isbn >> available >> total;  
                Book b(type, author, isbn, available, total);
                lib.addBook(b);
            }
        }

        else if(inp == "UpdateCopiesCount"){
            string isbn;
            int newCount;
            cin >> isbn >> newCount;
            
            lib.updateCopiesCount(isbn, newCount);
        }
        else if(inp == "Member"){
            string type;
            cin >> type;
            
            string memberId, name;
            int limit;

            if(type == "NoBorrowLimit"){
                cin >> memberId >> name;
                limit = 3;
            } 
            else{
                cin >> name >> limit; 
                memberId = type;
            }

            Member* m = lib.memberExists(memberId);

            if(m != nullptr) cout << "Invalid request! Member with same id already exists" << endl;
            else{
                Member newMember(name, memberId, limit);  
                lib.registerMember(newMember);
            }
        }
        else if(inp == "Borrow"){
            string memberId, isbn;
            cin >> memberId >> isbn;
            lib.borrowBook(memberId, isbn);
        }
        else if(inp == "Return"){
            string memberId, isbn;
            cin >> memberId >> isbn;
            lib.returnBook(memberId, isbn);
        }
        else if(inp == "PrintBook"){
            string isbn;
            cin >> isbn;

            lib.printBook(isbn);
        }
        else if(inp == "PrintMember"){
            string memberId;
            cin >> memberId;

            lib.printMember(memberId);
        }
        else if(inp == "PrintLibrary"){
            lib.printLibraryDetails();
        }
        else continue;
    }
    return 0;
}

