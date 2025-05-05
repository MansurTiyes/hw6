#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <unordered_set> // for letterDigitToNumber function
#include <cctype> // for tolower() used in letterDigitToNumber
#include <vector> 


typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        HASH_INDEX_T h_k = 0;

        std::vector<unsigned long long> w = all_w(k);

        for(size_t i = 0; i < 5; i++) {
            h_k += (rValues[i] * w[5-i-1]);
        }

        return h_k;


    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35


    // 1) keep unonordered map for numbers 
    // 2) if letter in map of numbers -> return (letter%48)+26
    // 3) else return letter.lowercased % 97
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        std::unordered_set<char> numbers = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        if (numbers.find(letter) != numbers.end()) {
            HASH_INDEX_T result = (letter%48) + 26;
            return result;
        }
        HASH_INDEX_T non_letter_result = tolower(letter)%97;
        return non_letter_result;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }

    // calculating w_i
    unsigned long long calculate_w(const std::string& str) const {
        std::vector<HASH_INDEX_T> a;
        
        for(size_t i = 0; i < (6-str.length()); i++) {
            a.push_back(0);
        }

        for (size_t i = 0; i < str.length(); i++) {
            a.push_back(letterDigitToNumber(str[i]));
        }

        unsigned long long result = 0;
        unsigned long long base = 1;

        for (int i = 5; i >= 0; --i) {
            result += base * static_cast<unsigned long long>(a[i]);
            base *= 36;
        }

        return result;
    }

    // calculating array of w
    std::vector<unsigned long long> all_w(const std::string& k) const {
        std::vector<unsigned long long> w;

        size_t size_of_w = (k.length() + 5) / 6;

        int end = k.length()-1;
        int start = end-5;
        if (start < 0) {
            start = 0;
        }

        for (size_t i = 0; i < size_of_w; i++) {
            w.push_back(calculate_w(k.substr(start,end-start+1)));
            end = start-1;
            start = end-5;
            if (start < 0) {
                start = 0;
            }
        }

        for (size_t i = 0; i < (5 - size_of_w); i++) {
            w.push_back(0);
        }

        return w;
    }
};

#endif
