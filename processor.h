#ifndef PROCESSOR
#define PROCESSOR


//Includes
#include "ppm.h"
#include <memory>
#include <random>
#include <algorithm>


namespace WYLJUS002{

    /*
    struct image{
        WYLJUS002::ppm img;
        int closest_mean; //This is also the cluster id
        
    };
    */

    class processor{

        //Globals
        std::string relative_path;
        std::vector<std::shared_ptr<ppm>> images;
        std::vector<struct feature> means;
        int num_means;
        int bin_size;
        bool greyscale;

        
        //Functions
        public:
        processor();
        ~processor();
        processor(const std::string path, const int k, const int bsize, const bool greyscale);

        private:
        void load_images();
        std::vector<std::string> get_file_names();

        public:
        void load_images(const std::string path);
        void compute_clusters();
        void update_centroid_locations();
        friend std::ostream& operator<<(std::ostream& os, const processor& proc);
        void file_results(const std::string& of_name);

        private:
        void init_means();


    };

}

#endif