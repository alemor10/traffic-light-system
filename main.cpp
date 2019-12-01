#include <iostream>
#include <string>
#include <fstream>
#import <pthread.h>
#include <stdio.h>

#include "includes/Simulation.h"

 #define NUM_THREADS     40

using namespace std;

void *DoTask(void *anArg)
{
    long tArg;
     
    // retrieve the thread argument
    tArg = (long)anArg;
    printf("DoTask:input:argument:%d\n", tArg);
     
    //
    // RUN THE REST OF THE TASK...
    //
}

void readFile(int num){
	    
    int arrivaltime;
    string direction;						
	string file;			
	ifstream myfile;
    
    pthread_t threads[NUM_THREADS];
    int threadcounter = 0;

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
	
	while (myfile >> arrivaltime >> direction){
		
        cout << arrivaltime << direction  << endl;

        if(direction == "N" )
        {
            int tErr;
            tErr = pthread_create(&threads[threadcounter] , NULL, DoTask, (void *) arrivaltime );
            if(tErr)
            {
                cout << "error creating thread wtf!" <<endl;
                exit(1);
            }
            threadcounter++;
        }
        else if (direction == "S")
        {
            int tErr;
            tErr = pthread_create(&threads[threadcounter] , NULL, DoTask, (void *) arrivaltime );
            if(tErr)
            {
                cout << "error creating thread wtf!" <<endl;
                exit(1);
            }
            threadcounter++;
        }
        else if (direction == "E")
        {
            int tErr;
            tErr = pthread_create(&threads[threadcounter] , NULL, DoTask, (void *) arrivaltime );
            if(tErr)
            {
                cout << "error creating thread wtf!" <<endl;
                exit(1);
            }
            threadcounter++;
        }
        else if (direction == "W")
        {
            int tErr;
            tErr = pthread_create(&threads[threadcounter] , NULL, DoTask, (void *) arrivaltime );
            if(tErr)
            {
                cout << "error creating thread wtf!" <<endl;
                exit(1);
            }
            threadcounter++;
        }
        else exit(1);
	}		
	 
    myfile.close();
	
    return;
}



int main()
{

    int file = 0; 


    Simulation sim; 


    cout << "Which file would you like to read?" <<endl;
    cout << "1. Easy.Txt" <<endl;
    cout << "2. Medium.Txt" <<endl;
    cout << "3. Hard Text" <<endl;
    cin >> file;

    readFile(file);


    return 0;
}