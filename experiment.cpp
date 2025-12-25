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


getline(file, line); //read and discard headers
while (getline(file, line)) { //read any remaining lines
        int ID;
        string name, status;
        

        stringstream ss(line);

    getline(ss,name, ','); //read any values seperated by comma, not sure if work
    ss>> ID;
    ss.ignore(); 
    ss>>status;

    cout    << "Name: " <<name //data output
            << " , ID: " <<ID
            << " , Status: " <<status << endl;

}
file.close();
return 0;
}