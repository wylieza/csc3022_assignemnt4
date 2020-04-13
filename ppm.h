#ifndef PPM
#define PPM

//Includes
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib> //For exit(0)
#include <math.h> //For pow

namespace WYLJUS002{

    struct feature{
        int dimension;
        int *location;

        double get_distance(struct feature other){
            if (dimension != other.dimension){
                return -1;
            }

            double accumulator = 0;
            for (int i = 0; i < dimension; i++){
                accumulator += pow(*(location+i) - *(other.location+i), 2);
            }

            return sqrt(accumulator);
        }

    };

    class ppm{

        //Globals
        int width;
        int height;
        std::string file_name;
        std::string relative_path;
        unsigned char *image_data;
        struct feature image_feature;

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
        struct feature get_image_feature(); //Return the image feature
        double get_distance(struct feature other);

    };

}


#endif