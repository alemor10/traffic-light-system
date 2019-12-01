//Implementation of Queue using Linked List

#include<iostream>
#include <string> 

#pragma once 
 
using namespace std;
 
struct Car {
    Car(int carID,string direction){};
    
    int carID;
    string arrivalTime;
    string direction;
};  