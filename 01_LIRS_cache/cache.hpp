#pragma once

#include <iostream>
#include <unordered_map>
#include <iterator>
#include <list>
#include <utility>
#include <unordered_set>

namespace caches {

    enum state {
        LIR,                                    //The element is marked as LIR. 
        Resident_HIR,                           //The element is marked as Resident_HIR.
        Non_resident_HIR                        //The element is marked as Non_resident_HIR.
    };

    template <typename Type, typename KeyT = int>
    struct cache {
        
        int size_;
        std::unordered_set<Type> cache_;
        using ListIt = typename std::list<KeyT>::iterator;
        using Stack_element = typename std::pair <enum state, KeyT>;
        using StackIt = typename std::list<Stack_element>::iterator;
        using Hash_element = typename std::pair <ListIt, StackIt>;

        std::unordered_map<KeyT, Hash_element> hash_;
        std::list<Stack_element> stack_;
        std::list<KeyT> list_;

        cache(int size) : size_(size) {}

        const int hirsize = (size_ + 3) / 4;    //Equating hirsize to a quarter of the total cache size with rounding up.
        const int lirsize = size_ - hirsize;     //Equating lirsize to to the remaining space in the cache (about three-quarters of it).

        bool full_list (int len_list) const {
            return (len_list == hirsize);
        }

        bool full_Lir(int len_stack) const {
            return (len_stack >= lirsize);
        }

        void delete_last_stack_elem() {
            auto keyptr_list = (hash_.find(stack_.back().second))->second.first;
            hash_.insert_or_assign(stack_.back().second, std::make_pair(keyptr_list, stack_.end()));
            stack_.pop_back();
        }

        void free_stack_to_Lir() {
            while ((stack_.back()).first != LIR) {
                delete_last_stack_elem();
            }
        }

        template <typename Func> bool Lirs_algoritm(KeyT key, Func slow_get_page) {

            bool is_hit = false;
            auto hash_element = hash_.find(key);

            if (size_ < 4) {
                std::cout << "The cache size cannot be less than 4." << std::endl;
                abort();
            }

            if (hash_element == hash_.end() || (hash_element->second.first == list_.end() && hash_element->second.second == stack_.end())) {                             //новый элемент
                
                if (full_list(list_.size())) {

                    stack_.push_front({Resident_HIR, key});
                    list_.push_front(key);

                    auto delete_element_list = hash_.find(list_.back());
                    auto change_element_in_stack = delete_element_list->second.second;
                    if (change_element_in_stack != stack_.end()) {
                        change_element_in_stack->first = Non_resident_HIR;
                    }

                    hash_.insert_or_assign(delete_element_list->first, std::make_pair(list_.end(), change_element_in_stack));
                    list_.pop_back();

                    hash_.insert_or_assign(key, std::make_pair(list_.begin(), stack_.begin()));

                    cache_.erase(delete_element_list->first);
                    cache_.insert(slow_get_page(key));
                }
                else {

                    if (!full_Lir(stack_.size())) {
                        stack_.push_front({LIR, key});
                        hash_.insert_or_assign(key, std::make_pair(list_.end(), stack_.begin()));
                    }
                    else {
                        stack_.push_front({Resident_HIR, key});
                        list_.push_front(key);
                        hash_.insert_or_assign(key, std::make_pair(list_.begin(), stack_.begin()));
                    }

                    cache_.insert(slow_get_page(key));
                }
            }
            else {

                if (hash_element->second.second == stack_.end()) {
                    
                    if (hash_element->second.first != list_.end()) {
                        
                        stack_.push_front({Resident_HIR, key});
                        auto keyptr_list = hash_element->second.first;

                        if (list_.begin() != keyptr_list) {
                            list_.splice(list_.begin(), list_, keyptr_list, std::next(keyptr_list));
                        }

                        hash_.insert_or_assign(key, std::make_pair(list_.begin(), stack_.begin()));

                        is_hit = true;
                    }
                }
                else {

                    auto keyptr_stack = hash_element->second.second;

                    if (keyptr_stack->first == LIR) {
                        
                        if (stack_.begin() != keyptr_stack) {
                            stack_.splice(stack_.begin(), stack_, keyptr_stack, std::next(keyptr_stack));
                        }

                        free_stack_to_Lir();

                        is_hit = true;
                    }

                    else if (keyptr_stack->first == Resident_HIR) {
                        
                        if (stack_.begin() != keyptr_stack) {
                            stack_.splice(stack_.begin(), stack_, keyptr_stack, std::next(keyptr_stack));
                        }
                        keyptr_stack->first = LIR;

                        list_.erase(hash_.find(key)->second.first);
                        hash_.insert_or_assign(key, std::make_pair(list_.end(), stack_.begin()));
                        list_.push_front(stack_.back().second);
                        hash_.insert_or_assign((stack_.back().second), std::make_pair(list_.begin(), stack_.end()));

                        delete_last_stack_elem();
                        free_stack_to_Lir();

                        is_hit = true;
                    }

                    else {

                        if (stack_.begin() != keyptr_stack) {
                            stack_.splice(stack_.begin(), stack_, keyptr_stack, std::next(keyptr_stack));
                        }
                        keyptr_stack->first = LIR;

                        auto elem_to_non_res = hash_.find(list_.back())->second.second;
                        elem_to_non_res->first = Non_resident_HIR;
                        hash_.insert_or_assign(list_.back(), std::make_pair(list_.end(), elem_to_non_res));
                        cache_.erase(list_.back());
                        list_.pop_back();

                        list_.push_front(stack_.back().second);
                        hash_.insert_or_assign((stack_.back().second), std::make_pair(list_.begin(), stack_.end()));

                        delete_last_stack_elem();
                        free_stack_to_Lir();

                        cache_.insert(slow_get_page(key));
                    }
                }
            }
            return is_hit;
        }

//---------------------------------------debug functions---------------------------------------//

        void print_cache() const {
            std::cout << "This is cache:" << std::endl;
            for(auto i = cache_.begin(); i != cache_.end(); i++) {
                std::cout << *i << std::endl;
            }
            std::cout << "/-------------------/" << std::endl;
            std::cout << std::endl;
        }

        void print_hash() const {
            std::cout << "This is hash:" << std::endl;
            for(auto i = hash_.begin(); i != hash_.end(); i++) {
                std::cout << "key:" << i->first << " list:";
                if (i->second.first == list_.end()) {
                    std::cout << "0";
                }
                else {
                    std::cout << "1";
                }
                std::cout << " stack:";
                if (i->second.second == stack_.end()) {
                    std::cout << "0";
                }
                else {
                    std::cout << "1";
                }
                std::cout << std::endl;
            }
            std::cout << "/-------------------/" << std::endl;
            std::cout << std::endl;
        }

        void print_list() const {
            std::cout << "This is list:" << std::endl;
            for(auto i = list_.begin(); i != list_.end(); i++) {
                std::cout << *i << std::endl;
            }
            std::cout << "/-------------------/" << std::endl;
            std::cout << std::endl;
        }

        void print_stack() const {
            std::cout << "This is stack:" << std::endl;
            for(auto i = stack_.begin(); i != stack_.end(); i++) {
                std::cout << "Type:" << i->first << ", element:" << i->second << std::endl;
            }
            std::cout << "/-------------------/" << std::endl;
            std::cout << std::endl;
        }
//---------------------------------------------------------------------------------------------//

    };
}