#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include <sys/stat.h>
#include <cstdio>

#ifdef _WIN32
#include <direct.h>
#endif

using namespace std;

string extractType(const string &s);   // extract column type
string extractValue(const string &s);  // extract name
bool isValidInt(const std::string &s); // check if input is valid int
void viewSheet(string filename);
bool directoryExists(const string &dirName);//directory check
bool createDirectory(const string &dirName); //directory create
string handleDirectory();
void errorDemo();
bool isValidType(string type) // Convert to uppercase
{

    for (char &c : type)
    {
        c = toupper(c);
    }

    return (type == "INT" || type == "STRING");
}

bool isValidColumnName(string name) // remove empty space
{

    size_t start = name.find_first_not_of(" \t");
    if (start == string::npos)
        return false;

    size_t end = name.find_last_not_of(" \t");
    name = name.substr(start, end - start + 1);

    return !name.empty();
}
void inStore(const string &filename);
void inEdit(const string &filename, const string &folder);
string inUpdate(const string &filename, string &line);
void fileswap(const string &filename, const string &folder);

bool checkOrCreateCSV(const string &filename)
{
    ifstream infile(filename); // if exist, add data to new row
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

    ofstream outfile(filename); // opens file
    if (!outfile.is_open())
    {
        cerr << "Error creating file.\n";
        return false;
    }

    int column;
    string temp;

    while (true)
    {
        cout << "Enter number of columns (1-10): ";
        getline(cin, temp);

        if (isValidInt(temp))
        {
            column = stoi(temp);
            if (column > 0 && column <= 10)
            {
                break;
            }
        }

        cout << "Invalid input. Please enter an integer between 1 and 10.\n";
    }

    cout << endl;

    for (int i = 0; i < column; i++)
    {
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
                for (char &c : type)
                    c = toupper(c);
                break;
            }

            cout << "Invalid type. Please enter INT or STRING only.\n";
        }

        outfile << name << "(" << type << ")";

        if (i < column - 1)
            outfile << ",";


    }
    outfile << "\n";
    outfile.close();
    cout << "File created successfully.\n"
         << endl;
    return true;
}

void divider(string text);

int main() // prompts filename and check
{
    divider("STUDENT ATTENDANCE TRACKER - MILESTONE 1");
    string folder = handleDirectory();
if (folder.empty())
{
    cout << "Operation cancelled.\n";
    return 0;
}

string filename;

while (true)
{
    cout << "Enter file name (please put '.csv' after the file name): ";
    getline(cin, filename);

    // Prevent empty or spaces-only input
    if (filename.find_first_not_of(" \t") == string::npos)
    {
        cout << "File name cannot be empty. Please try again.\n";
        continue;
    }

    //Enforce .csv extension
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".csv")
    {
        cout << "File name must end with .csv\n";
        continue;
    }

    break;
}

filename = folder + "/" + filename;


    if (checkOrCreateCSV(filename))
    {
        int inputs;
        string tempinputs, confirm;
        cout << "Do you want to add new entry to the sheet? Y to confirm, else to cancel" << endl;
        getline(cin, tempinputs);
        if (tempinputs == "Y" || tempinputs == "y")
        {
            cout << "How many entry do you want to add?" << endl;
            getline(cin, confirm);
             while (!isValidInt(confirm) || stoi(confirm) <= 0)
        {
        cout << "Please enter a positive integer: ";
        getline(cin, confirm);
        }
        inputs = stoi(confirm);
        for (int i = 0; i < inputs; i++)
        {
            inStore(filename);
        }
        }
       
    }
    else
    {
        cout << "Operation cancelled.\n";
    }
    inEdit(filename, folder);
    divider("View Attendance Sheet (CSV Mode)");
    viewSheet(filename);

    divider("Basic Error Handling Demo");
    errorDemo();

    divider("End of Milestone 1 Output");
    return 0;
}
bool directoryExists(const string &dirName)
{
    struct stat info;
    if (stat(dirName.c_str(), &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
}

bool createDirectory(const string &dirName)
{
#ifdef _WIN32
    return _mkdir(dirName.c_str()) == 0;
#else
    return mkdir(dirName.c_str(), 0777) == 0;
#endif
}

string handleDirectory()
{
    string folder;
    char choice;

    while (true)
    {
        cout << "Enter folder name to store attendance file: ";
        getline(cin, folder);
        
        if (folder.find_first_not_of("\t")== string::npos)
        {
            cout<<"Folder name is empty. Please give a name.\n\n";
            continue;
        }
        if (directoryExists(folder))
        {
            cout << "Folder exists. Using folder: " << folder << endl << endl;
            return folder;
        }
        else
        {
            cout << "Folder does not exist. Create it? (y/n): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 'y' || choice == 'Y')
            {
                if (createDirectory(folder))
                {
                    cout << "Folder created successfully.\n\n";
                    return folder;
                }
                else
                {
                    cout << "Error creating folder. Try again.\n\n";
                }
            }
            else
            {
                cout << "Folder creation cancelled.\n\n";
                return "";
            }
        }
    }
}

// adds input into file
void inStore(const string &filename)
{
    ifstream infile;
    infile.open(filename);
    ofstream outfile;
    outfile.open(filename, ios::app); /// open file in append mode
    string line, temp;
    int column, i;
    vector<string> data, input;
    getline(infile, line);
    stringstream s(line);
    /// Insert the segmented line into a vector
    while (getline(s, temp, ','))
    {
        data.push_back(temp);
    }

    column = data.size();
    input.resize(column); /// resizing input vector to match the number of column in file
    divider("Insert New Attendance Row");
    for (int i = 0; i < column; i++)
    {
        string type = extractType(data[i]);   /// getting the column type
        string value = extractValue(data[i]); /// getting the column name
        string tempStr;
        cout << "Please enter the " << value << " (" << type << ")" << ":";
        while (true)
        {
            getline(cin, tempStr);

            if (type == "INT")
            { /// input sanity check
                if (!isValidInt(tempStr))
                {
                    cout << "Invalid input. Please enter an integer: ";
                    continue;
                }
            }
            break;
        }
        input[i] = tempStr;
        outfile << input[i]; /// appending input into the file
        if (i < column - 1)
        {
            outfile << ",";
        }
    }
    outfile << "\n";

    cout << endl;

    infile.close();
    outfile.close();
}

void inEdit(const string &filename, const string &folder){
    string temp = folder + "/temp.csv";
    ifstream infile;
    infile.open(filename);
    ofstream tempfile;
    tempfile.open(temp, ios::app);
    string line, keyword, confirm;
    cout << "Please enter the keyword for the line that is to be updated/deleted: ";
    getline(cin, keyword);
    getline(infile, line);
    tempfile << line << endl;
    while(getline(infile, line)){

        if (line.find(keyword) != std::string::npos){
            cout << "This line contains the keyword inputted." << endl;
            cout << line << endl;
            cout << "Do you want to update or delete this line? Enter '1' to update, '2' to delete, else to cancel." << endl;
            getline(cin, confirm);
            if (confirm =="1")
            {        
            string out = inUpdate(filename, line);
            tempfile << out << endl;
            }   
            else if (confirm == "2")
            {
                continue;
            }

            else
            {
                tempfile << line << endl;
            }
        }
        else
        {
        tempfile << line << endl;
        }
    }
    infile.close();
    tempfile.close();

    fileswap(filename, temp);
}

string inUpdate(const string &filename, string &line){
    ifstream infile;
    infile.open(filename);
    string temtem, fRow;
    getline(infile, fRow);


    int column, i;
    vector<string> data, input;
    stringstream s;
    s << fRow;
    /// Insert the segmented line into a vector
    while (getline(s, temtem, ','))
    {
        data.push_back(temtem);
    }

    column = data.size();
    input.resize(column);

    for (int i = 0; i < column; i++)
    {
        string type = extractType(data[i]);   /// getting the column type
        string value = extractValue(data[i]); /// getting the column name
        string tempStr;
        cout << "Please enter the " << value << " (" << type << ")" << ":";
        while (true)
        {
            getline(cin, tempStr);

            if (type == "INT")
            { /// input sanity check
                if (!isValidInt(tempStr))
                {
                    cout << "Invalid input. Please enter an integer: ";
                    continue;
                }
            }
            break;
        }
        data[i] = tempStr;/// appending input into the file
    }
    infile.close();
    string out;
    for (size_t i=0; i < data.size(); ++i){
        out += data[i];
        if(i !=data.size() -1){
            out+=",";
        }
    }
    return out;
}

void fileswap(const string &filename, const string &temp){
    string finalname = filename;
    remove(finalname.c_str());
    rename(temp.c_str(), finalname.c_str());
}

/// Taking out the string between '(' and ')'
std::string extractType(const std::string &s)
{
    size_t start = s.find('('); /// finding the location of (
    size_t end = s.find(')');   /// finding the location of )
    if (start == std::string::npos || end == std::string::npos || end <= start)
    {
        return ""; /// error handlig due to incorrect format
    }
    return s.substr(start + 1, end - start - 1);
}

/// Taking out the string before the '('
std::string extractValue(const std::string &s)
{
    size_t end = s.find('(');
    return s.substr(0, end);
}

/// Input sanity check
bool isValidInt(const std::string &s)
{
    try
    {
        size_t pos;
        std::stoi(s, &pos);
        return pos == s.length(); // Making sure the whole string is numbers
    }
    catch (...)
    {
        return false;
    }
}

void divider(string text)
{
    cout << "-------------------------------------------" << endl
         << text << endl
         << "-------------------------------------------" << endl
         << endl;
}

void viewSheet(string filename) // displays the data in CSV Mode
{
    string text;
    ifstream sheet(filename);
    while (getline(sheet, text))
    {
        cout << text << endl;
    }
    cout << endl;
    sheet.close();
}

void errorDemo()
{
    string tempInt;

    cout << "Enter ID (int): ";
    getline(cin, tempInt);

    if (!isValidInt(tempInt))
    {
        cout << "Invalid input. Please enter an integer: " << endl
             << endl;
    }
}

// Delete part (Muiz)


