#include "ppm.h"

namespace WYLJUS002{
    

    ppm::ppm(){
        greyscale = false;
        feature_computed = false;
        closest_mean = 0;
    }

    ppm::~ppm(){        
    }

    ppm::ppm(const std::string file_name, const std::string path){
        ppm();
        this->file_name = file_name;
        this->relative_path = path;
        load_image();
        to_greyscale();
    }

    /* COPY consturctor
    ppm::ppm(const ppm &other){
        std::cout << "Copy constructor called\n";
        std::cout << other.file_name << std::endl;

    }
    */

   int num_bins(int bin_size){
        return (int) ceil(256/(double)bin_size);
    }

    void ppm::load_image(){
        if(file_name.empty()){
            std::cout << "Image file name not specified\n";
            exit(0);
        }
        //Load image file
        std::ifstream in_file(("./" + relative_path + "/" + file_name).c_str(), std::ios::binary);

        if(!in_file.is_open()){
            std::cout << "An error occured while attempting to open file\n"
            "File location: >" << relative_path + file_name << "<\n";
            exit(0);
        }

        std::string str_buff;

        bool read_header = true;
        int param_line_num = 0;
        while(read_header){
            if(std::getline(in_file, str_buff)){
                if(str_buff.find('#') != str_buff.npos){
                    continue;
                }else{
                    switch(param_line_num){
                        case 0:
                            if (str_buff.find("P6") != str_buff.npos){
                                param_line_num++;
                                break;
                            }else{
                                std::cout << "Incorectly formatted .ppm file!\n";
                                exit(0);
                            }
                        case 1:
                            std::stringstream(str_buff.substr(0, str_buff.find(" "))) >> height;
                            std::stringstream(str_buff.substr(str_buff.find(" ") + 1, str_buff.length())) >> width;
                            param_line_num ++;
                            break;
                        case 2:
                            read_header = false;
                            break;

                        default:
                            std::cout << "Default case reached, program will now terminate\n";   
                            exit(0);                 
                    }
                }
            }
        }

        //Begin reading the raw image data
        int raw_data_beg = in_file.tellg();
        in_file.seekg(0, in_file.end);
        int raw_data_end = in_file.tellg();
        in_file.seekg(raw_data_beg);

        //Check raw data size matches image size
        if(width*height*3 != raw_data_end-raw_data_beg){
            std::cout << "Bytes of image data expected (" << width*height*3 << ") did not match the data"
                            "present in file (" << raw_data_end-raw_data_beg << ")\n";
            exit(0);
        }

        //Read the raw data
        image_data = std::vector<unsigned char>(width*height*3);
        char* char_buff = new char[width*height*3];
        in_file.read(char_buff, width*height*3);

        for(int i = 0; i < width*height*3; i++){
            image_data[i] = (unsigned char) char_buff[i];
        }

        delete[] char_buff;


    }

    void ppm::load_image(const std::string file_name, const std::string path = "./"){
        this->file_name = file_name;
        this->relative_path = path;
        load_image();
    }

    void ppm::to_greyscale(){
        int wh_product = width*height;
        if(wh_product*3 %3 != 0 || wh_product == 0){
            std::cout << "Image data must be exactly divisible by 3\n";
            exit(0);
        }

        std::vector<unsigned char> gs_image_data(wh_product);
        
        int r, g, b;
        for(int i = 0; i < wh_product; i++){
            r = image_data[i*3];
            g = image_data[i*3 + 1];
            b = image_data[i*3 + 2];
            gs_image_data[i] = 0.21*r + 0.72*g + 0.07*b;
        }
        
        image_data = gs_image_data;
        greyscale = true;

    }

    void ppm::generate_image_feature(const int &bin_size){
        if(!greyscale){
            std::cout << "Cannot find a loaded image in greyscale format\n";
            exit(0);
        }

        image_feature.location = std::vector<double>(num_bins(bin_size));

        int wh_product = width*height;
        for (int px = 0; px < wh_product; px++){
            for (int bin = 0; bin < num_bins(bin_size); bin++){
                if(bin*bin_size <= image_data[px] && image_data[px] < (bin+1)*bin_size){
                    image_feature.location[bin]++;
                    break;
                }
            }
        }
    }

    double ppm::get_distance(struct feature other){
        return image_feature.get_distance(other);
    }

    std::string ppm::get_name(){
        return file_name;
    }


}