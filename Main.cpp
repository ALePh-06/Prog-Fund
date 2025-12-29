#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

string extractType(const string& s);
string extractValue(const string& s);
bool isValidInt(const std::string& s);
void inStore();
bool checkOrCreateCSV(const string& filename);

bool checkOrCreateCSV(const string& filename) {
    ifstream infile(filename);
    if (infile.good()) {
        infile.close();
        return true; // File already exists
    }
    infile.close();

    // File does not exist → create it
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Error creating file.\n";
        return false;
    }

    int columns;
    cout << "CSV file not found. Creating a new one.\n";
    cout << "Enter number of columns: ";
    cin >> columns;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < columns; i++) {
        string name, type;

        cout << "Enter column " << i + 1 << " name: ";
        getline(cin, name);

        cout << "Enter column " << i + 1 << " type (INT or STRING): ";
        getline(cin, type);

        outfile << name << "(" << type << ")";
        if (i < columns - 1) {
            outfile << ", ";
        }
    }

    outfile.close();
    cout << "CSV file created successfully.\n";
    return true;
}


int main (){
    if (checkOrCreateCSV("test.txt")) {
    inStore();
    }
    return 0;
}

///Input Insertion into the file
void inStore(){
    ifstream infile;
    infile.open("test.txt");
    ofstream outfile;
    outfile.open("test.txt", ios::app);
    string line, temp;
    int column, i;
    vector<string> data, input;
    getline(infile, line);
    stringstream s(line);
    ///Insert the segmented line into a vector
    while(getline(s, temp, ',')){
        data.push_back(temp);
    }

    column = data.size();
    input.resize(column);
    outfile << "\n";
    for(int i = 0; i < column; i++){
        string type = extractType(data[i]);
        string value = extractValue(data[i]);
        int number;
        string tempStr;
        cout << "Please enter the " << value << ":" << endl;
        while (true) {
        getline(cin, tempStr);

        if (type == "INT") {
            if (!isValidInt(tempStr)) {
                cout << "Invalid input. Please enter an integer: ";
                continue;
                }
            }
            break;
        }
        input[i] = tempStr;
        outfile << input[i];
        if (i < column - 1){
            outfile << ", ";
        }
    }

    infile.close();
    outfile.close();
}

///Taking out the string between '(' and ')'
std::string extractType(const std::string& s)
{
    size_t start = s.find('(');
    size_t end = s.find(')');
    if (start == std::string::npos || end == std::string::npos || end <= start) {
    return "";  
    }
    return s.substr(start + 1, end - start - 1);
}

///Taking out the string before the '('
std::string extractValue(const std::string& s)
{
    size_t end = s.find('(');
    return s.substr(0, end);
}

///Input sanity check
bool isValidInt(const std::string& s) {
    try {
        size_t pos;
        std::stoi(s, &pos);
        return pos == s.length(); //Making sure the whole string is numbers
    } catch (...) {
        return false;
    }
}