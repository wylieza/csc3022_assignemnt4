#include "ppm.h"

namespace WYLJUS002{

    ppm::ppm(){
        greyscale = false;
        feature_computed = false;
    }

    ppm::~ppm(){
        delete[] image_data;
    }

    ppm::ppm(const std::string file_name, const std::string path){
        ppm();
        this->file_name = file_name;
        this->relative_path = path;
        load_image();
        to_greyscale();
    }

    void ppm::load_image(){
        if(file_name.empty()){
            std::cout << "Image file name not specified\n";
            exit(0);
        }else{ //Load image file
            std::ifstream in_file(("./" + relative_path + "/" + file_name).c_str(), std::ios::binary);

            if(!in_file.is_open()){
                std::cout << "An error occured while attempting to open file\n"
                "File location: >" << relative_path + file_name << "<\n";
                exit(0);
            }

            std::string buffer;

            bool read_header = true;
            int param_line_num = 0;
            while(read_header){
                if(std::getline(in_file, buffer)){
                    if(buffer.find('#') != buffer.npos){
                        continue;
                    }else{
                        switch(param_line_num){
                            case 0:
                                if (buffer.find("P6") != buffer.npos)
                                    param_line_num++;
                                break;
                            case 1:
                                std::stringstream(buffer.substr(0, buffer.find(" "))) >> height;
                                std::stringstream(buffer.substr(buffer.find(" ") + 1, buffer.length())) >> width;
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
            //DEBUG - ensure correct data is read
            std::cout << "Image height >" << height << "< width >" << width << "<\n";
            //DEBUG

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
            std::cout << "Reading data\n";
            image_data = new unsigned char[width*height*3];
            in_file.read((char*) image_data, width*height*3);


        }

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

        unsigned char *gs_image_data = new unsigned char[wh_product];
        
        int r, g, b;
        for(int i = 0; i < wh_product; i++){
            r = *(image_data + i*3);
            g = *(image_data + i*3 + 1);
            b = *(image_data + i*3 + 2);
            *(gs_image_data+i) = 0.21*r + 0.72*g + 0.07*b;
        }
        delete[] image_data;
        image_data = gs_image_data;
        greyscale = true;

    }

    void ppm::generate_image_feature(const int bin_size){
        if(!greyscale){
            std::cout << "Cannot find a loaded image in greyscale format\n";
            exit(0);
        }

        image_feature = new int[bin_size];
        double bin_width = (double)256/(double)bin_size;

        int wh_product = width*height;
        for (int px = 0; px < wh_product; px++){
            for (int bin = 0; bin < bin_size; bin++){
                if(bin*bin_width <= *(image_data + px) < (bin+1)*bin_width){
                    (*(image_feature + bin))++;
                    break;
                }
            }
        }
    }

    


}