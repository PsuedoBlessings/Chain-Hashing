#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>    

#include "Hash.h"

using std::vector;
using std::pair;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K,V> { // derived from Hash
private:
    // Needs a table and a size.
    // Table should be a vector of std::pairs for lazy deletion

    vector<pair<K, V>> table;
    int currentSize;
    int a;

public:
    ProbingHash(int n = 11) // n represents bucket size
    {
        this->findNextPrime(n); // makes sure it's a prime

        this->table.reserve(n); // this is the bucket size

        do
        {
            this->a = rand()%10;
        } while(a == n); // pervents a loop within our insert function
        
        
        this->a = this->findNextPrime(a);
    }

    ~ProbingHash() {
        this->clear();
    }

    int size() {
        return this->currentSize;
    }

    V operator[](const K& key) {
        int index = (hash(key) + this->probing()) % bucket_count();
        std::pair<K, V> pair = this->table[index];
        return pair.second;
    }

    bool insert(const std::pair<K, V>& pair) 
    {
       int index = (hash(pair.first) + this->probing()) % bucket_count();
       if(this->table[index] == pair)
        //this->table.assign(index, pair); if you want insert to be able to update
        return false;

        this->table[index] = pair;
        currentSize++;
        if(this->load_factor() < 0.75)
        rehash();

        return true;
    }

    void erase(const K& key) {
        int index = (hash(key) + this->probing()) % bucket_count();
        this->table.erase(this->table.begin()+index);
    }

    void clear() {
        table.clear();
    }

    int bucket_count() {
        return table.size();
    }

    float load_factor() {
            float loadfactor = this->size()/this->bucket_count();
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

    int hash(const K& key) 
    {
        return key % this->bucket_count();       
    }

    int probing()
    {
        return (this->a*this->currentSize)+0;
    }
    
    void rehash()
    {
        this->table.resize(ceil(this->currentSize/this->load_factor()));
        
        while(a == table.size()); // pervents a loop within our insert function
        {
            a = rand()%10;
            a = this->findNextPrime(a);
        }
    }
};

#endif //__PROBING_HASH_H
