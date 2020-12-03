#include "cache.h"
using namespace std;

//Used when calling different caches
int cacheSize[4] = {32, 128, 512, 1024};
int assoc[4] = {2, 4, 8, 16};

vector<unsigned long long> memAddr;
vector<char> types;

//Input and Output
void readFile(char* argv[]){
    unsigned long long addr;
    char type;

    // Open file for reading
    ifstream infile(argv[1]);
    // The following loop will read a hexadecimal number and
    // a type each time and then output them
    while(infile >> type >> hex >> addr) {
        types.push_back(type);
        //printf("Addr: %lu", addr);
        types.push_back(type);
        memAddr.push_back(addr);
    }
    infile.close();
}

int main(int argc, char *argv[]){
    readFile(argv);
    ofstream outfile(argv[2]);
    for(int i = 0; i < 4; i++){
        outfile << directMapped(cacheSize[i]) << "," << memAddr.size() << "; ";
    }
    outfile << endl;
    for(int i = 0; i < 4; i++){
        outfile << setAsso(assoc[i]) << "," << memAddr.size() << "; ";
    }
    outfile << endl;
    outfile << fullyAsso() << "," << memAddr.size() << "; ";
    outfile << endl;
    //fully hot cold no work
    outfile << endl;
    for(int i = 0; i < 4; i++){
        outfile << noAlloc(assoc[i]) << "," << memAddr.size() << "; ";
    }
    outfile << endl;
    for(int i = 0; i < 4; i++){
        outfile << nPref(assoc[i]) << "," << memAddr.size() << "; ";
    }
    outfile << endl;
    for(int i = 0; i < 4; i++){
        outfile << missPref(assoc[i]) << "," << memAddr.size() << "; ";
    }  
    outfile << endl;
    outfile.close();
    return 0;
}
