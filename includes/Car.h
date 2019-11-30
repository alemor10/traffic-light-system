//Implementation of Queue using Linked List

#include<iostream>
#include <string> 

 
using namespace std;
 
struct Car {
    Car(int carID, string interval, string direction){};
    
    int carID;
    string arrivalTime;
    string direction;
};  