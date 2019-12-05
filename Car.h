#include <string>

using namespace std;

struct Car {
    bool ready;
    int ID;
    int arrivalTime;
    string direction;

    Car(int ID ,int arrivalTime, string direction) {
        this->ID = ID;
        this->ready=false;
		this->arrivalTime = arrivalTime;
		this->direction = direction;
	}
	~Car() { };
}; 

// this is an strucure which implements the 
// operator overlading 
struct CompareCars { 
    bool operator()(Car* const& p1, Car* const& p2) 
    { 
        // return "true" if "p1" is ordered  
        // before "p2", for example: 
        return p1->arrivalTime > p2->arrivalTime; 
    } 
}; 
  