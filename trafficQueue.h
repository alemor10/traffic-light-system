//Implementation of Queue using Linked List

#include<iostream>
#include <string> 
 
using namespace std;
 
struct Node{
    int ID;
    string Direction;
    
    Node *next;
};
 
class Queue{
    public:
    Node *front,*tail;
    Queue(){
        front=tail=NULL;
    }
 
    void insert(int n,string x);
    void deleteitem();
    void display();
    ~Queue();
};