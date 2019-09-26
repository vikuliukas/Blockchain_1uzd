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
    std::string t;
    if(f.fail()){
        throw std::exception();
    }
    while(!f.eof()){
        std::getline(f,t);
        multiplehash.push_back(t);
    }
    for(unsigned int i = 0; i < multiplehash.size()-1; i++){
        t = multiplehash[i].getinput();
        if(t.length() > 0){
            t.erase(t.length()-1);
        }
        multiplehash[i].setinput(t);
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
    unsigned long long int sum[5] = {1};
    int speperating = 1;
    int i = 1;
    for(char32_t &character : input32) {
        if(speperating == 1){
            sum[0] = ( i + i ) * sum[0] + character;
        }
        if(speperating == 2){
            sum[1] = ( i + i ) * sum[1] + character;
        }
        if(speperating == 3){
            sum[2] = ( i + i ) * sum[2] + character;
        }
        if(speperating == 4){
            sum[3] = ( i + i ) * sum[3] + character;
            speperating = 0;
        }
        speperating ++;
        i++;
    }

    std::string hex [5] = {""};
    for(int i = 0; i < 4; i++){
        hex[i] = inttohex(sum[i]);
        while(hex[i].length()<16){
            hex[i] += "0";
        }
    }
    std::string unshuffledhash = hex[0] + hex[1] + hex[2] + hex[3];
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

void printhash ( std::vector<hash> &multiplehash ){
    std::ofstream r ("rezultatas.txt");
    for(unsigned int i = 0; i < multiplehash.size(); i++){
        r << multiplehash[i].getoutput() << "\n";
    }
    r.close();
}

void generatefiles1(){
    std::ofstream gf("gen1.txt");
    std::string abc = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
    std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 51);
    for(int i = 0; i < 1000000; i++){
        for(int j = 0; j < 5; j++){
            gf << abc[uni(rng)];
        }
        gf << "\r\n";
    }
    gf.close();
}

void generatefiles2(){
    std::ofstream gf("gen2.txt");
    std::string abc = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
    std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 51);
    std::uniform_int_distribution<int> uni1(0, 4);
    std::string t = "";
    for(int i = 0; i < 500000; i++){
        t = "";
        for(int j = 0; j < 5; j++){
            t += abc[uni(rng)];
        }
        gf << t;
        gf << "\r\n";
        t[uni1(rng)] = '!';
        gf<< t ;
        gf << "\r\n";
    }
    gf.close();
}
