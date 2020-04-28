#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

#define  BLOCK_SIZE     16
//#define  CACHE_SIZE     16384

int main () {
   // build cache
   vector<int> CACHE_SIZE = {1024,2048,4096,8192,16384};
   for(int a = 0; a < CACHE_SIZE.size(); a++){
        int number_of_blocks = CACHE_SIZE.at(a) / BLOCK_SIZE;
        vector<unsigned long long> cache;

        for (int i = 0; i < number_of_blocks; ++i)
            cache.push_back(0);

        // read memory accesses from standard input
        ifstream inFS;
        inFS.open("trace.txt");
        unsigned long long read_line;
        int total = 0, miss = 0;
        while (inFS >> hex >> read_line) {
            total++;
            unsigned long long tag   = read_line >> (10 + 4); // get tag
            unsigned long long index = read_line >> 4;        // get index
            index = index & 0x00000000000003FF;
            

            // check for miss
            if (cache[index] != tag) {
                cache[index] = tag;
                //std::cout << index << " " << tag << std::endl;
                miss++;
            }
        }
        double miss_rate = (double)miss / (double)(total);

        printf("total accesses: %d\n", total);
        printf("miss rate:      %0.2lf\n", miss_rate * 100.0);
            inFS.close();
   }
   return 0;
}

