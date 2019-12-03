#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue> 
#include "unistd.h"

#include "Car.h"

#define NUM_THREADS 40

using namespace std;




string currDir = "";	//keep track of the current direction thats going
priority_queue<Car*, vector<Car*>, CompareCars> North;
priority_queue<Car*, vector<Car*>, CompareCars> South;
priority_queue<Car*, vector<Car*>, CompareCars> East;
priority_queue<Car*, vector<Car*>, CompareCars> West;
priority_queue<Car*, vector<Car*>, CompareCars> activeLane; 
queue<Car*> intersection;	//queue to track all the cars in the intersection
vector<thread> carThreads; // holds all of the car threads
mutex m_mutex;
condition_variable cv;






void releaseCar()
{
    while ((!North.empty() || !South.empty()) || (!East.empty() || !West.empty())) {

        if (!North.empty()) //check that North queue has cars
        {	
            if(currDir != "E" && currDir != "W") //checks collision
            {	
				currDir = "N";	//update the currDir string for later
				cout << "cv notify: N" << endl;
				cv.notify_one();	//notify the queue
			}
        }
        if (!South.empty()) //check that North queue has cars
        {
            if(currDir != "E" && currDir != "W") //checks collision
            {	
				currDir = "S";	//update the currDir string for later
				cout << "cv notify: S" << endl;
				cv.notify_one();	//notify the queue
			}	
        }
        if (!East.empty()) //check that North queue has cars
        {	
            if(currDir != "S" && currDir != "N") //checks collision
            {	
				currDir = "E";	//update the currDir string for later
				cout << "cv notify: E" << endl;
				cv.notify_one();	//notify the queue
			}	
        }
        if (!West.empty()) //check that North queue has cars
        {	
            if(currDir != "S" && currDir != "N") //checks collision
            {	
				currDir = "W";	//update the currDir string for later
				cout << "cv notify: W" << endl;
				cv.notify_one();	//notify the queue
			}	
        }

    }    

}


void threadCallback(int carArrival, string str,priority_queue<Car*,vector<Car*>,CompareCars>& activeLane)
{
    Car* newCar = new Car(carArrival, str);
    unique_lock<mutex> mlock(m_mutex);

    activeLane.push(newCar);

    while (activeLane.top() != newCar && currDir != newCar->direction)
        cv.wait(mlock);


    Car* frontCar = activeLane.top();
    activeLane.pop();
    intersection.push(frontCar);
    cout << frontCar->direction << frontCar->arrivalTime <<endl;
    intersection.pop();

}


int main()
{
    int file = 0;
    int threadCount = 0;
    string filename;
    string fileline;			
	ifstream myfile;
    
    time_t start = time(0);
    cout << "Which file would you like to read?" <<endl;
    cout << "1. Easy.Txt" <<endl;
    cout << "2. Medium.Txt" <<endl;
    cout << "3. Hard Text" <<endl;
    cin >> file;
    //which file to use?
	if(file == 1){
        filename = "simple.txt";
	}
	else if (file == 2){
		filename = "medium.txt";
	}
	else if (file == 3){
		filename = "difficult.txt";
	}
	else exit(1);   // call system to stop
	
	

    myfile.open(filename);	//open the file for use

    //check that file exists and opens
    if (!myfile)
    {
	    cerr << "Unable to open file" << endl;
		exit(1);   // call system to stop
	}
    


    //time_t start = time(0);
    while(getline(myfile,fileline))
    {
		size_t index = fileline.find(" ");
		string arrivalTime = fileline.substr(0,index);
        int tempArrivalTime = stoi(arrivalTime);
		string direction = fileline.substr(index+1);
        carThreads.push_back(thread(threadCallback,tempArrivalTime,direction, ref(activeLane)));
        cout << "Car Thread Created: " << threadCount << endl;
        threadCount++;
    }
    myfile.close();
    

    thread releaseCars(releaseCar);	//create threads to call the release function

    releaseCars.join();
    for (int i = 0; i < carThreads.size(); ++i) {
		carThreads[i].join();	//join the car threads
		cout << "Thread joined: " << i+1 << endl;
	}

    double seconds_since_start = difftime( time(0), start);	//end the timer and do the math to get final time
	cout << "Total Time: " << seconds_since_start << "s" << endl;


    return 0;
}

