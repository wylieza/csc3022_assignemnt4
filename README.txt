>>> K-Means Classification Application <<<


--- Instructions for use ---
The images (of format .ppm) must be contained inside a folder. This folder must be placed in the same
directory as the executable (clusterer).

Given the name of this folder (as an argument) the program will scan the folder for any files with
the .ppm extension and attempt to import and classify these files.

I have tested the image importing on nightmare and it works (requirement). It also works in the linux subsystem on windows 10.
The implementation is not designed to work on windows and has not been tested.

Example:
./clusterer <folder name>

-- Program Arguments --

<input folder> [-o output] [-k n] [-bin b] [-color]

<input folder> -> Folder containing images to be classified
[-o output] -> Specify a file to contain the classification results (extension must be provided)
[-k n] -> Specify the number of clusters to classify
[-bin b] -> Specify the 'bin width' for histogram generation
[-color] -> Do not convert images to greyscale, use color to enhance the k-means clustering


--- MAKEFILE ---
The makefile allows for compilation, execution and removel of binaries.

Command -> Action
_________________________________________________________________________________________________
>> Standard Commands <<
make                            -> Compile all source code
make clean                      -> Remove all binaries
make run args="<Custom args>"   -> Runs the clusterer binary with the supplied argument(s)
_________________________________________________________________________________________________

Note: The application can also be executed in the terminal using the usual ./clusterer <args>


--- Source Code Description ---
>> clusterer.cpp <<
This is the 'driver' and contains the main() method. This source code interperates the arguments given
by the user and then calls apon the appropriate methods in the Clusterer class to perform the
image classification. It does this through an instance of the processor class.

>> processor.h <<
This header file contains the processor class declaration. This class is responsible for importing
image data, computing the clusters (K-means tecnique) and finally exporting or printing the results.

>> processor.cpp <<
This source constitutes all the definitions of the methods declared in the processor class.

>> ppm.h <<
This header file contains the ppm class declaration and a structure definition for 'feature'.
An instance of the ppm class is used to contain all the information for a single image, including
but not limited to: image histogram, image filename, dimensions and manipulation methods. The
The 'feature' structure decribes an N dimensional vector (or 'feature') that describes a location
in N dimensional space. An instance of this stuct is used for both the images and the cluster
centroids.

>> ppm.cpp <<
This source constitutes all the definitions of the methods declared in the ppm class.


--- Student Details ---
Work of Justin Wylie
Student number: WYLJUS002