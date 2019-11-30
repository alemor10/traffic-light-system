#include <iostream>	
#include <thread>
#include <vector> 	
#include <queue>	

#include "Car.h"



using namespace std;

class TrafficLane {
    public: 
    condition_variable cv;
    mutex lock;

    vector<Car> waitingCars;

    struct checkArrivalTime {
			bool operator()(Car const& c1, Car const& c2)
			{   
					return c1.arrivalTime < c2.arrivalTime;
			}
	};

    priority_queue<Car,vector<Car>,checkArrivalTime> cars;

};