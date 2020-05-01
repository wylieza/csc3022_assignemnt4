#ifndef PROCESSOR
#define PROCESSOR


//Includes
#include "ppm.h"
#include <memory>


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

        
        //Functions
        public:
        processor();
        ~processor();
        processor(const std::string path, const int k, const int bsize);

        private:
        void load_images();
        std::vector<std::string> get_file_names();

        public:
        void load_images(const std::string path);
        void compute_clusters();
        friend std::ostream& operator<<(std::ostream& os, const processor& proc);
        void file_results(const std::string& of_name);

        private:
        void generate_image_features();
        void init_means();


    };

}

#endif