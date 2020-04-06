//Inlucdes
#include <sstream> //String streaming and strings
#include <iostream> //cout

//Global Variables


//Functions


//main

namespace WYLJUS002{

    int main(int argc, char *argv[]){

        std::string data_set;
        
        if(argc > 1)
        data_set = std::string(argv[1]);

        std::cout << "Data set is: " << data_set << std::endl;

    }

}