#pragma once

#include <iostream>
#include <unordered_map>
#include <iterator>
#include <list>
#include <utility>
#include <unordered_set>
#include <typeinfo>

namespace caches {

    enum state {
        LIR,                                    //The element is marked as LIR. 
        Resident_HIR,                           //The element is marked as Resident_HIR.
        Non_resident_HIR                        //The element is marked as Non_resident_HIR.
    };

    template <typename Type, typename KeyT = int>
    struct cache {
        
        private:

            unsigned size_;
            std::unordered_set<Type> cache_;
            using ListIt = typename std::list<KeyT>::iterator;
            using Stack_element = typename std::pair <enum state, KeyT>;
            using StackIt = typename std::list<Stack_element>::iterator;
            using Hash_element = typename std::pair <ListIt, StackIt>;
            using HashIt = typename std::unordered_map<KeyT, Hash_element>::iterator;

            std::unordered_map<KeyT, Hash_element> hash_;
            std::list<Stack_element> stack_;
            std::list<KeyT> list_;

            const unsigned hirsize = (size_ + 3) / 4;    //Equating hirsize to a quarter of the total cache size with rounding up.
            const unsigned lirsize = size_ - hirsize;     //Equating lirsize to to the remaining space in the cache (about three-quarters of it).
        
        public:

            cache(unsigned size) : size_(size) {}

            template <typename Func> bool Lirs_algoritm(KeyT key, Func slow_get_page) {

                bool is_hit = false;
                HashIt hash_element = hash_.find(key);

                if (size_ < 4) {
                    std::cout << "The cache size cannot be less than 4." << std::endl;
                    abort();
                }

                if (hash_element == hash_.end() || (hash_element->second.first == list_.end() && hash_element->second.second == stack_.end())) {
                    first_entery(key, slow_get_page);
                }
                else {

                    if (hash_element->second.second == stack_.end()) {
                        if (hash_element->second.first != list_.end()) {
                            only_in_list(key, hash_element);
                            is_hit = true;
                        }
                    }
                    else {
                        in_stack(key, slow_get_page, hash_element, &is_hit);
                    }
                }
                return is_hit;
            }
        
        private:

            bool full_list (unsigned len_list) const {
                return (len_list == hirsize);
            }

            bool full_Lir(unsigned len_stack) const {
                return (len_stack >= lirsize);
            }

            void delete_last_stack_elem() {
                ListIt keyptr_list = hash_[stack_.back().second].first;
                hash_.insert_or_assign(stack_.back().second, std::make_pair(keyptr_list, stack_.end()));
                stack_.pop_back();
            }

            void free_stack_to_Lir() {
                while ((stack_.back()).first != LIR) {
                    delete_last_stack_elem();
                }
            }

            template <typename Func> void new_elem_full_cache(KeyT key, Func slow_get_page) {

                stack_.push_front({Resident_HIR, key});
                list_.push_front(key);

                HashIt delete_element_list = hash_.find(list_.back());
                StackIt change_element_in_stack = delete_element_list->second.second;
                if (change_element_in_stack != stack_.end()) {
                    change_element_in_stack->first = Non_resident_HIR;
                }

                hash_.insert_or_assign(delete_element_list->first, std::make_pair(list_.end(), change_element_in_stack));
                list_.pop_back();

                hash_.insert_or_assign(key, std::make_pair(list_.begin(), stack_.begin()));

                cache_.erase(delete_element_list->first);
                cache_.insert(slow_get_page(key));
            }

            void only_in_list(KeyT key, HashIt hash_element) {
                stack_.push_front({Resident_HIR, key});
                ListIt keyptr_list = hash_element->second.first;

                if (list_.begin() != keyptr_list) {
                    list_.splice(list_.begin(), list_, keyptr_list, std::next(keyptr_list));
                }

                hash_.insert_or_assign(key, std::make_pair(list_.begin(), stack_.begin()));
            }

            void element_is_LIR(StackIt keyptr_stack) {

                if (stack_.begin() != keyptr_stack) {
                    stack_.splice(stack_.begin(), stack_, keyptr_stack, std::next(keyptr_stack));
                }

                free_stack_to_Lir();
            }

            void resident_in_stack(KeyT key, StackIt keyptr_stack) {
                
                if (stack_.begin() != keyptr_stack) {
                    stack_.splice(stack_.begin(), stack_, keyptr_stack, std::next(keyptr_stack));
                }
                keyptr_stack->first = LIR;

                list_.erase(hash_[key].first);
                hash_.insert_or_assign(key, std::make_pair(list_.end(), stack_.begin()));
                list_.push_front(stack_.back().second);
                hash_.insert_or_assign((stack_.back().second), std::make_pair(list_.begin(), stack_.end()));

                delete_last_stack_elem();
                free_stack_to_Lir();
            }

            template <typename Func> void non_resident_in_stack(KeyT key, StackIt keyptr_stack, Func slow_get_page) {

                if (stack_.begin() != keyptr_stack) {
                    stack_.splice(stack_.begin(), stack_, keyptr_stack, std::next(keyptr_stack));
                }
                keyptr_stack->first = LIR;

                StackIt elem_to_non_res = hash_[list_.back()].second;
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

            template <typename Func> void first_entery(KeyT key, Func slow_get_page) {
                
                if (full_list(list_.size())) {
                        new_elem_full_cache(key, slow_get_page);
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

            template <typename Func> void in_stack(KeyT key, Func slow_get_page, HashIt hash_element, bool *is_hit) {

                StackIt keyptr_stack = hash_element->second.second;

                if (keyptr_stack->first == LIR) {
                    element_is_LIR(keyptr_stack);
                    *is_hit = true;
                }
                else if (keyptr_stack->first == Resident_HIR) {
                    
                    resident_in_stack(key, keyptr_stack);
                    *is_hit = true;
                }
                else {
                    non_resident_in_stack(key, keyptr_stack, slow_get_page);
                }
            }


//---------------------------------------debug functions---------------------------------------//

            void print_cache() const {
                std::cout << "This is cache:" << std::endl;
                for(auto i = cache_.begin(); i != cache_.end(); ++i) {
                    std::cout << *i << std::endl;
                }
                std::cout << "/-------------------/" << std::endl;
                std::cout << std::endl;
            }

            void print_hash() const {
                std::cout << "This is hash:" << std::endl;
                for(HashIt i = hash_.begin(); i != hash_.end(); ++i) {
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
                for(ListIt i = list_.begin(); i != list_.end(); ++i) {
                    std::cout << *i << std::endl;
                }
                std::cout << "/-------------------/" << std::endl;
                std::cout << std::endl;
            }

            void print_stack() const {
                std::cout << "This is stack:" << std::endl;
                for(StackIt i = stack_.begin(); i != stack_.end(); ++i) {
                    std::cout << "Type:" << i->first << ", element:" << i->second << std::endl;
                }
                std::cout << "/-------------------/" << std::endl;
                std::cout << std::endl;
            }
//---------------------------------------------------------------------------------------------//

    };
}