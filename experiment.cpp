#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include <filesystem>

namespace fs= std::filesystem; //filesystem library
using namespace std;

string handleDirectory();
string extractType(const string& s); //extract column type
string extractValue(const string& s);//extract name

bool isValidInt(const std::string& s); //check if input is valid int
bool isValidType(string type) {
    // Convert to uppercase
    for (char &c : type) {
        c = toupper(c);
    }

    return (type == "INT" || type == "STRING");
}

string handleDirectory() {
    string dirName;
    while (true) {
        cout << "Enter the category (folder) name: ";
        getline(cin, dirName);

        if (fs::exists(dirName)) {
            if (fs::is_directory(dirName)) {
                cout << "Folder found. Entering '" << dirName << "'...\n";
                return dirName;
            } else {
                cout << "Error: A file with this name already exists, but it's not a folder.\n";
            }
        } else {
            char choice;
            cout << "Folder does not exist. Create and enter it? (y/n): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

            if (choice == 'y' || choice == 'Y') {
                if (fs::create_directory(dirName)) {
                    cout << "Folder created successfully.\n";
                    return dirName;
                }
            }
            cout << "Please enter a valid folder name to proceed.\n";
        }
    }
}

bool isValidColumnName(string name) //remove empty space
{
    
    size_t start = name.find_first_not_of(" \t");
    if (start == string::npos) return false;

    size_t end = name.find_last_not_of(" \t");
    name = name.substr(start, end - start + 1);

    return !name.empty();
}
void inStore(const string& filename);


bool checkOrCreateCSV(const string& filename) 
{
    ifstream infile(filename); //if exist, add data to new row
    if (infile.good()) 
    {
        infile.close();
        cout << "File exists. Opening file...\n";
        return true;
    }
    infile.close();

    // File does not exist → ask user
    char choice;
    cout << "File does not exist.\n";
    cout << "Do you want to create it? (y/n): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice != 'y' && choice != 'Y') 
    {
        return false;
    }

    ofstream outfile(filename); //opens file
    if (!outfile.is_open()) 
    {
        cerr << "Error creating file.\n";
        return false;
    }

int column;
string temp;

while (true) 
{
    cout << "Enter number of columns: ";
    getline(cin, temp);

    if (isValidInt(temp)) 
    {
        column = stoi(temp);
        if (column > 0) {
            break;
        }
    }

    cout << "Invalid input. Please enter a positive integer.\n";
}




    for (int i = 0; i < column; i++) {
        string name, type;

while (true)
 {
    cout << "Enter column " << i + 1 << " name: ";
    getline(cin, name);

    if (isValidColumnName(name)) 
    {
        break;
    }

    cout << "Column name cannot be empty. Please try again.\n";
}

    while (true) 
    {
    cout << "Enter column " << i + 1 << " type (INT or STRING): ";
    getline(cin, type);

    if (isValidType(type)) 
    {
        // normalize to uppercase before storing
        for (char &c : type) c = toupper(c);
        break;
    }

    cout << "Invalid type. Please enter INT or STRING only.\n";
    }

outfile << name << "(" << type << ")";

        if (i < column - 1) outfile << ", ";
    }

    outfile.close();
    cout << "File created successfully.\n";
    return true;
}



int main() //prompts filename and check
   {
    // 1. Ask for folder first
    string folder = handleDirectory();

    // 2. Ask for file
    string filename;
    cout << "Enter file name (e.g., student_data.csv): ";
    getline(cin, filename);

    // 3. Combine folder and filename (e.g., "Category/file.csv")
    string fullPath = folder + "/" + filename;

    // 4. Run your existing logic using fullPath instead of filename
    if (checkOrCreateCSV(fullPath)) {
        inStore(fullPath);
    } else {
        cout << "Operation cancelled.\n";
    }

    return 0;
}


//adds input into file
void inStore(const string& filename)
{
    ifstream infile;
    infile.open(filename);
    ofstream outfile;
    outfile.open(filename, ios::app);
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