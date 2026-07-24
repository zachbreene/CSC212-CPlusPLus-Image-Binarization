/*
Zach Breene
HW1
Due 9/28
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

// Calculates the local threshold using a variation of Sauvola's binarization method.
float localEquation(vector<double> *area, float n){
  // Initialize values to avoid garbage data.
  double stdev, sum = 0, total = 0, mean = 0;
  int i, j;
  
  // Computes the mathematical mean of the extracted local neighborhood.
  for(i = 0; i < area->size(); i++){
      mean = mean + (*area)[i];
      total = total + 1;
    }
  mean = mean/total;
  
  // Computes the standard deviation of the local neighborhood.
  for(i = 0; i < area->size(); i++){
      sum = sum + (((*area)[i] - mean) * ((*area)[i] - mean));
    }
  sum = sum/total;
  stdev = sqrt(sum);
  
  // Calculates and returns the dynamic threshold based on the mean and stdev.
  double threshold = mean * (1 + (0.2 * ((stdev/n) - 1)));
  return threshold;
}

// Extracts the neighborhood of pixel values surrounding a specific center coordinate.
void localArea(vector<double> *area, vector<vector<double>> *imgTemp, int coord1, int coord2, int size){
  int i, j, rad = size/2;
  // Iterates through the defining radius of the window.
  for(i =- rad; i <= rad; i++){
    for(j =- rad; j <= rad; j++){
      // Bounds checking to ensure the window doesn't attempt to read outside the 2D matrix edges.
      if((((i + coord1) >= 0) and ((i + coord1) < (*imgTemp).size())) and (((j + coord2) >= 0) and ((j + coord2) < (*imgTemp)[coord1].size()))){
        area -> push_back((*imgTemp)[i + coord1][j + coord2]);
      }
    }
  }
}

// Reads the raw space-separated text data representing an image into a 2D vector.
void fRead(string f_name, vector<vector<double>> *img_data, vector<double> *arrComplete){
  string str;
  ifstream file(f_name);
  
  while (getline(file, str)){
    vector<double> rowCreate;
    istringstream ss(str);
    double tok;
    
    while(ss >> tok){
      rowCreate.push_back(tok);
      arrComplete->push_back(tok); // Also builds a 1D version of the data for median calculation.
    }
    
    img_data->push_back(rowCreate);
  }
}

// Writes the processed, binarized 2D matrix back into an output text file.
void fWrite(string f_name, vector<vector<double>> *const img_data){
  ofstream outFile(f_name);
  
  for(unsigned int i = 0; i < (*img_data).size(); i++){
    for(unsigned int j = 0; j < (*img_data)[i].size(); j++){
      outFile << (*img_data)[i][j] << " ";
    }
    outFile << "\n";
  }
}

int main (int argc, char* argv[]) {
  double threshold;
  vector<vector<double>> imgTemp;
  vector<double> imgSort;
  
  // Extracts the input and output filenames from the command line arguments.
  string inFile = argv[2], outFile = argv[3];
  fRead(inFile, &imgTemp, &imgSort);
  
  int height = imgTemp[0].size(), len = imgTemp.size(), total = height * len; 
  // Sorts the 1D version of the array to easily extract the global median.
  sort(imgSort.begin(), imgSort.end());
  
  int i, j;
  
  // GLOBAL BINARIZATION PATH
  if(argv[1][0] != 'l'){
    // Determines the mathematical median across the entire image.
    if(total % 2 != 1){
            threshold = (imgSort[total/2] + imgSort[(total / 2) - 1])/2;
        } 
        else {
            threshold = imgSort[total/2];
        }
        
        // Iterates through every pixel and applies the single global threshold.
        for(i = 0; i < len; i++) {
            for(j = 0; j < height; j++) {
                if(imgTemp[i][j] < threshold) {
                    imgTemp[i][j] = 0; // Converts to pure black.
                } else {
                    imgTemp[i][j] = 255; // Converts to pure white.
                }
            }
        }
        fWrite(outFile, &imgTemp);
  }
  
  // LOCAL (ADAPTIVE) BINARIZATION PATH
  if(argv[1][0] == 'l'){
    // Calculates a dynamic range constant 'n' based on min/max intensities.
    double n = 0.5 *(imgSort[total-1] - imgSort[0]);
    int size = stoi(argv[4], nullptr); // Takes the requested window size from CLI.
    vector<vector<double>> imgOut;
    int coord1, coord2;
    
    for(coord1 = 0; coord1 < len; coord1++){
      vector<double> rowCreate;
      for(coord2 = 0; coord2 < height; coord2++){
        vector<double> area;
        
        // Gets the neighborhood window for the specific pixel.
        localArea(&area, &imgTemp, coord1, coord2, size);
        
        // Computes the adaptive threshold solely for this specific coordinate.
        threshold = localEquation(&area, n);
        
        // Applies the local threshold to determine black vs white.
        if(imgTemp[coord1][coord2] < threshold){
          rowCreate.push_back(0);
        }
        else{
          rowCreate.push_back(255);
        }
      }
      imgOut.push_back(rowCreate);
    }
    fWrite(outFile, &imgOut);
  }
}