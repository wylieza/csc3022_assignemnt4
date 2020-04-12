#ifndef PPM
#define PPM

//Includes
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib> //For exit(0)

namespace WYLJUS002{

    class ppm{

        //Globals

        int width;
        int height;
        std::string file_name;
        std::string relative_path;
        unsigned char *image_data;
        int *image_feature;

        //Flags
        bool greyscale;
        bool feature_computed;


        //Functions
        public:
        ppm(); //Default constructor
        ~ppm(); //Default destructor
        ppm(const std::string file_name, const std::string path); //Constructor with file name specified
        void load_image(); //Load from file name already defined
        void load_image(const std::string file_name, const std::string path); //Load image data from file
        void to_greyscale(); //Convert image to a greyscale image
        void generate_image_feature(const int bin_size); //Generate the image feature data
        int* get_image_feature(); //Return the image feature

    };

}


#endif