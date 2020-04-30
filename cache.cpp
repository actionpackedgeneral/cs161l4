#include <iostream>
#include <vector>
#include <fstream>

#define  BLOCK_SIZE     16

void direct_fifo(int CACHE_SIZE){
    std::ifstream inFS;
    inFS.open("trace");
    int number_of_blocks = CACHE_SIZE / BLOCK_SIZE;
    std::vector<unsigned long long> cache;

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
void n2_fifo(int CACHE_SIZE){
    std::ifstream inFS("trace");
    int number_of_blocks = CACHE_SIZE / BLOCK_SIZE;
    std::vector<std::vector<unsigned long long>> cache;

    for (int i = 0; i < number_of_blocks; ++i){
        std::vector<unsigned long long> block;
        block.reserve(2);
        cache.push_back(block);
    }

   // read memory accesses from standard input
    unsigned long long read_line;
    int total = 0, miss = 0;
    while (inFS >> std::hex >> read_line) {
        total++;
        unsigned long long tag   = read_line >> (10 + 4); // get tag
        unsigned long long index = read_line >> 4;        // get index
        unsigned long long offset = read_line & 0x1;
        index = index & 0x00000000000003FF;

      // check for miss
        if(cache[index][offset] != tag) {
            cache[index][offset] = tag;
            miss++;
        }
   }
    double miss_rate = (double)miss / (double)(total);

    printf("total accesses: %d\n", total);
    printf("miss rate:      %0.2lf\n", miss_rate * 100.0);
    inFS.close();
}


int main () {
   // build cache
    direct_fifo(16384);
    n2_fifo(16384);

   return 0;
}

