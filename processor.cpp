#include "processor.h"

namespace WYLJUS002{

    processor::processor(){
        num_means = -1;
    }

    processor::~processor(){
        
    }

    processor::processor(const std::string& path, const int& k, const int& bsize, const bool& gs){
        processor();
        num_means = k;
        relative_path = path;
        bin_size = bsize;
        greyscale = gs;
        load_images();


    }

    //Loads up the images (depends on 'get_file_names')
    void processor::load_images(){
        if(relative_path.empty()){
            std::cout << "No image base has been specified!\n";
            exit(0);
        }

        std::vector<std::string> file_names = get_file_names();

        for(int i = 0; i < file_names.size(); i++){

            std::shared_ptr<ppm> img(new ppm(file_names[i], relative_path, greyscale));
            images.push_back(img);

        }
    }

    //For a linux os, will find all file names of .ppm files
    std::vector<std::string> processor::get_file_names(){
        std::cout << "Importing images\n";

        std::vector<std::string> file_names;

        std::string files_list = "files_list.tmp";
        system(("ls ./" + relative_path + " >> " + files_list).c_str());

        std::ifstream in_file(files_list.c_str());
        std::string buffer;

        if(in_file){
            while(std::getline(in_file, buffer)){                
                if(buffer.find(".ppm") != std::string::npos){
                    file_names.push_back(buffer);
                }else{
                    std::cout << "[WARN] File '" << buffer << "' does not have the extension '.ppm' and was ignored\n";
                }
            }
        }else{
            std::cout << "Error occured while reading file!";
        }

        system("rm -rf files_list.tmp");

        return file_names;
    }


    void processor::load_images(const std::string& path){
        relative_path = path;
        load_images();
    }

    void processor::compute_clusters(){
        if(images.empty()){
            std::cout << "Cannot compute clusters on empty image set\n";
            exit(0);
        }

        //Generate the image feature for each image
        for(int i = 0; i < images.size(); i++){
            images[i]->generate_image_feature(bin_size);
            //std::cout << "Loaded image: " << images[i]->get_name() << " with location:"; images[i]->image_feature.print();//DEBUG
        }

        //This function initilizes the centroids 
        init_means();

        //Setup a vector to track the 'previous' closest mean (If there is no change after an iteration we stop)
        std::vector<int> previous_cm(images.size());
        for(int i = 0; i < previous_cm.size(); i++)
            previous_cm[i] = images[i]->closest_mean;

        //Variable for tracking during the iterations
        bool done = false;
        int closest_mean;
        double last_dist;
        int iteration = 0;

        while(!done){
            iteration++;

            //Determine closest centroid for each image
            for (int i = 0; i < images.size(); i++){
                closest_mean = images[i]->closest_mean;
                last_dist = images[i]->get_distance(means[closest_mean]);
                double distance_tm = 0;
                for (int k = 0; k < num_means; k++){
                    distance_tm = images[i]->get_distance(means[k]);
                    if(last_dist > distance_tm){
                        last_dist = distance_tm;
                        closest_mean = k;
                    }                    
                }
                images[i]->closest_mean = closest_mean;
            }

            //Determine if any images has changed to a new centroid since the last cycle
            done = true;
            for (int i = 0; i < images.size(); i++){
                done = images[i]->closest_mean == previous_cm[i] ? done : false;

                //debug
                /*
                if (previous_cm[i] != images[i]->closest_mean){
                    std::cout << "Image: " << images[i]->get_name() << " "
                    "Previous mean: " << previous_cm[i] << " New mean: " << images[i]->closest_mean << std::endl;
                }
                */

                previous_cm[i] = images[i]->closest_mean;                
            }
            //If 'done' is true at this point, there was no change in mean locations

            if(!done){
                update_centroid_locations();
            }else{
                std::cout << "Finnished, total iterations: " << iteration << std::endl;
            }
        
        }

    }

    //This recalculates the locations of each centroid
    void processor::update_centroid_locations(){
        //std::cout << "Adjusting centroids\n";
        for(int k = 0; k < num_means; k ++){
            std::vector<double> new_mean(num_bins(bin_size));
            int divisor = 0;
            for(int i = 0; i < images.size(); i++){
                if(images[i]->closest_mean == k){
                    divisor++;
                    for(int b = 0; b < num_bins(bin_size); b++){
                        new_mean[b] += images[i]->image_feature.location[b];
                    }
                }
            }

            if(divisor > 0){
                for(int b = 0; b < num_bins(bin_size); b++){
                    new_mean[b] = new_mean[b]/(double)divisor;
                }
                means[k].location = new_mean;
            }else{
                std::cout << "A centroid with no closest images has formed\n"; //DEBUG
            }
        }
    }

    std::ostream& operator<<(std::ostream& os, const processor& proc){
        for(int k = 0; k < proc.num_means; k++){
            os << "cluster: " << k << ":";
            for (int i = 0; i < proc.images.size(); i++){
                if(proc.images[i]->closest_mean == k)
                    os << " " << proc.images[i]->get_name();
            }
            os << std::endl;
        }
        return os;
    }

    void processor::file_results(const std::string& of_name){
        std::ofstream file_obj;
        file_obj.open(of_name.c_str());
        file_obj << *this;
        file_obj.close();
    }

    void processor::init_means(){
        int dimension = images[0]->image_feature.location.size();        
        std::vector<double> location(dimension);
        //std::vector<std::vector<double>> ordering_list(images.size());
        //std::vector<double> origin(dimension);

        if(num_means > images.size())
        std::cout << "[Warn] There are more clusters than items to fill those clusters!\n"; //Wikipedia says this isn't allowed

        std::cout << "[DEBUG] Centroids are: " << dimension << " dimensional\n"; //debug

        //RANDOM SEEDED ALLOCATION METHOD        
        //Shuffle the images
        int seed = 1324458;
        std::shuffle(images.begin(), images.end(), std::default_random_engine(seed));

        //Assign images to the centroids
        int k = 0;
        for(int i = 0; i < images.size(); ++i, ++k){
            if (k == num_means){
                k = 0;
            }
                
            
            if (i < num_means)
                means.push_back({location});

            images[i]->closest_mean = k;
        }

        //ALREADY CLUSTERED CENTROID ALLOCATION
        //DEBUG - Print all image names and allocate as 'already clustered'
        /*
        int j = 0;
        for (int i = 0; i < images.size(); i++){
            std::cout << images[i]->get_name() << " into group " << j << std::endl;
            images[i]->closest_mean = j;
            if((i+1)%10 == 0)
                j++;
            if(j >= 10)
                j = 0;
        }
        */

        update_centroid_locations();



        //ORIGINAL INITILIZATION METHOD
        /*
        //Assign all images to ordering list [dist, index]
        for (int i = 0; i < images.size(); i++){
            ordering_list[i] = {images[i]->get_distance({origin}), (double) i};
        }

        //Order all images to be closest to furtherest from the origin
        std::vector<double> tmp;
        for (int i = 0; i < images.size()-1; i++){
            for (int j = i; j < images.size(); j++){
                if(ordering_list[i][0] > ordering_list[j][0]){
                    tmp = ordering_list[i];
                    ordering_list[i] = ordering_list[j];
                    ordering_list[j] = tmp;
                }
            }
        }

        //Set the centroids to be evenly distributed amoungst the data point locations using dist from origin as measure
        std::cout << "Determining centroid starting locations\n";
        int gap_size = images.size()/num_means;
        for (int i = 0; i < num_means; i++){
            int index = (i*gap_size) >= 1 ? (i*gap_size) : (i*gap_size*num_means) % images.size();

            location = images[(int) ordering_list[index][1]]->image_feature.location;

            means.push_back({location});
            means[i].print(); //debug
        }
        */
    }


}