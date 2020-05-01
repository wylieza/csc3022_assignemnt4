//Inlucdes
#include <sstream> //String streaming and strings
#include <iostream> //cout

#include "ppm.h"
#include "processor.h"

//Global Variables


//Functions
int char_to_int(const char num[]){
    std::stringstream str_num(num);
    int result;

    str_num >> result;

    return result;
}


//main

int main(int argc, char *argv[]){

    std::string dataset_name;
    
    //Get the dataset name
    if(argc > 1)
        dataset_name = std::string(argv[1]);
    else{
        std::cout << "You must specify a data set!\n";
        exit(0);
    }

    std::cout << "Data set is: " << dataset_name << std::endl;

    std::string output_file;
    int num_clusters = 10;
    int bin_size = 1;

    for(int i = 2; i < argc-1; i++){
        if (*(argv[i] + 1) == 'o')
            output_file = std::string(argv[i+1]);
        
        if (*(argv[i] + 1) == 'k')
            num_clusters = char_to_int(argv[i+1]);

        if (std::string(argv[i] + 1) == "bin")
            bin_size = char_to_int(argv[i+1]);
    }

    if(!output_file.empty()){
        std::cout << "Output file: " << output_file << std::endl;
    }else{
        std::cout << "Output file unspecified, writting to std_out\n";
    }

    //Feedback to user on the configuration
    std::cout << "Clusters: " << num_clusters << std::endl;
    std::cout << "Bin size: " << bin_size << std::endl;


    //Initiate image processing
    //WYLJUS002::ppm image("eight_10.ppm", dataset_name);
    WYLJUS002::processor proc(dataset_name, num_clusters, bin_size);
    std::cout << "Computing clusters...\n";
    proc.compute_clusters();
    std::cout << proc;


}