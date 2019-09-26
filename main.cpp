#include "hash.h"

int main(){

	//generatefiles1();
	//generatefiles2();

    std::string t = "";
    std::cout << "Do you want to read text from file or to input the text by hand? \n";
	std::cout << "If you want to read ext from file input 1, if you want to write text by hand input 0 \n";
	std::cin >> t;
	while (t != "0" && t != "1")
	{
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Input 1 to read from file, iput 0 to input the text by hand. \n";
		std::cin >> t;
	}
    if(t == "0"){
		hash current;
        inputbyhand( current );
		makelongerifneeded( current );
		auto start = std::chrono::high_resolution_clock::now();
		hashalgorithm( current );
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> dur = end - start;
		std::cout << "Hash algorithm took " << dur.count() << " sec.\n";
		printhash( current );
    }
    else{
        std::string filename;
		std::cout<<"If you want 1 hash for whole file input 0, if you want different hash for every line in the text file input 1 \n";
		std::cin>>t;
		if(t=="0"){
			std::cout<<"Input the file name. Example: text.txt\n";
			std::cin>>filename;
			hash current;
        	try{
           		inputfromfile( current, filename);   
        	}
        	catch (std::exception &e)
			{
				std::cout << "Unable to open file. \n";
				return 0;
			}
			makelongerifneeded( current );
			auto start = std::chrono::high_resolution_clock::now();
			hashalgorithm( current );
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> dur = end - start;
			std::cout << "Hash algorithm took " << dur.count() << " sec.\n";
			printhash( current );

		}
		else{
			std::cout<<"Input the file name. Example: text.txt\n";
			std::cin>>filename;
			std::vector<hash> multiplehash = {};
        	try{
           		inputlinebyline( multiplehash, filename );
        	}
        	catch (std::exception &e)
			{
				std::cout << "Unable to open file. \n";
				return 0;
			}

			std::chrono::duration<double> timesum;
			for( unsigned int i=0; i<multiplehash.size(); i++){
				std::string copy = multiplehash[i].getinput();
				makelongerifneeded( multiplehash[i] );
				auto start = std::chrono::high_resolution_clock::now();
				hashalgorithm( multiplehash[i] );
				auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> dur = end - start;
				timesum = timesum + dur;
			}
			printhash( multiplehash );
			std::cout << "Hash algorithm took " << timesum.count() << " sec.\n";
		}
    }

	return 0;
}