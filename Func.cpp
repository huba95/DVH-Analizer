#include"Func.h"
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>
#include<cmath>

using namespace std;

double getrelVol(string line){
    std::string::size_type sz;
    string b;
/*	stold(line, &sz);
    b = line.substr(sz);
    stold(b, &sz);
    return stold(b.substr(sz));*/
    sz=line.find_last_of(" ");
    b = line.substr(sz + 1);
    return stold(b);
}

double GetrelVolonAbsDose(vector<string> const &structure, double num){
    std::vector<string>::size_type i = 0;
    while (i <= (structure.size() - 2))
    {
        if (stold(structure.at(i)) == num){
            return getrelVol(structure.at(i));
        }
        i++;
    }
    return -1;
}

double getAbsDoseonRelVol(vector<string> const &structure, double num){
    int i = 0;
    double prev;
    if (num < 0 || num > 100){ return -1; }
    while (getrelVol(structure.at(i)) - num > 0){
        prev = getrelVol(structure.at(i)) - num;
        i++;
    }
    if (getrelVol(structure.at(i)) - num == 0){
        return stold(structure.at(i));
    }
    if (getrelVol(structure.at(i)) - num < 0){
        if (prev < abs(getrelVol(structure.at(i)) - num)){
            return  stold(structure.at(i - 1));
        }
        else{
            return  stold(structure.at(i));
        }
    }
    return 0;
}

double HI(vector<string> const &structure){
    double D98 = getAbsDoseonRelVol(structure, 98.0);
    double D02 = getAbsDoseonRelVol(structure, 2.0);
    double D50 = getAbsDoseonRelVol(structure, 50.0);

    double hi = (D02 - D98) / D50;
    return hi;

}

string getvolume(string line){
    line.erase(6, 6);
    line += " cm3";
    return line;
}

string getminmaxdose(string line){
    line.erase(8, 6);
    line += " cGy";
    return line;
}

string getmeandose(string line){
    line.erase(9, 6);
    line += " cGy";
    return line;
}

bool structsearch(string line)
{
    string Struct("Structure");
    if (line.compare(0, Struct.length(), Struct) == 0)
        return true;
    else
        return false;

}
