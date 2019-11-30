#include <iostream>	
#include <vector> 
#include <string>
#include <fstream>	


#include "Car.h"

using namespace std;


class Simulation {
    public: 
    Simulation(){};
    
    void readFile(int num){
	    string dir;			
	    string line;			
	    string file;			
	    ifstream myfile;	

	
	//which file to use?
	if(num == 1){
        file = "simple.txt";
	}
	else if (num == 2){
		file = "medium.txt";
	}
	else if (num == 3){
		file = "difficult.txt";
	}
	else exit(1);   // call system to stop
	
	
	 myfile.open(file);	//open the file for use

	
	//check that file exists and opens
    if (!myfile)
    {
	    cerr << "Unable to open file" << endl;
		exit(1);   // call system to stop
	}
	
	    while (getline (myfile,line)){
		
			size_t index = line.find(" ");
			string interval = line.substr(0,index);
			string direction = line.substr(index+1);

			carsArrivalTime.push_back(interval);
			carsDirection.push_back(direction);
			

	    }
		
	    myfile.close();

	
	    return;
    };
    void createCar(){
        for(int i = 0; i < carsArrivalTime.size(); i++) {
            Car *newCar = new Car(i,carsArrivalTime[i], carsDirection[i]);
        }
    };

    void printCarsArrivalTime() {
        for(int i = 0; i < carsArrivalTime.size(); i++){
            cout << carsArrivalTime.at(i) << endl;
        }
    }
    
    void printCarsDirection() {
        for(int i = 0; i < carsDirection.size(); i++){
            cout << carsDirection.at(i) << endl;
        }
    }

    vector<string> carsArrivalTime;
    vector<string> carsDirection;

    
};