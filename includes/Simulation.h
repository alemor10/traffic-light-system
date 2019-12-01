#include <iostream>	
#include <vector> 




//#include "Car.h"
#include "TrafficLane.h"

using namespace std;


class Simulation {
    public: 
    Simulation(){};




    void printTrafficLane(vector<int> vect)
    {
        
        for (int i=0; i<vect.size(); i++) 
            cout <<" "; 

    }



    private: 
    TrafficLane northLane;
    TrafficLane southLane;
    TrafficLane eastLane;
    TrafficLane westLane;

    
};