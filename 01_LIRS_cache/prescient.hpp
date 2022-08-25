#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <deque>


namespace caches {

    template <typename Type, typename KeyT = int>
    struct prescient {
        
        private:

            unsigned size_;
            unsigned num_count_;
            std::unordered_set<Type> cache_;
            std::vector<KeyT> keys_;
            std::unordered_map<KeyT, std::deque<int>> hash_;

        public:
            prescient(unsigned size, unsigned num_count) : size_(size), num_count_(num_count) {
                keys_.reserve(num_count);
            }

            void fill() {

                for (int i = 0; i < num_count_; ++i) {
                    int num;
                    std::cin >> num;
                    assert(std::cin.good());

                    keys_[i] = num;
                    hash_[num].push_back(i);
                }
            }
            
            template <typename Func> unsigned prescient_algoritm(Func slow_get_page) {

                unsigned hits = 0;

                for (int i = 0; i < num_count_; ++i) {
                    
                    auto key = keys_[i];

                    if (cache_.size() < size_) {
                        first_fill_cache(key, &hits, slow_get_page);
                    }
                    else {

                        if (cache_.find(key) != cache_.end()){
                            ++hits;
                        }
                        else {
                            not_found_in_cache(key, slow_get_page);
                        }
                        hash_[key].pop_front();
                    }
                }
                return hits;
            }

        private:
        
            template <typename Func> void first_fill_cache(KeyT key, unsigned *hits, Func slow_get_page) {
                
                if (cache_.find(key) == cache_.end()) {
                    cache_.insert(cache_.begin(), slow_get_page(key));
                }
                else {
                    ++(*hits);
                }
                hash_[key].pop_front();
            }

            template <typename Func> void not_found_in_cache(KeyT key, Func slow_get_page) {
                
                auto delete_element = cache_.begin();
                unsigned max_len = 0;

                for (auto j = cache_.begin(); j != cache_.end(); j = std::next(j)) {
                    
                    unsigned near_address = (*(hash_[*j]).begin());
                    if (near_address > max_len && near_address != 0) {
                        delete_element = j;
                        max_len = near_address;
                    }
                    if (near_address == 0) {
                        delete_element = j;
                        break;
                    }
                }   
                cache_.erase(delete_element);
                cache_.insert(cache_.begin(), slow_get_page(key));
            }

//---------------------------------------debug functions---------------------------------------//
            void print_hash() {
                std::cout << "This is hash:" << std::endl;
                for(auto i = hash_.begin(); i != hash_.end(); ++i) {
                    std::cout << "key:" << i->first << " adress:";
                    for (auto j = (i->second).begin(); j != (i->second).end(); j = std::next(j)) {
                        std::cout << *j << " ";
                    }
                }
                std::cout << std::endl;
                std::cout << "/-------------------/" << std::endl;

                std::cout << std::endl;
            }
            
            void print_cache() const {
                std::cout << "This is cache:" << std::endl;
                for(auto i = cache_.begin(); i != cache_.end(); ++i) {
                    std::cout << *i << std::endl;
                }
                std::cout << "/-------------------/" << std::endl;
                std::cout << std::endl;
            }

            void print_keys() const {
                std::cout << "This is keys:" << std::endl;
                for(auto i = 0; i != num_count_; ++i) {
                    std::cout << keys_[i] << std::endl;
                }
                std::cout << "/-------------------/" << std::endl;
                std::cout << std::endl;
            }
//---------------------------------------------------------------------------------------------//
    };
    
}