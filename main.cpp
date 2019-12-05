#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <mutex>
#include <algorithm>
#include <map>
#include <condition_variable>
#include <chrono>

using namespace std;

struct Car {
    int id;
    int time;
    string dest;
    string origin;
};

int driveTime = 5;

map<char, char> dict;

vector<Car> cars;

ofstream outFile;

queue<int> northQ;
queue<int> southQ;
queue<int> eastQ;
queue<int> westQ;

mutex inter00;
mutex inter01; 
mutex inter10;
mutex inter11;

mutex originN;
mutex originS;
mutex originE;
mutex originW;
// Positions of locks in the intersection

/*
                 |         |       |
                 | originN |       |
                 |         |       |
                 |         |       |
_________________                   _________________
                  inter00   inter01     originE
_________________                   _________________
        originW   inter10   inter11
_________________                   _________________
                 |        |         |
                 |        |         |
                 |        | originS |
                 |        |         |
                 |        |         |
*/

// Lock used for logging
mutex coutLock;

bool compare(const Car &a, const Car &b){
        return a.time < b.time;
}

void fileReader(string fileName){
    // Read file and save each line as a Car object.
    ifstream input;
    input.open(fileName + ".txt");
    if(!input){
        cerr << "Unable to open file" << endl;
        exit(1);
    }
    string line;
    vector<Car> lines;
    int i = 1;
    while(getline(input, line)){
        std::stringstream linestream(line);
        string time;
        string dir;
        Car temp;

        getline(linestream, time, ' ');
        linestream >> dir;

        if(dir.length() == 2){
            temp.dest = dir[1];
            temp.origin = dict[dir[0]];
        }else{
            temp.dest = dir[0];
            temp.origin = dict[dir[0]];
        }
        temp.time = stoi(time);
        temp.id = i;
        cars.push_back(temp);
        i++;
    }
    input.close();

    // sort cars by time it takes to reach intersection
    sort(cars.begin(), cars.end(), compare);
    return;
}

// Function for printing Logs and writing output file
void printLogs(Car c, int i){
    switch (i){
        case 1:
            cout << "CAR #" << c.id << ": CREATED " << endl;
            break;
        case 2:
            cout << "CAR #" << c.id << "  First in " << c.origin << " Queue" << endl;
            outFile << "CAR #" << c.id << "  First in " << c.origin << " Queue" << "\n";
            break;
        case 3:
            cout << "CAR #" << c.id << "  LEFT  " << "      Direction: " << c.origin << "-->" << c.dest <<  endl;
            outFile << "CAR #" << c.id << "  LEFT  " << "      Direction: " << c.origin << "-->" << c.dest << "\n";
            break;
    }
    return;
}

// Function to be executed by Cars comming from North
void fromNorth(Car c){

    coutLock.lock();
    printLogs(c, 1);
    coutLock.unlock();

    this_thread::sleep_for (chrono::seconds(c.time));
    northQ.push(c.id);
    while(true){
        if(northQ.front() == c.id){
            coutLock.lock();
            printLogs(c, 2);
            coutLock.unlock();

            lock_guard<mutex> lockOrigin(originN);

            if(c.dest == "S"){
                lock_guard<mutex> lock1(inter00);
                lock_guard<mutex> lock2(inter10);
                this_thread::sleep_for(chrono::seconds(driveTime));

            }else if(c.dest == "W"){
                lock_guard<mutex> lock1(inter00);
                this_thread::sleep_for(chrono::seconds(driveTime));

            }else if(c.dest == "E"){
                lock_guard<mutex> lock1(inter00);
                lock_guard<mutex> lock2(inter11);
                this_thread::sleep_for(chrono::seconds(driveTime));
            }
            coutLock.lock();
            printLogs(c, 3);
            coutLock.unlock();
            break;
        }else{
            continue;
        }
    }
    northQ.pop();
    return;
}

// Function to be executed by Cars comming from South
void fromSouth(Car c){
    coutLock.lock();
    printLogs(c, 1);
    coutLock.unlock();
    
    this_thread::sleep_for (chrono::seconds(c.time));
    southQ.push(c.id);
    while(true){
        if(southQ.front() == c.id){
            coutLock.lock();
            printLogs(c, 2);
            coutLock.unlock();
            lock_guard<mutex> lockOrigin(originS);

            if(c.dest == "N"){
                lock_guard<mutex> lock1(inter11);
                lock_guard<mutex> lock2(inter01);
                this_thread::sleep_for(chrono::seconds(driveTime));


            }else if(c.dest == "W"){
                lock_guard<mutex> lock1(inter11);
                lock_guard<mutex> lock2(inter00);
                this_thread::sleep_for(chrono::seconds(driveTime));


            }else if(c.dest == "E"){
                lock_guard<mutex> lock1(inter11);
                this_thread::sleep_for(chrono::seconds(driveTime));

            }
            coutLock.lock();
            printLogs(c, 3);
            coutLock.unlock();
            break;
        }else{
            continue;
        }
    }
    southQ.pop();
    return;
}

// Function to be executed by Cars comming from East
void fromEast(Car c){

    coutLock.lock();
    printLogs(c, 1);
    coutLock.unlock();

    this_thread::sleep_for (chrono::seconds(c.time));
    eastQ.push(c.id);
    while(true){
            if(eastQ.front() == c.id){

                coutLock.lock();
                printLogs(c, 2);
                coutLock.unlock();

                lock_guard<mutex> lockOrigin(originE);

                if(c.dest == "W"){
                    lock_guard<mutex> lock1(inter01);
                    lock_guard<mutex> lock2(inter00);
                    this_thread::sleep_for(chrono::seconds(driveTime));
                }else if(c.dest == "S"){
                    lock_guard<mutex> lock1(inter01);
                    lock_guard<mutex> lock2(inter10);
                    this_thread::sleep_for(chrono::seconds(driveTime));

                }else if(c.dest == "N"){
                    lock_guard<mutex> lock1(inter01);
                    this_thread::sleep_for(chrono::seconds(driveTime));
                }
                coutLock.lock();
                printLogs(c, 3);
                coutLock.unlock();
                break;  
            }else{
                continue;
            }
        }
    eastQ.pop();
    return;
}

// Function to be executed by Cars comming from West
void fromWest(Car c){

    coutLock.lock();
    printLogs(c, 1);
    coutLock.unlock();

    this_thread::sleep_for (chrono::seconds(c.time));
    westQ.push(c.id);
    while(true){
        if(westQ.front() == c.id){
            coutLock.lock();
            printLogs(c, 2);
            coutLock.unlock();
            lock_guard<mutex> lockOrigin(originW);
            if(c.dest == "E"){
                lock_guard<mutex> lock1(inter10);
                lock_guard<mutex> lock2(inter11);
                this_thread::sleep_for(chrono::seconds(driveTime));

            }else if(c.dest == "N"){
                lock_guard<mutex> lock1(inter10);
                lock_guard<mutex> lock2(inter01);
                this_thread::sleep_for(chrono::seconds(driveTime));

            }else if(c.dest == "S"){
                lock_guard<mutex> lock1(inter10);
                this_thread::sleep_for(chrono::seconds(driveTime));
            }
            coutLock.lock();
            printLogs(c, 3);
            coutLock.unlock();
            break;
        }else{
            continue;
        }
    }
    westQ.pop();
    return;
}

bool fileExists(string filename)
{
  ifstream ifile(filename);
  return (bool)ifile;
}

int main(){
    if(fileExists("outFile.txt")){
        remove("outFile.txt");
    }
    outFile.open("outFile.txt");
    vector<thread> threads;
    dict['N'] = 'S';
    dict['S'] = 'N';
    dict['E'] = 'W';
    dict['W'] = 'E';

    string fileName;
    cout << "input file name (simple, medium or difficult): ";
    cin >> fileName;
    outFile << "Output File for input file: " << fileName << "\n" << "\n";
    fileReader(fileName);

    auto start = chrono::steady_clock::now();
    for(int i = 0; i < cars.size(); i++){
        if(cars[i].origin == "N"){
            threads.push_back(thread(fromNorth, cars[i]));
        }
        else if(cars[i].origin == "S"){
            threads.push_back(thread(fromSouth, cars[i]));
        }
        else if(cars[i].origin == "E"){
            threads.push_back(thread(fromEast, cars[i]));
        }
        else if(cars[i].origin == "W"){
            threads.push_back(thread(fromWest, cars[i]));
        }
    }

    for(int i = 0; i < threads.size(); i++){
        threads[i].join();
    }
    auto end = chrono::steady_clock::now();
    cout << "execution took " << chrono::duration_cast<chrono::seconds>(end - start).count() << "seconds." << endl;
    outFile << "execution took " << chrono::duration_cast<chrono::seconds>(end - start).count() << "seconds.";
    outFile.close();
    return 0;
}



    while (!North.empty() || !South.empty() || !East.empty() || !West.empty())
    {
        do 
        {
            if(trafficLight)
            {
                Car* northCar = South.top();
                Car* southCar = North.top();
                //cout << southCar->arrivalTime<<endl;
                //cout << northCar->arrivalTime<<endl;
                releaseCar(northCar);
                releaseCar(southCar);
                count++;

            }
        } while (count < numAllowedCars);
    
        trafficLight =!trafficLight;
    
        do
        {
            if(!trafficLight)
            {
                Car* eastCar = West.top();
                Car* westCar = East.top();
                //cout << southCar->arrivalTime<<endl;
                //cout << northCar->arrivalTime<<endl;
                releaseCar(eastCar);
                releaseCar(westCar);
                count--;
            }
        }while (count > numAllowedCars);
        trafficLight =!trafficLight;
    }