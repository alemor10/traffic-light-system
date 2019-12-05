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
    //Car* tempCar = South.top();
    unique_lock<mutex> mlock(m_mutex);
    newCar->ready=true;
    cv.notify_one();
    usleep(50);
}

void mainThreadFunction(int numAllowedCars)
{
    int count = 0;
    
    while (!North.empty() || !South.empty()) 
    {
        if(trafficLight){
            Car* northCar = South.top();
            Car* southCar = North.top();
            //cout << southCar->arrivalTime<<endl;
            //cout << northCar->arrivalTime<<endl;
            releaseCar(northCar);
            releaseCar(southCar);

        }
    }
    trafficLight =!trafficLight;

    while (!East.empty() || !West.empty()) 
    {
        if(!trafficLight){
            Car* eastCar = West.top();
            Car* westCar = East.top();
            //cout << southCar->arrivalTime<<endl;
            //cout << northCar->arrivalTime<<endl;
            releaseCar(eastCar);
            releaseCar(westCar);

        }
    }


}

void threadCallback(int vectorPos,int carArrival, string str)
{
     unique_lock<mutex> mlock(m_mutex);
     Car* newCar = new Car(vectorPos,carArrival, str);

     if(str[0] == 'N')
     {
        //cout << "im going into the south queue" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        South.push(newCar);
        ready.at(vectorPos) = false;
        while (!(newCar->ready) || !trafficLight || South.top()!= newCar)
        {
            cv.wait(mlock);
        }
        //cout <<"i should be unlocked"<<endl;
        South.pop();
        activeLane.push(newCar);
        this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "aight fam im finna head out fam" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        activeLane.pop();
        //cout << South.size() <<endl;
     }
     if(str[0] == 'S')
     {
        //cout << "im going into the North queue" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        North.push(newCar);
        ready.at(vectorPos) = false;
        while (!(newCar->ready) ||!trafficLight || North.top()!= newCar)
        {
            cv.wait(mlock);
        }
        //cout <<"i should be unlocked"<<endl;
        North.pop();
        activeLane.push(newCar);
        this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "aight fam im finna head out fam" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        activeLane.pop();

     }
     if(str[0] == 'W')
     {
        East.push(newCar);
        ready.at(vectorPos) = false;
        while (!(newCar->ready) ||  trafficLight || East.top()!= newCar)
        {
          cv.wait(mlock);
        }
        //cout <<"i should be unlocked"<<endl;
        East.pop();
        activeLane.push(newCar);
        this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "aight fam im finna head out fam" << newCar->arrivalTime << " " <<newCar->direction <<endl;
        activeLane.pop();

     }  
     if(str[0] == 'E')
     {
        West.push(newCar);
        ready.at(vectorPos) = false;
        while (!(newCar->ready) || trafficLight || West.top()!= newCar)
        {
            cv.wait(mlock);
        }
        //cout <<"i should be unlocked"<<endl;
        West.pop();
        activeLane.push(newCar);
        this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "aight fam im finna head out fam" << newCar->arrivalTime << " " <<newCar->direction <<endl;
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
        //cout << "Car Thread Created: " << threadCount << endl;
        sleep(1);
        threadCount++;

    }
    myfile.close();

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
