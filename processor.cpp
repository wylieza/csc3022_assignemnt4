#include "processor.h"

namespace WYLJUS002{

    processor::processor(){
        num_images = -1;
        num_means = -1;
    }

    processor::~processor(){
        
    }

    processor::processor(const std::string path, const int k){
        processor();
        num_means = k;
        relative_path = path;
        load_images();


    }

    void processor::load_images(){
        if(relative_path.empty()){
            std::cout << "No image base has been specified!\n";
            exit(0);
        }

        std::vector<std::string> file_names = get_file_names();

        for(int i = 0; i < file_names.size(); i++){
            //DEBUG
            std::cout << "Name: " << file_names[i] << std::endl;
            std::cout << "relative path: " << relative_path << std::endl;
            //DEBUG

            std::shared_ptr<ppm> img(new ppm(file_names[i], relative_path));
            images.push_back(img);
        }
    }

    std::vector<std::string> processor::get_file_names(){ //Currently only for linux
    //TODO: Remove other extensions such as .txt
        std::vector<std::string> file_names;

        std::string files_list = "files_list.tmp";
        system(("ls ./" + relative_path + " >> " + files_list).c_str());

        std::ifstream in_file(files_list.c_str());
        std::string buffer;

        if(in_file){
            while(std::getline(in_file, buffer)){                
                file_names.push_back(buffer);
            }
        }else{
            std::cout << "Error occured while reading file!";
        }

        system("rm -rf files_list.tmp");

        return file_names;
    }


    void processor::load_images(const std::string path){
        relative_path = path;
        load_images();
    }

    void processor::compute_clusters(){

    }


}