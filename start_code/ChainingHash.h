/*
 *  Separate chaining hashtable
 */

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

// Standard library includes
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <crypt.h>

// Custom project includes
#include "Hash.h"

// Namespaces to include
using std::vector;
using std::list;
using std::pair;

using std::cout;
using std::endl;

//
// Separate chaining based hash table - derived from Hash
//
template<typename K, typename V>
class ChainingHash : public Hash<K,V> 
{
    private:
    vector<list<pair<K, V>>> table;

    int currentSize;

    public:
        ChainingHash(int n = 11) // constructor | n represents the amount of buckets
        {
            if(!isPrime(n)) // making sure the load factor is always 0.75
            {
                n = findNextPrime(n);
            }

            this->table.reserve(n); // creating the amount of space needed
        }

        ~ChainingHash() {
            this->clear();
        }

        int size() 
        {
            return this->currentSize;
        }

        V operator[](const K& key) {
            auto& List = table[this->hash(key)];
        }

        bool insert(const std::pair<K, V>& pair) 
        {
            auto & List = table[this->hash(pair.first)];
            if(std::find(std::begin(List), std::end(List), pair) != std::end(List));
            return false;
            List.push_back(pair);
            ++this->currentSize;

            if(this->load_factor() < 0.75) // checking if current load_factor is too high
            rehash();

            return true;
        }

        void erase(const K& key) 
        {
            auto & List = table[this->hash(key)];
            auto itr = std::find(List.begin(), List.end(), key);

            if(itr == List.end())
            return;

            List.erase(itr);
            --currentSize;
            return;
        }

        void clear() 
        {
            for(int i = 0; i < table.size(); i++)
            table[i].clear();
        }

        int bucket_count() {
            return this->table.size() + 1;
        }

        float load_factor() {
            float loadfactor = size()/bucket_count();
            return loadfactor;
        }

    private:

        int findNextPrime(int n)
        {
            while (!isPrime(n))
            {
                n++;
            }
            return n;
        }

        int isPrime(int n)
        {
            for (int i = 2; i <= sqrt(n); i++)
            {
                if (n % i == 0)
                {
                    return false;
                }
            }

            return true;
        }

        int hash(const K& key) {

            return key % bucket_count();       
        }

        void rehash()
        {
            this->table.resize(ceil(this->currentSize/this->load_factor()));
        }
};

#endif //__CHAINING_HASH_H
