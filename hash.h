#ifndef HASH_H
#define HASH_H

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>

class hash{
    private:
    std::string input_;
    std::string fixedinput_;
    std::string output_;

    public:
    std::vector<int> values = {};
    hash(){
        input_ = "";
        fixedinput_ = "";
        output_ = "";
    }
    hash( std::string input ){
        input_ = input;
        fixedinput_ = "";
        output_ = "";
    }
    void setinput( std::string &input ) { input_ = input; }
    void setfixedinput( std::string &fixed) { fixedinput_ = fixed; }
    void setoutput( std::string &output) { output_ = output; }
    std::string const getinput () { return input_; }
    std::string const getfixedinput () { return fixedinput_; }
    std::string const getoutput () { return output_; }
    ~hash(){};
};

void inputfromfile( hash &current, std::string &filename );
void inputbyhand( hash &current );
void inputlinebyline( std::vector<hash> &multiplehash, std::string &filename ); 
void makelongerifneeded( hash &current );
void hashalgorithm( hash &current );
std::string inttohex( unsigned long long int &i );
std::string hashshuffle( std::string &unshuffledhash, hash &current );
void printhash ( hash &current ); 

#endif