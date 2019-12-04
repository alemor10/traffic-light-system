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

using namespace std;


void threadCallback(int, string);//needed here so threadback can be called 

string currDir = "N";	//keep track of the current direction thats going
priority_queue<Car*, vector<Car*>, CompareCars> North;
priority_queue<Car*, vector<Car*>, CompareCars> South;
priority_queue<Car*, vector<Car*>, CompareCars> East;
priority_queue<Car*, vector<Car*>, CompareCars> West;
priority_queue<Car*, vector<Car*>, CompareCars> activeLane; 
vector<thread> carThreads; // holds all of the car threads

mutex m_mutex;
condition_variable cv;


void releaseCars()
{
    string carDir;
    int arrive;
    string collision1 ="NW";
    string collision2 ="SE";
    string collision3 ="EN";
    string collision4 ="WS";

    while (! activeLane.empty() ) {
        Car* frontCar = activeLane.top(); 
        carDir = frontCar->direction;
        arrive = frontCar->arrivalTime;
        //cout <<arrive << carDir <<endl;

        if(currDir =="N" && carDir.find(collision1) != std::string::npos)
        {       
            cout << "fina crash so we finna wait till all the other cars pass"<< arrive <<endl;
            activeLane.pop();

        }
        else if(currDir =="N" && carDir.find(collision2) != std::string::npos)
        {       
            cout << "fina crash so we finna wait till all the other cars pass"<< arrive <<endl;
            activeLane.pop();
            cv.notify_one();
        }
        else if(currDir =="W" && carDir.find(collision3) != std::string::npos)
        {       
            cout << "fina crash so we finna wait till all the other cars pass"<< arrive <<endl;
            activeLane.pop();
            cv.notify_one();
        }
        else if(currDir =="W" && carDir.find(collision4) != std::string::npos)
        {       
            cout << "fina crash so we finna wait till all the other cars pass"<< arrive <<endl;
            activeLane.pop();
            cv.notify_one();
        }
        else 
        {
            cout << "pop this pussi "<<endl;
            activeLane.pop();
            cv.notify_one();
        }
    }

}



void mainThreadFunction(int numAllowedCars)
{
    

  

}

void threadCallback(int carArrival, string str)
{
    
    if(str[0] == 'N')//check the first character of the string and sort into pq
    {	
        Car* newCar = new Car(carArrival, str);
        unique_lock<mutex> mlock(m_mutex);
		South.push(newCar);
        string temp = newCar->direction.substr(0,1);
        cout << newCar->arrivalTime <<" is now in the South Lane"<<endl;
        while (South.top() != newCar  && currDir != temp)
        {
            cv.wait(mlock);
        }

        Car* tempCar = South.top();
        South.pop();
        activeLane.push(tempCar);
        sleep(5);
        cout << "aight fam im finna head out fam" << tempCar->arrivalTime << " " <<tempCar->direction <<endl;
        activeLane.pop();


	}
    if(str[0] == 'S')//check the first character of the string and sort into pq
    {	
        Car* newCar = new Car(carArrival, str);
        unique_lock<mutex> mlock(m_mutex);
		North.push(newCar);
        string temp = newCar->direction.substr(0,1);
        cout << newCar->arrivalTime <<" is now in the North Lane"<<endl;
        while (North.top() != newCar  && currDir != temp)
        {
            cout << temp <<endl;
            cv.wait(mlock);
        }
        Car* tempCar = North.top();
        North.pop();
        activeLane.push(tempCar);
        sleep(5);
        cout << "aight fam im finna head out fam" << tempCar->arrivalTime << " " <<tempCar->direction <<endl;
        activeLane.pop();
	}
    if(str[0] == 'W')//check the first character of the string and sort into pq
    {	
        Car* newCar = new Car(carArrival, str);
        unique_lock<mutex> mlock(m_mutex);
		East.push(newCar);
        string temp = newCar->direction.substr(0,1);
        while (East.top() != newCar  && currDir != temp)
        {
            cout << temp <<endl;
            cv.wait(mlock);
        }
        Car* tempCar = East.top();
        East.pop();
        activeLane.push(tempCar);
        sleep(5);
        cout << "aight fam im finna head out fam" << tempCar->arrivalTime << " " <<tempCar->direction <<endl;
        activeLane.pop();
	}
    if(str[0] == 'E')//check the first character of the string and sort into pq
    {	
        Car* newCar = new Car(carArrival, str);
        unique_lock<mutex> mlock(m_mutex);
		West.push(newCar);
        string temp = newCar->direction.substr(0,1);	
        while (West.top() != newCar  && currDir != temp)
        {
            cout << temp <<endl;
            cv.wait(mlock);
        }
        Car* tempCar = West.top();
        West.pop();
        activeLane.push(tempCar);
        sleep(5);
        cout << "aight fam im finna head out fam" << tempCar->arrivalTime << " " <<tempCar->direction <<endl;
        activeLane.pop();
	}
}

int main()
{
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
        carThreads.push_back(thread(threadCallback,tempArrivalTime,direction));
        cout << "Car Thread Created: " << threadCount << endl;
        //sleep(1);
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

