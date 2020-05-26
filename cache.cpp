#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cmath>
#include <utility>

using namespace std;

#define  BLOCK_SIZE     16

void n2_fifo(int CACHE_SIZE, int associativity, vector<unsigned long long> trace){
    //std::ifstream inFS("trace");
    int number_of_blocks = CACHE_SIZE / (BLOCK_SIZE * associativity);
    //std::cout << log2(number_of_blocks) << "\n";
    int index_bitmask = pow(2,log2(number_of_blocks) ) - 1;
//    std::cout << std::hex << index_bitmask << "\n" ;
    std::vector<std::deque<unsigned long long> > cache;

    for (int i = 0; i < number_of_blocks; ++i){
        std::deque<unsigned long long> block;
        for(int j= 0 ; j < associativity; j++) block.push_back(0);
        cache.push_back(block);
    }

   // read memory accesses from standard input
    unsigned long long read_line;
    int total = 0, miss = 0;
    for(int a = 0; a < trace.size(); a++) {
        read_line = trace.at(a);
        total++;
        int tag   = read_line >> ((10 - (associativity / 2)) + 4); // get tag
        int index = read_line >> 4;        // get index
        //int offset = read_line & 0x1;
        index = index & index_bitmask;

      // check for miss
      bool missBool = true;
        for(int k = 0; k < associativity; k++){
            if(cache[index][k] == tag) missBool = false;
        }
      

        if(missBool){
            miss++;
            cache[index].push_back(tag);
            cache[index].pop_front();
        }

   }
    double miss_rate = (double)miss / (double)(total);

    //printf("total accesses: %d\n", total);
    printf("miss rate:      %0.2lf\n", miss_rate * 100.0);
    //inFS.close();
}

void lru(int CACHE_SIZE, int associativity,vector <unsigned long long> trace) {
    //cin.clear();
    //std::ifstream inFS("trace");
    int number_of_blocks = CACHE_SIZE / (BLOCK_SIZE * associativity);
    //std::cout << log2(number_of_blocks) << "\n";
    int index_bitmask = pow(2,log2(number_of_blocks) ) - 1;
//    std::cout << std::hex << index_bitmask << "\n" ;
    std::vector <std::vector<pair<unsigned long long,int>>> cache;

    for (int i = 0; i < number_of_blocks; ++i){
        std::vector < pair<unsigned long long,int> > block;
        for(int j= 0 ; j < associativity; j++) block.push_back(make_pair((unsigned long long)0,j));
        cache.push_back(block);
    }

   // read memory accesses from standard input
    unsigned long long read_line;
    int total = 0, miss = 0;
    for(int a = 0; a < trace.size(); a++) {
        total++;
        read_line = trace.at(a);
        int tag   = read_line >> ((10 - (associativity / 2)) + 4); // get tag
        int index = read_line >> 4;        // get index
        //int offset = read_line & 0x1;
        index = index & index_bitmask;

      // check for miss
      bool missBool = true;
        for(int k = 0; k < associativity; k++){
            if(cache[index][k].first == tag){
                missBool = false;
                cache[index][k].second = 0;
            }
            else cache[index][k].second++;
            
        }
      

        if(missBool){
            miss++;
            int maxIndex = 0;
            for( int  k = 1; k < associativity;k++){
                if(cache[index][k].second > cache[index][maxIndex].second) maxIndex = k;
            }
            cache[index][maxIndex] = make_pair(tag,0);
        }

   }
    double miss_rate = (double)miss / (double)(total);

    //printf("total accesses: %d\n", total);
    printf("miss rate:      %0.2lf\n", miss_rate * 100.0);
    //inFS.close();


}


int main () {

   // build cache
    //direct_fifo(16384);
    vector< unsigned long long> trace;
    unsigned long long read_line; 
    while( cin >> hex >> read_line) trace.push_back(read_line);
    n2_fifo(16384,1,trace);
    n2_fifo(16384,2,trace);
    n2_fifo(16384,4,trace);
    n2_fifo(16384,8,trace);
    lru(16384,1,trace);
    lru(16384,2,trace);
    lru(16384,4,trace);
    lru(16384,8,trace);

   return 0;
}
