#include "hash.h"

void inputfromfile( hash&current, std::string &filename ){
    std::ifstream f(filename);
    if(f.fail()){
        throw std::exception();
    }
    std::string content( (std::istreambuf_iterator<char>(f) ),(std::istreambuf_iterator<char>()    ) );
    current.setinput(content);
    f.close();
}

void inputlinebyline( std::vector<hash> &multiplehash, std::string &filename ){
    std::ifstream f(filename);
    std::string t = "";
    if(f.fail()){
        throw std::exception();
    }
    while(!f.eof()){
        std::getline(f,t);
        t.erase(t.length()-1);
        multiplehash.push_back(t);
    }
    f.close();
}

void inputbyhand( hash&current ){
    std::cout << "Input wanted text here: \n";
    std::string content ="";
    std::cin >> content;
    current.setinput(content);
}

void makelongerifneeded( hash &current ){
    if(current.getinput().length() < 60){
        std::string filler (60, '!');
        std::string copy = current.getinput();
        filler.replace(0,copy.length(),copy);
        current.setfixedinput(filler);
    }
    else{
        std::string copy = current.getinput();
        current.setfixedinput(copy);
    }
}

void hashalgorithm( hash&current ){
    std::string copy = current.getfixedinput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);
    unsigned long long int sum1 = 1;
    unsigned long long int sum2 = 1;
    unsigned long long int sum3 = 1;
    unsigned long long int sum4 = 1;
    int speperating = 1;
    int i = 1;
    for(char32_t &character : input32) {
        if(speperating == 1){
            sum1 = i * sum1 + character;
        }
        if(speperating == 2){
            sum2 = i * sum2 + character;
        }
        if(speperating == 3){
            sum3 = i * sum3 + character;
        }
        if(speperating == 4){
            sum4 = i * sum4 + character;
            speperating = 0;
        }
        speperating ++;
        i++;
    }

    std::string hex1 = inttohex (sum1);
    std::string hex2 = inttohex (sum2);
    std::string hex3 = inttohex (sum3);
    std::string hex4 = inttohex (sum4);
    std::string unshuffledhash = hex1 + hex2 + hex3 + hex4;
    std::string shuffledhash = hashshuffle( unshuffledhash, current );
    current.setoutput(shuffledhash);
}

std::string inttohex( unsigned long long int &i ){
    std::stringstream stream;
    stream << std::hex << i;
    std::string result( stream.str() );
    return result;
}

std::string hashshuffle( std::string &unshuffledhash, hash &current ){

    std::string copy = current.getinput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);
    int i = 1;
    int sum = 0;
    for(char32_t &character : input32) {
        sum = sum + character * i;
        i++;
    }
    unsigned seed = sum;
    std::shuffle(unshuffledhash.begin(), unshuffledhash.end(),std::default_random_engine(seed));
    return unshuffledhash;
}

void printhash ( hash &current ){
    std::cout<<current.getoutput()<<"\n";
}
