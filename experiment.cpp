#include <iostream>
#include <fstream> ///provides classes for reading and writing into files or data streams
#include <sstream> ///allows reading and writing to strings
#include <string>

using namespace std;

int main() 
{
    ifstream file("file.csv");///opens a csv file for reading
    string line;

    if (!file.is_open())
    {
        cout<<"Error, file cannot be opened."<<endl;
        return 1;
    }
}