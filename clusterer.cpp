//Inlucdes
#include <sstream> //String streaming and strings
#include <iostream> //cout

#include "ppm.h"
#include "processor.h"

//Functions
int char_to_int(const char num[]){
    std::stringstream str_num(num);
    int result;

    str_num >> result;

    return result;
}

int main(int argc, char *argv[]){

    std::string dataset_name;
    
    //Get the dataset name
    if(argc > 1)
        dataset_name = std::string(argv[1]);
    else{
        std::cout << "You must specify a data set!\n";
        exit(0);
    }

    std::string output_file;
    int num_clusters = 10;
    int bin_size = 1;
    bool greyscale = true;

    for(int i = 2; i < argc; i++){
        if(argc > i+1){
            if (*(argv[i] + 1) == 'o')
                output_file = std::string(argv[i+1]);
            
            if (*(argv[i] + 1) == 'k')
                num_clusters = char_to_int(argv[i+1]);

            if (std::string(argv[i] + 1) == "bin")
                bin_size = char_to_int(argv[i+1]);
        }

        if (std::string(argv[i] + 1) == "color")
            greyscale = false;
    }

    //Feedback to user on the configuration
    std::cout << "Configuration\n";
    std::cout << "Data set is: " << dataset_name << std::endl;
    std::cout << "Clusters: " << num_clusters << std::endl;
    std::cout << "Bin size: " << bin_size << std::endl;
    std::cout << "Images are processed in " << (greyscale ? "greyscale":"full color") << std::endl;
    std::cout << "\n";


    //Begin image processing
    WYLJUS002::processor proc(dataset_name, num_clusters, bin_size, greyscale);
    std::cout << "Computing clusters\n";
    proc.compute_clusters();


    if(!output_file.empty()){
        std::cout << "Writing to file: " << output_file << std::endl;
        proc.file_results(output_file);
    }else{
        std::cout << "Writing to std_out\n\n";
        std::cout << proc;
    }
    


}