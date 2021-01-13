#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
void remove_adjacent_duplicate(string& dat1, string dat2);
vector<string> split(string dat, string separator);

// template<class T> 
void WriteVector(string path,vector<int>& out);