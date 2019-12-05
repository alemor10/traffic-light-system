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
#define MAX 40

using namespace std;

bool trafficLight = true;	//keep track of the current direction thats going
priority_queue<Car*, vector<Car*>, CompareCars> North;
priority_queue<Car*, vector<Car*>, CompareCars> South;
priority_queue<Car*, vector<Car*>, CompareCars> East;
priority_queue<Car*, vector<Car*>, CompareCars> West;
priority_queue<Car*, vector<Car*>, CompareCars> activeLane; 
vector<bool> ready(MAX); // holds all of the car threads
int releasecount =0;

mutex m_mutex;
condition_variable cv;



void threadCallback(int,int,string);//needed here so threadback can be called 


void releaseCar(Car* newCar)
{
    unique_lock<mutex> mlock(m_mutex);
    newCar->ready=true;
    cv.notify_one();
    this_thread::sleep_for(std::chrono::milliseconds(100));
}

void mainThreadFunction(int numAllowedCars)
{
    int stackTargetNorth = North.size() - numAllowedCars;
    int stackTargetSouth = South.size() - numAllowedCars;
    int stackTargetWest = West.size() - numAllowedCars;
    int stackTargetEast  = East.size()-numAllowedCars;

    while (!North.empty() || !South.empty() || !East.empty() || !West.empty()) 
    {
        while(North.size() != stackTargetNorth && South.size() !=stackTargetSouth)
        {
            Car* northCar = South.top();
            Car* southCar = North.top();
            releaseCar(northCar);
            releaseCar(southCar);
            cout <<"i should be popping like wtf"<<endl;

        }
        if(North.size()-numAllowedCars >=0 && South.size()-numAllowedCars>=0)
        {
            stackTargetNorth=North.size()-numAllowedCars;
            stackTargetSouth = South.size()-numAllowedCars;
        }
        else
        {
            stackTargetNorth=0;
            stackTargetSouth=0;
        }
        while(East.size() != stackTargetEast && West.size() !=stackTargetEast)
        {
            Car* eastCar = West.top();
            Car* westCar = East.top();
            releaseCar(eastCar);
            releaseCar(westCar);

        }
        if(East.size()-numAllowedCars > 0 && South.size()-numAllowedCars>=0)
        {
            stackTargetEast=East.size()-numAllowedCars;
            stackTargetWest=West.size()-numAllowedCars;
        }
        else
        {
            stackTargetEast=0;
            stackTargetWest=0;
        }
    }
}

void threadCallback(int vectorPos,int carArrival, string str)
{
     unique_lock<mutex> mlock(m_mutex);
     Car* newCar = new Car(vectorPos,carArrival, str);

     if(str[0] == 'N')
     {
        cout << "im going into the south queue" << newCar->direction << " " <<newCar->ID <<endl;
        South.push(newCar);
        while (!(newCar->ready)  || South.top()!= newCar)
        {
            cv.wait(mlock);
        }
        cout <<newCar->ID << "is now unlocked"<<endl;
        this_thread::sleep_for(std::chrono::milliseconds(200));
        South.pop();
        activeLane.push(newCar);
        cout << "im finna head out" << newCar->direction << " " <<newCar->ID <<endl;
        activeLane.pop();
     }
     if(str[0] == 'S')
     {
        cout << "im going into the North queue" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        North.push(newCar);
        while (!(newCar->ready) || North.top()!= newCar)
        {
            cv.wait(mlock);
        }
        cout <<newCar->ID << "is now unlocked"<<endl;
        this_thread::sleep_for(std::chrono::milliseconds(200));
        North.pop();
        activeLane.push(newCar);
        cout << "im finna head out" << newCar->direction << " " <<newCar->ID <<endl;
        activeLane.pop();

     }
     if(str[0] == 'W')
     {
        cout << "im going into the East queue" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        East.push(newCar);
        while (!(newCar->ready) || East.top()!= newCar)
        {
          cv.wait(mlock);
        }
        cout <<newCar->ID << "is now unlocked"<<endl;
        this_thread::sleep_for(std::chrono::milliseconds(200));
        East.pop();
        activeLane.push(newCar);
        cout << "im finna head out fam" << newCar->direction << " " <<newCar->ID <<endl;
        activeLane.pop();

     }  
     if(str[0] == 'E')
     {
        cout << "im going into the West queue" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        West.push(newCar);
        while (!(newCar->ready) || West.top()!= newCar)
        {
            cv.wait(mlock);
        }
        cout <<newCar->ID << "is now unlocked"<<endl;
        this_thread::sleep_for(std::chrono::milliseconds(200));
        West.pop();
        activeLane.push(newCar);
        cout << "im finna head out fam" << newCar->direction << " " <<newCar->ID <<endl;
        activeLane.pop();
     }
    

}


int main()
{
    vector<thread> carThreads; // holds all of the car threads
    int file = 0;
    int numAllowedCars;
    int threadCount = 0;
    string filename;
    string fileline;			
	ifstream myfile;
    
    //time_t start = time(0);
    cout << "Which file would you like to read and allowed cars to pass per light?" <<endl;
    cout << "1. Easy.Txt" <<endl;
    cout << "2. Medium.Txt" <<endl;
    cout << "3. Hard Text" <<endl;
    cin >> file >> numAllowedCars;
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
    


    time_t start = time(0);
    while(getline(myfile,fileline))
    {
		size_t index = fileline.find(" ");
		string arrivalTime = fileline.substr(0,index);
        int tempArrivalTime = stoi(arrivalTime);
		string direction = fileline.substr(index+1);
        carThreads.push_back(thread(threadCallback,threadCount,tempArrivalTime,direction));
        cout << "Car Thread Created: " << threadCount << endl;
        sleep(1);
        threadCount++;

    }
    myfile.close();
    bool begin = false;

    thread releaseCars(mainThreadFunction,numAllowedCars);	//create threads to call the release function

    releaseCars.join();
    for (int i = 0; i < carThreads.size(); ++i) {
		carThreads[i].join();	//join the car threads
		cout << "Thread joined: " << i+1 << endl;
	}


    double seconds_since_start = difftime( time(0), start);	//end the timer and do the math to get final time
	cout << "Total Time: " << seconds_since_start << "s" << endl;


    return 0;
}
