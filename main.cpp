#include <iostream>
#include <fstream>

using namespace std;
int main()
{
    ifstream inFile;
    // open the file stream
    inFile.open("simple.txt");
    // check if opening a file failed
    if (inFile.fail()) {
        cerr << "Error opeing a file" << endl;
        inFile.close();
        exit(1);
    }
    string line;
    while (getline(inFile, line))
    {
        cout << line << endl;
    }
    // close the file stream
    inFile.close();
}