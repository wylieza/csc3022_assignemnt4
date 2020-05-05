#ifndef PPM
#define PPM

//Includes
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib> //For exit(0)
#include <math.h> //For pow
#include <vector>

namespace WYLJUS002{

    struct feature{
        std::vector<double> location;

        double get_distance(const struct feature& other){
            if (location.size() != other.location.size() || location.empty() || other.location.empty()){
                return -1;
            }

            double accumulator = 0;
            for (int i = 0; i < location.size(); i++){
                accumulator += pow(location[i] - other.location[i], 2);
            }

            return sqrt(accumulator);
        }

        void print(){
            std::cout << "[";
            int last_index = location.size()-1;
            for (int i = 0; i < last_index; i++){
                std::cout << location[i] << ", ";
            }
            std::cout << location[last_index] << "]\n";
        }

    };

    int num_bins(const int& bin_size);

    class ppm{
        //Globals
        int width;
        int height;
        std::string file_name;
        std::string relative_path;
        std::vector<unsigned char> image_data;
        public:
        struct feature image_feature;
        int closest_mean; //Also the cluster id
        private:

        //Flags
        bool greyscale;
        bool image_loaded;


        //Functions
        public:
        ppm(); //Default constructor
        ppm(const std::string& file_name, const std::string& path, const bool& greyscale); //Constructor with file name specified
        //ppm(const ppm &other); //Copy constructor
        void load_image(); //Load from file name already defined
        void load_image(const std::string& file_name, const std::string& path); //Load image data from file
        void to_greyscale(); //Convert image to a greyscale image
        void generate_image_feature(const int &bin_size); //Generate the image feature data
        double get_distance(const struct feature& other);
        std::string get_name();

    };

}


#endif