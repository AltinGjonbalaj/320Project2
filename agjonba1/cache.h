#ifndef cache_h
#define cache_h
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>


extern std::vector<unsigned long long> memAddr;
extern std::vector<char> types;

//Methods
int directMapped(int cacheSize);
int setAsso(int associativity);
int fullyAsso();

int noAlloc(int associativity);
int nPref(int associativity);
int missPref(int associativity);




#endif
