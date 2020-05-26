#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cmath>

using namespace std;

#define  BLOCK_SIZE     16

void direct_fifo(int CACHE_SIZE){
    
    // initialize file
    std::ifstream inFS;
    inFS.open("trace");
    int number_of_blocks = CACHE_SIZE / BLOCK_SIZE;
    std::vector<unsigned long long> cache;

    // initialize blocks
    for (int i = 0; i < number_of_blocks; ++i)
        cache.push_back(0);

   // read memory accesses from standard input
    unsigned long long read_line;
    int total = 0, miss = 0;
    while (inFS >> std::hex >> read_line) {
        total++;
        unsigned long long tag   = read_line >> (10 + 4); // get tag
        unsigned long long index = read_line >> 4;        // get index
        index = index & 0x00000000000003FF;

      // check for miss
        if(cache[index] != tag) {
            cache[index] = tag;
            miss++;
        }
   }
    double miss_rate = (double)miss / (double)(total);

    printf("total accesses: %d\n", total);
    printf("miss rate:      %0.2lf\n", miss_rate * 100.0);
}
void n2_fifo(int CACHE_SIZE, int associativity){
    std::ifstream inFS("trace");
    int number_of_blocks = CACHE_SIZE / (BLOCK_SIZE * associativity);
    std::cout << log2(number_of_blocks) << "\n";
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
    while (inFS >> std::hex >> read_line) {
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

    printf("total accesses: %d\n", total);
    printf("miss rate:      %0.2lf\n", miss_rate * 100.0);
    inFS.close();
}

void 


int main () {
   // build cache
    direct_fifo(16384);
    n2_fifo(16384,1);
    n2_fifo(16384,2);
    n2_fifo(16384,4);
    n2_fifo(16384,8);

   return 0;
}
