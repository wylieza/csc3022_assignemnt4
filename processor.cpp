#include "processor.h"

namespace WYLJUS002{

    processor::processor(){
        num_means = -1;
    }

    processor::~processor(){
        
    }

    processor::processor(const std::string path, const int k, const int bsize){
        processor();
        num_means = k;
        relative_path = path;
        bin_size = bsize;
        load_images();


    }

    void processor::load_images(){
        if(relative_path.empty()){
            std::cout << "No image base has been specified!\n";
            exit(0);
        }

        std::vector<std::string> file_names = get_file_names();

        for(int i = 0; i < file_names.size(); i++){

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
        if(images.empty()){
            std::cout << "Cannot compute clusters on empty image set\n";
            exit(0);
        }

        generate_image_features();
        init_means();

        std::vector<int> previous_cm(images.size());
        for(int i = 0; i < previous_cm.size(); i++)
            previous_cm[i] = -1;

        //not? recursive code:
        bool done = false;
        int closest_mean;
        double last_dist;
        int iteration = 0; //debug

        while(!done){

            //debug
            iteration++;
            if(iteration % 10 == 0){
                std::cout << "\n\nPrinting means \n";
                for(int k = 0; k < num_means; k++){
                    means[k].print();
                }
            }

            //There is something wrong with the mean recalculation.... the initial location of all images is 1024 as it should be for num bins =1


            for (int i = 0; i < images.size(); i++){
                //std::cout << "Images: " << i << std::endl;
                closest_mean = 0;
                last_dist = images[i]->get_distance(means[0]);
                for (int k = 1; k < num_means; k++){
                    //std::cout << "Dist: " << images[i]->get_distance(means[k]) << std::endl;
                    if(last_dist > images[i]->get_distance(means[k])){
                        last_dist = images[i]->get_distance(means[k]);
                        closest_mean = k;
                    }                    
                }
                images[i]->closest_mean = closest_mean;
            }

            //std::cout << "Calculated closest means\n";

            done = true;
            for (int i = 0; i < images.size(); i++){
                done = images[i]->closest_mean == previous_cm[i] ? done : false;

                //debug
                if (!done){
                    std::cout << "Image: " << images[i]->get_name() << " has a new closest mean!\n"
                    "Previous mean: " << previous_cm[i] << " New mean: " << images[i]->closest_mean << std::endl;
                }

                previous_cm[i] = images[i]->closest_mean;                
            }
            //If 'done' is true at this point, there was no change in mean locations

            if(!done){ //Recalc the means
                std::cout << "Recalculating the means\n";
                for(int k = 0; k < num_means; k ++){
                    std::vector<double> new_mean(num_bins(bin_size)); //Do I need to init all values to zero?
                    int divisor = 0;
                    for(int i = 0; i < images.size(); i++){
                        if(images[i]->closest_mean == k){
                            divisor++;
                            for(int b = 0; b < num_bins(bin_size); b++){
                                new_mean[b] += images[i]->image_feature.location[b];
                            }
                        }
                    }
                    if(divisor != 0){
                        for(int b = 0; b < num_bins(bin_size); b++){
                            new_mean[b] = new_mean[b]/(double)divisor;
                        }
                        means[k].location = new_mean;
                    }
                }
            }
        
        }

        //print out the clusters
        for(int k = 0; k < num_means; k++){
            std::cout << "Cluster: " << k << std::endl;
            for (int i = 0; i < images.size(); i++){
                if(images[i]->closest_mean == k)
                    std::cout << images[i]->get_name() << std::endl;
            }
        }

        std::cout << "\n\nAll images: \n";
        for (int i = 0; i < images.size(); i++){
            std::cout << images[i]->get_name() << " closest mean: " << images[i]->closest_mean << std::endl;
        }


    }

    void processor::generate_image_features(){
        for(int i = 0; i < images.size(); i++){
            images[i]->generate_image_feature(bin_size);
            std::cout << "Loaded image: " << images[i]->get_name() << " with location:"; images[i]->image_feature.print();//DEBUG
        }
    }

    void processor::init_means(){
        int dim = images[0]->image_feature.location.size();
        std::cout << dim << " dimensional\n"; //debug
        std::vector<double> location(dim);

        for (int i = 0; i < num_means; i++){
            for (int j = 0; j < dim; j++){
                location[j] = i*(255/(double)num_means);
            }

            means.push_back({location});
            means[i].print(); //debug
        }
    }


}