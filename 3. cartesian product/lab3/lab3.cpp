#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;

constexpr auto OPENSET_SYMBOL = '{';
constexpr auto CLOSESET_SYMBOL = '}';
constexpr auto OPENTUPLE_SYMBOL = '<';
constexpr auto CLOSETUPLE_SYMBOL = '>';

string inputpath = "input.txt";
string outputpath = "output.txt";

ifstream fin;
ofstream fout;

vector <string> getSets(string inputpath) { 
    /*
    The function for obtaining sets from a file as follows:
    A={1,2,3}
    B={4,5,6}
    Returns vector of strings, representing sets.
    */
    fin.open(inputpath);
    if (!fin.is_open())
        cout << "Error while opening file (" + inputpath + ")!" << endl;
    vector <string> sets;
    int line = 0;
    while (!fin.eof()) {
        line++;
        string buff;
        getline(fin, buff);
        cout << buff << endl; // print sets to console
        if ((buff[2] == '{') && (buff[1] == '=') && (buff[buff.length() - 1] == '}')) {
            int opens = 0;
            int closes = 0;
            for (int i = 0; i < buff.length(); i++) {
                if (buff[i] == OPENSET_SYMBOL || buff[i] == OPENTUPLE_SYMBOL)
                    opens++;
                if (buff[i] == CLOSESET_SYMBOL || buff[i] == CLOSETUPLE_SYMBOL)
                    closes++;
            }
            if (opens != closes) {
                cout << "Check input for correctness! Line: " + to_string(line) + ", File: " + inputpath << endl;
                cout << "Number of '<' or '{' is not equal to number of '>' or '}'" << endl;
                exit(0);
            }
            sets.push_back(buff);
        }
        else {
            cout << "Check input for correctness! Line: " + to_string(line) + ", File: " + inputpath << endl;
            exit(0);
            break;
        }
    }
    return sets;
}


vector <vector<string>> splitSets(vector <string> sets, char delimiter) {
    /*
    The function for splitting array of strings representing sets into matrix of elements that are 
    delimited by [delimiter].
    Returns vector-matrix of substrings.
    ISSUE: does not work with very complicated sets which have above 4-5 nested structures!
    */
    int numSets = sets.size();
    vector <vector<string>> elements(numSets);
    for (int i = 0; i < numSets; i++) {
        string buff;
        buff = sets[i].substr(3, sets[i].length() - 4);
        stringstream ss(buff);
        string token;
        string element = "";
        int setOpened = 0;
        int tupleOpened = 0;
        while (getline(ss, token, delimiter)) {

            if (token[0] == OPENSET_SYMBOL) {
                element += token + ',';
                setOpened++;
                for (int i = 1; i < token.length(); i++) {
                    if (token[i] == '{' || token[i] == '<') {
                        if (token[i] == '{')
                            setOpened++;
                        if (token[i] == '<')
                            tupleOpened++;
                    }
                    else break;
                        
                }
                continue;
            }

            if (token[0] == OPENTUPLE_SYMBOL) {
                element += token + ',';
                tupleOpened++;
                for (int i = 1; i < token.length(); i++) {
                    if (token[i] == '{' || token[i] == '<') {
                        if (token[i] == '{')
                            setOpened++;
                        if (token[i] == '<')
                            tupleOpened++;
                    }
                    else break;
                }
                continue;
            }

            if (token[token.length() - 1] == CLOSESET_SYMBOL) {
                setOpened--;
                element += token + ',';
                if (setOpened == 0 && tupleOpened == 0) {
                    if (element[element.length() - 1] == ',')
                        element.pop_back();
                    elements[i].push_back(element);
                    element = "";
                }
                continue;
            }

            if (token[token.length() - 1] == CLOSETUPLE_SYMBOL) {
                tupleOpened--;
                element += token + ',';
                if (setOpened == 0 && tupleOpened == 0) {
                    if (element[element.length() - 1] == ',')
                        element.pop_back();
                    elements[i].push_back(element);
                    element = "";
                }
                continue;
            }

            if (setOpened != 0 || tupleOpened != 0) {
                element += token + ',';
                continue;
            }

            if (setOpened == 0 && tupleOpened == 0) {
                elements[i].push_back(token);
            }
        }
    }
    return elements;
}


vector <string> _cartesianProduct(vector <string> set1, vector <string> set2) {
    /*
    The function for calculation cartesian product of 2 sets
    */
    int size1 = set1.size();
    int size2 = set2.size();
    vector <string> result;

    for (int i = 0; i < set1.size(); i++) {
        for (int j = 0; j < set2.size(); j++) {
            result.push_back(set1[i] + ',' + set2[j]);
        }
    }
    return result;
}


vector <string> cartesianProduct(vector <vector<string>> elements) {
    /*
    A function for calculating cartesian product of multiple sets (2 or more)
    */
    int numSets = elements.size();
    if (numSets == 1)
        return elements[0];
    vector <string> result = elements[0];
    for (int i = 1; i < numSets; i++) {
        result = _cartesianProduct(result, elements[i]);
    }
    return result;
}


string prettify(vector <string> set) {
    /*
     A function to place commas between elements of set and etc.
     accepts vector of elements, returns ready-to-print string.
    */
    string result = "";
    result += OPENSET_SYMBOL;
    for (int i = 0; i < set.size(); i++) {
        result += OPENTUPLE_SYMBOL;
        result += set[i];
        result += CLOSETUPLE_SYMBOL;
        result += ',';
    }
    result[result.length()- 1] = CLOSESET_SYMBOL;
    return result;
}


int main() {
    vector <string> sets;
    sets = getSets(inputpath);

    vector <vector<string>> elements;
    elements = splitSets(sets, ',');

    vector <string> result = cartesianProduct(elements);
        
    cout << "\n Cartesian product=";
    cout << prettify(result) << endl;
}