#ifndef PROCESSOR
#define PROCESSOR


//Includes
#include "ppm.h"
#include <vector>


namespace WYLJUS002{

    struct image{
        WYLJUS002::ppm img;
        int closest_mean; //This is also the cluster id
        
    };

    class processor{

        //Globals
        std::string relative_path;
        int num_images;
        std::vector<struct image> images;
        int num_means;
        struct WYLJUS002::feature *means;

        
        //Functions
        public:
        processor();
        ~processor();
        processor(const std::string path, const int k);

        private:
        void load_images();
        std::vector<std::string> get_file_names();
        public:
        void load_images(const std::string path);

        void compute_clusters();

    };

}

#endif