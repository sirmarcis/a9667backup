#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <utility>

#include "image.h"
#include "priority_queue.h"
//#include "priority_queue_with_hash.h"  //unfortunalty cannot include this as it does not compile on the server

// ===================================================================================================

// distance field method functions
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image);

// visualization style helper functions
Color Rainbow(double distance, double max_distance);
Color GreyBands(double distance, double max_distance, int num_bands);

// ===================================================================================================

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " input.ppm output.ppm distance_field_method visualization_style" << std::endl;
    exit(1);
  }

  // open the input image
  Image<Color> input;
  if (!input.Load(argv[1])) {
    std::cerr << "ERROR: Cannot open input file: " << argv[1] << std::endl;
    exit(1);
  }

  // a place to write the distance values
  Image<DistancePixel> distance_image;
  distance_image.Allocate(input.Width(),input.Height());

  // calculate the distance field (each function returns the maximum distance value)
  double max_distance = 0;
  if (std::string(argv[3]) == std::string("naive_method")) {
    max_distance = NaiveDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("improved_method")) {
    max_distance = ImprovedDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_map")) {
    max_distance = FastMarchingMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_hash_table")) {
    // EXTRA CREDIT: implement FastMarchingMethod with a hash table
  } else {
    std::cerr << "ERROR: Unknown distance field method: " << argv[3] << std::endl;
    exit(1);
  }

  // convert distance values to a visualization
  Image<Color> output;
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      double v = distance_image.GetPixel(i,j).getValue();
      if (std::string(argv[4]) == std::string("greyscale")) {
	output.SetPixel(i,j,GreyBands(v,max_distance*1.01,1));
      } else if (std::string(argv[4]) == std::string("grey_bands")) {
	output.SetPixel(i,j,GreyBands(v,max_distance,4));
      } else if (std::string(argv[4]) == std::string("rainbow")) {
	output.SetPixel(i,j,Rainbow(v,max_distance));
      } else {
	// EXTRA CREDIT: create other visualizations
	std::cerr << "ERROR: Unknown visualization style" << std::endl;
	exit(0);
      }
    }
  }
  // save output
  if (!output.Save(argv[2])) {
    std::cerr << "ERROR: Cannot save to output file: " << argv[2] << std::endl;
    exit(1);
  }
}

// ===================================================================================================

double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
    int w = input.Width();
    int h = input.Height();
    // return the maximum distance value
    double answer = 0;
    // loop over the pixels in the input image
    for (int i = 0; i < w; i++)  {
        for (int j = 0; j < h; j++) {
            double closest = -1;
            // loop over all other pixels in the input image
            for (int i2 = 0; i2 < w; i2++)  {
                for (int j2 = 0; j2 < h; j2++) {
                    const Color& c = input.GetPixel(i2,j2);
                    // skip all pixels that are not black
                    if (!c.isBlack()) continue;
                    // calculate the distance between the two pixels
                    double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
                    // store the closest distance to a black pixel
                    if (closest < 0 || distance < closest) {
                        closest = distance;
                    }
                }
            }
            assert (closest >= 0);
            answer = std::max(answer,closest);
            // save the data to the distance image
            DistancePixel& p = distance_image.GetPixel(i,j);
            p.setValue(closest);
        }
    }
    return answer;
}


double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
    int w = input.Width();
    int h = input.Height();
    std::vector<std::pair<int, int> > blackPoints;
    //first, get all the balck pixels positions and store them in a vector
    int cnt = 0;
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
                cnt++;
            const Color& c = input.GetPixel(i,j);
            if (c.isBlack()){  //if the pixel is black, store that position
                blackPoints.push_back(std::make_pair(i,j));
            }
        }
    }
    double answer = 0;
    // then, loop over the pixels in the input image
    for (int i = 0; i < w; i++)  {
        for (int j = 0; j < h; j++) {
            double closest = -1;
            // loop over all the black pixels in the image
            for (int blackElt = 0; blackElt < blackPoints.size(); blackElt++)  {
                int i2 = blackPoints[blackElt].first;
                int j2 = blackPoints[blackElt].second;
                double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
                // store the closest distance to a black pixel
                if (closest < 0 || distance < closest) {
                    closest = distance;
                }
            }
            assert (closest >= 0);
            answer = std::max(answer,closest);
            // save the data to the distance image
            DistancePixel& p = distance_image.GetPixel(i,j);
            p.setValue(closest);
        }
    }
    return answer;
}

void initialize_distance_image(Image<Color> &input, Image<DistancePixel> &distance_image,
                                                          std::vector<std::pair<int, int> > &blackPoints){
    //called by FastMarchingMethod to initialize the distance field before propagation and mark all black pixels as known distances (order notation w*h)
    int w = input.Width();
    int h = input.Height();
    double maxDistanceVal = 1000000;  //for easy access and editing, the initial distance value for all non-black pixels is stored here
    //first, get all the balck pixels positions and set their distance value to zero
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            const Color& c = input.GetPixel(i,j);
            DistancePixel& p = distance_image.GetPixel(i,j);
            p.setX(i); p.setY(j);  //update the pixels position
            if (c.isBlack()){  //if the pixel is black, store that position and initialize the distince value to zero
                blackPoints.push_back(std::make_pair(i,j));
                p.setValue(0);
                p.assertFinalDistance(); //set that distance to known
            }
            else  //else initialize the pixel to maxDistanceVal and distance to not known
                p.setValue(maxDistanceVal);
        }
    }
}

std::vector<std::pair<int, int> > propagate_pixel(const int i, const int j, const Image<DistancePixel> &distance_image){
    //called by FastMarchingMethod, finds all not known distance pixels around the current one
    std::vector<std::pair<int, int> > pixelPointArr;
    if((i-1) >= 0){
        if((j-1) >= 0){
            if(distance_image.GetPixel(i-1,j-1).finalDistancep() == false){
                pixelPointArr.push_back(std::make_pair(i-1,j-1)); //add point (i-1, j-1)
            }
        }
        if(j+1 < distance_image.Height()){
            if(distance_image.GetPixel(i-1,j+1).finalDistancep() == false){
                pixelPointArr.push_back(std::make_pair(i-1,j+1)); //add point (i-1, j+1)
            }
        }
        if(distance_image.GetPixel(i-1,j).finalDistancep() == false){
            pixelPointArr.push_back(std::make_pair(i-1,j)); //add point (i-1, j)
        }
    }
    if(j+1 < distance_image.Height())
        if(distance_image.GetPixel(i,j+1).finalDistancep() == false)
            pixelPointArr.push_back(std::make_pair(i,j+1)); //add point (i, j+1)
    if((j-1) >= 0)
        if(distance_image.GetPixel(i,j-1).finalDistancep() == false)
            pixelPointArr.push_back(std::make_pair(i,j-1)); //add point (i, j-1)
    if((i+1) < distance_image.Width()){
        if((j-1) >= 0)
            if(distance_image.GetPixel(i+1,j-1).finalDistancep() == false)
                pixelPointArr.push_back(std::make_pair(i+1,j-1)); //add point (i+1, j-1)
        if(j+1 < distance_image.Height())
            if(distance_image.GetPixel(i+1,j+1).finalDistancep() == false)
                pixelPointArr.push_back(std::make_pair(i+1,j+1)); //add point (i+1, j+1)
        if(distance_image.GetPixel(i+1,j).finalDistancep() == false)
            pixelPointArr.push_back(std::make_pair(i+1,j)); //add point (i+1, j)
    }
    return pixelPointArr;
}

double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
    std::vector<std::pair<int, int> > blackPoints;
    initialize_distance_image(input, distance_image, blackPoints); //initialize distance field
    DistancePixel_PriorityQueue pixelQueue;
    double answer = 0;
    //loop over distance_image until all distances are known
    //start be looping over blackPoints to fill the priorityQueue some, then loop until the queue is empty
    for(int x = 0; x < blackPoints.size(); x++){
        int i = blackPoints[x].first;
        int j = blackPoints[x].second;
        DistancePixel& blackPixel = distance_image.GetPixel(i,j);
        //propagate values around the current pixel
        std::vector<std::pair<int, int> > pixelPointArr = propagate_pixel(i,j, distance_image);
        for(int y = 0; y < pixelPointArr.size(); y++){  //loop over all points around current pixel, finding distance values and adding them to the priority queue
            int i2 = pixelPointArr[y].first;
            int j2 = pixelPointArr[y].second;
            DistancePixel& p = distance_image.GetPixel(i2,j2);
            double distance = (sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2))) + blackPixel.getValue(); //get the normal distance, plus however far away it is from a black pixel
            if(p.getValue() > distance){  //if the current distance is smaller than the one already there for that pixel, replace it
                p.setValue(distance);
                if(!pixelQueue.in_heap(&p)){  //if the pixel is not already in the heap, add it
                    pixelQueue.push(&p);
                }
                else
                    pixelQueue.update_position(&p);
            }
        }
    }
    //continue by taking the top element from the queue, setting its value to known, and propigating its neihbors
    while(pixelQueue.size() > 0){
        const DistancePixel* top = pixelQueue.top();  // get the pixel of most priority
        int i = top->getX();
        int j = top->getY();
        DistancePixel& blackPixel = distance_image.GetPixel(i,j);
        blackPixel.assertFinalDistance();
        //propagate values around the current pixel
        std::vector<std::pair<int, int> > pixelPointArr = propagate_pixel(i,j,distance_image);
        for(int y = 0; y < pixelPointArr.size(); y++){  //loop over all points around current pixel, finding distance values and adding them to the priority queue
            int i2 = pixelPointArr[y].first;
            int j2 = pixelPointArr[y].second;
            DistancePixel& p = distance_image.GetPixel(i2,j2);
            double distance = (sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2))) + blackPixel.getValue(); //get the normal distance, plus however far away it is from a black pixel
            if(p.getValue() > distance){  //if the current distance is smaller than the one already there for that pixel, replace it
                p.setValue(distance);
                if(!pixelQueue.in_heap(&p)){  //if the pixel is not already in the heap, add it
                    pixelQueue.push(&p);
                }
                else //else just update the pixels position in the queue
                    pixelQueue.update_position(&p);
            }

        }
        answer = std::max(answer, blackPixel.getValue());
        pixelQueue.pop(); //remove the found distance pixel from the queue
    }
    //printDistanceValues(input, distance_image);
    return answer;
}

// ===================================================================================================

Color Rainbow(double distance, double max_distance) {
  Color answer;
  if (distance < 0.001) {
    // black
    answer.r = 0; answer.g = 0; answer.b = 0;
  } else if (distance < 0.2*max_distance) {
    // blue -> cyan
    double tmp = distance * 5.0 / max_distance;
    answer.r = 0;
    answer.g = tmp*255;
    answer.b = 255;
  } else if (distance < 0.4*max_distance) {
    // cyan -> green
    double tmp = (distance-0.2*max_distance) * 5.0 / max_distance;
    answer.r = 0;
    answer.g = 255;
    answer.b = (1-tmp*tmp)*255;
  } else if (distance < 0.6*max_distance) {
    // green -> yellow
    double tmp = (distance-0.4*max_distance) * 5.0 / max_distance;
    answer.r = sqrt(tmp)*255;
    answer.g = 255;
    answer.b = 0;
  } else if (distance < 0.8*max_distance) {
    // yellow -> red
    double tmp = (distance-0.6*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = (1-tmp*tmp)*255;
    answer.b = 0;
  } else if (distance < max_distance) {
    // red -> white
    double tmp = (distance-0.8*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = tmp*255;
    answer.b = tmp*255;
  } else {
    // white
    answer.r = answer.g = answer.b = 255;
  }
  return answer;
}

Color GreyBands(double distance, double max_value, int num_bands) {
  Color answer;
  if (distance < 0.001) {
    // red
    answer.r = 255; answer.g = 0; answer.b = 0;
  } else {
    // shades of grey
    answer.r = answer.g = answer.b = int(num_bands*256*distance/double(max_value)) % 256;
  }
  return answer;
}

// ===================================================================================================
