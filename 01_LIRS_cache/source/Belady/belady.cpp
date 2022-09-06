#include <iostream>
#include <cassert>

#include "belady.hpp"

int slow_get_page_int(int key) {                // slow get page imitation
    
    return key; 
}

int main() {

    unsigned cache_size = 0;
    unsigned num_count = 0;
    unsigned hits = 0;

    std::cin >> cache_size >> num_count;
    assert(std::cin.good());

    caches::prescient <int> cache{cache_size, num_count}; 

    cache.fill();
    hits = cache.prescient_algoritm(slow_get_page_int);

    std::cout << hits << std::endl;

    return 0;
}