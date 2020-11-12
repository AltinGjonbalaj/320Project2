#include "cache.h"

using namespace std;

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

void writeFile(char* argv[]){
/*    // Open file for writing
    ofstream outfile(argv[2]);
    outfile << alwaysTCount << "," << address.size() << ";" << endl;
    outfile << alwaysNTCount << "," << address.size() << ";" << endl;
    
    for(int i = 0; i < 7; i++){
        if(i!= 6)
            outfile << bOneCount[i] << "," << address.size() << "; ";
        else
            outfile << bOneCount[i] << "," << address.size() << ";";

    }
        outfile << endl;
    for(int i = 0; i < 7; i++){
        if(i!=6)
            outfile << bTwoCount[i] << "," << address.size() << "; ";
        else
            outfile << bTwoCount[i] << "," << address.size() << ";";

    }
        outfile << endl;
    for(int i = 0; i < 9; i++){
        if(i!=8)
            outfile << gCount[i] << "," << address.size() << "; ";
        else
            outfile << gCount[i] << "," << address.size() << ";";
    }
        outfile << endl;
    outfile << tCount << "," << address.size() << ";" << endl;
    outfile << btbCount << "," << btbTotal << ";" << endl;
    outfile.close();
*/}




//Methods
void directMapped(){

}

void setAsso(){

}

void fullyAsso(){

}



