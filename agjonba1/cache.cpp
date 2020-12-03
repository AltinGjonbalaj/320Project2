#include "cache.h"
#include <cmath>

using namespace std;

struct line{
    bool valid;
    unsigned long long tag;
};

//Methods
int directMapped(int cacheSize){
    int numhits = 0;
    line cache[cacheSize];
    for(int i = 0; i < cacheSize; i++){
        cache[i] = {false, 0};
    }
    for (size_t i = 0; i < memAddr.size(); i++){
        int cacheIndex = (memAddr[i]/32)% cacheSize; //index in cache
        int bits = 5 + log2(cacheSize); //non tag bits
        unsigned long long tag = memAddr[i] >> bits; //tag

        if(cache[cacheIndex].valid == true){
            if(cache[cacheIndex].tag == tag) //cache hit
                numhits++;
            else{ //cache miss
                cache[cacheIndex].tag = tag;
            }
        }
        else{ //compulsory miss
            cache[cacheIndex].valid = true;
            cache[cacheIndex].tag = tag;
        }
    }
    return numhits;
}


int setAsso(int assoc){
    int numhits = 0;
    line cache[512/assoc][assoc];
    vector<int> lru[512/assoc];
    for(int i = 0; i < 512/assoc; i++){
        for(int j = 0; j < assoc; j++){
            cache[i][j] = {false, 0};
        }
    }
    for(size_t i = 0; i < memAddr.size(); i++){
        int cacheIndex = (memAddr[i]/32)%(512/assoc);
        int bits = 5 + log2(512/assoc);
        unsigned long long tag = memAddr[i] >> bits;
        bool hit = false; //used if its a hit or compulsory miss
        int MRU;

        for(int j = 0; j < assoc; j++){ //loops through to find if cache hit
            if(cache[cacheIndex][j].valid == true){
                if(cache[cacheIndex][j].tag == tag){
                    numhits++;
                    hit = true;
                    MRU = j;
                    break;
                }
            }
            else{//space left in set, compulsory miss
                cache[cacheIndex][j].valid = true;
                cache[cacheIndex][j].tag = tag;
                hit = true;
                MRU = j;
                break;
            }
        }
        if(!hit){ //cache miss w/ replacement
            int last = lru[cacheIndex].back();    
            cache[cacheIndex][last].tag = tag;
            MRU = last;
        }

        //remakes LRU with new head
        vector<int> lru2;
        lru2.push_back(MRU);
        for(size_t k = 0; k < lru[cacheIndex].size(); k++){
            int curr = lru[cacheIndex][k];
            if(curr != MRU)
                lru2.push_back(curr);
        }
        lru[cacheIndex] = lru2; //most recently used is now at head
    }

    return numhits;
}


int fullyAsso(){
    return setAsso(512);
}

int noAlloc(int assoc){
   int numhits = 0;
    line cache[512/assoc][assoc];
    vector<int> lru[512/assoc];
    for(int i = 0; i < 512/assoc; i++){
        for(int j = 0; j < assoc; j++){
            cache[i][j] = {false, 0};
        }
    }
    for(size_t i = 0; i < memAddr.size(); i++){
        int cacheIndex = (memAddr[i]/32)%(512/assoc);
        int bits = 5 + log2(512/assoc);
        unsigned long long tag = memAddr[i] >> bits;
        int MRU = -1;

        for(int j = 0; j < assoc; j++){ //loops through to find if cache hit
            if(cache[cacheIndex][j].valid == true && cache[cacheIndex][j].tag == tag){
                numhits++;
                MRU = j;
                break;
            }
            else if(cache[cacheIndex][j].valid == false){
                if(types[i] == 'L'){
                    cache[cacheIndex][j].valid = true;
                    cache[cacheIndex][j].tag = tag;
                    MRU = j;
                }
                break;
            }
            else if(j == assoc -1){ //cache miss w/ replacement
                if(types[i] == 'L'){
                    MRU = lru[cacheIndex].back();    
                    cache[cacheIndex][MRU].tag = tag;
                }
                break;
            }
        }
        //remakes LRU with new head
        if(MRU != -1){
            vector<int> lru2;
            lru2.push_back(MRU);
            for(size_t k = 0; k < lru[cacheIndex].size(); k++){
                int curr = lru[cacheIndex][k];
                if(curr != MRU)
                    lru2.push_back(curr);
            }
            lru[cacheIndex] = lru2; //most recently used is now at head
        }
    }
    return numhits;
}

int nPref(int assoc){
    int numhits = 0;
    line cache[512/assoc][assoc];
    vector<int> lru[512/assoc];
    for(int i = 0; i < 512/assoc; i++){
        for(int j = 0; j < assoc; j++){
            cache[i][j] = {false, 0};
        }
    }
    for(size_t i = 0; i < memAddr.size(); i++){
        int cacheIndex = (memAddr[i]/32)%(512/assoc);
        int bits = 5 + log2(512/assoc);
        unsigned long long tag = memAddr[i] >> bits;
        int MRU;
        int nextIndex = ((memAddr[i] + 32)/32)%(512/assoc);
        unsigned long long tag2 = (memAddr[i] + 32) >> bits;
        for(int j = 0; j < assoc; j++){ //loops through to find if cache hit
            if(cache[cacheIndex][j].tag == tag){
                numhits++;
                MRU = j;
                break;
            }
            else if(cache[cacheIndex][j].valid == false){
                cache[cacheIndex][j].valid = true;
                cache[cacheIndex][j].tag = tag;
                MRU = j;
                break;
            }
            else if(j == assoc -1){ //cache miss w/ replacement
                MRU = lru[cacheIndex].back();    
                cache[cacheIndex][MRU].tag = tag;
                break;
            }
        }
         //remakes LRU with new head
        vector<int> lru2;
        lru2.push_back(MRU);
        for(size_t k = 0; k < lru[cacheIndex].size(); k++){
            int curr = lru[cacheIndex][k];
            if(curr != MRU)
                lru2.push_back(curr);
        }
        lru[cacheIndex] = lru2; //most recently used is now at head
        //THIS IS FOR THE SECOND MEMORY
        int MRU2;
        for(int j = 0; j < assoc; j++){ //loops through to find if cache hit
            if(cache[nextIndex][j].tag == tag2){
                MRU2 = j;
                break;
            }
            else if(cache[nextIndex][j].valid == false){
                cache[nextIndex][j].valid = true;
                cache[nextIndex][j].tag = tag2;
                MRU2 = j;
                break;
            }
            else if(j == assoc -1){ //cache miss w/ replacement
                MRU2 = lru[nextIndex].back();    
                cache[nextIndex][MRU2].tag = tag2;
                break;
            }
        }
        //remakes LRU with new head
        vector<int> lru3;
        lru3.push_back(MRU2);
        for(size_t k = 0; k < lru[nextIndex].size(); k++){
            int curr = lru[nextIndex][k];
            if(curr != MRU2)
                lru3.push_back(curr);
        }
        lru[nextIndex] = lru3; //most recently used is now at head
    }

    return numhits;

}

int missPref(int assoc){
    int numhits = 0;
    line cache[512/assoc][assoc];
    vector<int> lru[512/assoc];
    for(int i = 0; i < 512/assoc; i++){
        for(int j = 0; j < assoc; j++){
            cache[i][j] = {false, 0};
        }
    }
    for(size_t i = 0; i < memAddr.size(); i++){
        int cacheIndex = (memAddr[i]/32)%(512/assoc);
        int bits = 5 + log2(512/assoc);
        unsigned long long tag = memAddr[i] >> bits;
        int MRU;
        bool miss = false;
        int nextIndex = ((memAddr[i] + 32)/32)%(512/assoc);
        unsigned long long tag2 = (memAddr[i] + 32) >> bits;
        for(int j = 0; j < assoc; j++){ //loops through to find if cache hit
            if(cache[cacheIndex][j].tag == tag){
                numhits++;
                MRU = j;
                break;
            }
            else if(cache[cacheIndex][j].valid == false){
                cache[cacheIndex][j].valid = true;
                cache[cacheIndex][j].tag = tag;
                MRU = j;
                miss = true;
                break;
            }
            else if(j == assoc -1){ //cache miss w/ replacement
                MRU = lru[cacheIndex].back();    
                cache[cacheIndex][MRU].tag = tag;
                miss = true;
                break;
            }
        }
         //remakes LRU with new head
        vector<int> lru2;
        lru2.push_back(MRU);
        for(size_t k = 0; k < lru[cacheIndex].size(); k++){
            int curr = lru[cacheIndex][k];
            if(curr != MRU)
                lru2.push_back(curr);
        }
        lru[cacheIndex] = lru2; //most recently used is now at head
        //THIS IS FOR THE SECOND MEMORY
        if(miss){
            int MRU2;
            for(int j = 0; j < assoc; j++){ //loops through to find if cache hit
                if(cache[nextIndex][j].tag == tag2){
                    MRU2 = j;
                    break;
                }
                else if(cache[nextIndex][j].valid == false){
                    cache[nextIndex][j].valid = true;
                    cache[nextIndex][j].tag = tag2;
                    MRU2 = j;
                    break;
                }
                else if(j == assoc -1){ //cache miss w/ replacement
                    MRU2 = lru[nextIndex].back();    
                    cache[nextIndex][MRU2].tag = tag2;
                    break;
                }
            }
            //remakes LRU with new head
            vector<int> lru3;
            lru3.push_back(MRU2);
            for(size_t k = 0; k < lru[nextIndex].size(); k++){
                int curr = lru[nextIndex][k];
                if(curr != MRU2)
                    lru3.push_back(curr);
            }
            lru[nextIndex] = lru3; //most recently used is now at head
        }
    }
    return numhits;
}


//for hot cold, use array implementation of a binary tree for simplicity
//for no allocation, just dont insert into cache if its a store instruction
//for prefetching, do everything norm but every time you check a line you also check the next line -- address + 32, see if its in cahce, and add it if it isnt -> the second line becomes most recently used

