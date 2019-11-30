#include <iostream>
#include <string>

#include "includes/Simulation.h"

using namespace std;

int main()
{

    int file = 0; 


    Simulation sim; 


    cout << "Which file would you like to read?" <<endl;
    cout << "1. Easy.Txt" <<endl;
    cout << "2. Medium.Txt" <<endl;
    cout << "3. Hard Text" <<endl;
    cin >> file;

    sim.readFile(file);

    sim.printCarsArrivalTime();
    sim.printCarsDirection();
    

    return 0;
}