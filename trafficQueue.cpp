#include "trafficQueue.h"

void Queue::insert(int n,string x){
    Node *temp=new Node;
    if(temp==NULL){
        cout<<"Overflow"<<endl;
        return;
    }
    temp->ID=n;
    temp->Direction=x;
    temp->next=NULL;
 
    //for first node
    if(front==NULL){
        front=tail=temp;
    }
    else{
        tail->next=temp;
        tail=temp;
    }
    cout<<n<<" has been inserted successfully."<<endl;
}
 
void Queue::display(){
    if(front==NULL){
        cout<<"Underflow."<<endl;
        return;
    }
    Node *temp=front;
    //will check until NULL is not found
    while(temp){
        cout<<temp->ID<<" ";
        temp=temp->next;
    }
    cout<<endl;
}
 
void Queue :: deleteitem()
    {
    if (front==NULL){
        cout<<"underflow"<<endl;
        return;
    }
     
    cout<<front->ID<<" is being deleted "<<endl;
    if(front==tail)//if only one node is there
        front=tail=NULL;
    else
        front=front->next;
}
 
Queue ::~Queue()
{
    while(front!=NULL)
    {
        Node *temp=front;
        front=front->next;
        delete temp;
    }
    tail=NULL;
}
 