#ifndef FUNC_H
#define FUNC_H

#include<iostream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

double getrelVol(string line);
double GetrelVolonAbsDose(vector<string> const &structure, double num);
double getAbsDoseonRelVol(vector<string> const &structure, double num);
double HI(vector<string> const &structure);
string getvolume(string line);
string getminmaxdose(string line);
string getmeandose(string line);
bool structsearch(string line);


#endif // FUNC_H
