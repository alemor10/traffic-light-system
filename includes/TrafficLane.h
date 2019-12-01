#include <iostream>	
#include <thread>
#include <mutex>
#include <vector> 	
	

//#include "Car.h"



using namespace std;

class TrafficLane {
    public: 
    TrafficLane(){
        //front = NULL; 
    };
    ~TrafficLane(){};

    //void Insert(Car newCar){
    //    Lane.push_back(newCar);
    //};
    void remove(int threadID);




    //struct checkArrivalTime {
	//		bool operator()(Car const& c1, Car const& c2)
	//		{   
    //					return c1.arrivalTime < c2.arrivalTime;
	//		}
	//};

    private: 
        mutex lock; 
        //vector<Car> Lane;
        //Car *front;



    

};