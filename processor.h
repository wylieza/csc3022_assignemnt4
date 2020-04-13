#ifndef PROCESSOR
#define PROCESSOR


//Includes
#include "ppm.h"


namespace WYLJUS002{

    struct image{
        WYLJUS002::ppm img;
        int closest_mean; //This is also the cluster id
        
    };

    class processor{

        //Globals
        std::string relative_path;
        int num_images;
        struct image *images;
        int num_means;
        struct WYLJUS002::feature *means;

        
        //Functions
        public:
        processor();
        ~processor();
        processor(const std::string path, const int k);

        void load_images();
        void load_images(const std::string path);

        void compute_clusters();





    };

}

#endif