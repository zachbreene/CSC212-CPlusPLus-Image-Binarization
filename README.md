<h1 align=center> Binarization Algorithm </h1>
<h2 align=center> A C++ Project by Zachary Breene </h2>
</n>
<h4 align=center> *NO AI WAS USED IN THE CREATION OF THIS PROJECT* </h4>

## Introduction
The goal of this assignment was to implement an algorithm capable of transforming a grayscale image being represented as a txt file of a grid of numbers from 0-255 into a pure black-and-white binary image. Because lighting and shadows across an image can cause standard global thresholds to perform poorly (often washing out details or blocking them in pure black), this project implements two different approaches: a standard Global Binarization method based on image median, and an Adaptive Local Binarization method that calculates dynamic thresholds for individual pixels based on their immediate neighborhood.

---

## Implementation + Functions
### binarize.cpp

This main source file contains the logic for reading 2D matrix text files and executing the mathematical thresholding algorithms. </n>

&emsp; ***Matrix Parsing & File Handling***

*   The program relies on `fRead()` to stream space-separated pixel intensity values from an input file into a 2D vector for spatial mapping, while simultaneously building a 1D vector list to easily sort for global minimums, maximums, and medians. The `fWrite()` function translates the finalized data back into a text file.

&emsp; ***Global Binarization Method***

*   When running globally, the program sorts the 1D array to locate the exact median pixel intensity of the entire image. It then performs a blanket iteration across the 2D matrix, forcing any pixel strictly under the median to `0` (Black) and any pixel above the median to `255` (White).

&emsp; ***Adaptive Local Binarization Method***

*   When executing locally, the program determines a unique threshold for *every single pixel*. It utilizes `localArea()` to extract the values of the surrounding neighborhood window while respecting edge boundaries. 
*   The neighborhood values are then passed to `localEquation()`, which implements an adaptive thresholding algorithm based on the local mean and local standard deviation.

---

## How To Run
To execute the program, download the source code and compile it utilizing your standard C++ compiler (e.g., `g++ binarize.cpp -o binarize`). Note that this program parses raw pixel data rather than standard image files; your input must be a .txt file containing a space-separated 2D matrix of grayscale pixel intensities (0-255). The resulting executable expects command-line arguments to dictate which algorithm to run.

*   **For Global Binarization:** Pass the flag `g`, followed by your input and output text file paths. 
    *   *Example:* `./binarize g image_input.txt image_output.txt`
*   **For Local Binarization:** Pass the flag `l`, followed by your input/output paths, and finally an odd integer representing the size of your local neighborhood window (e.g., `3`, `5`, `11`, `21`). 
    *   *Example:* `./binarize l image_input.txt image_output.txt 11`

---

## Contribution
As I was the sole member of this project, I contributed to the whole of the project. This contribution is as follows:
*   Implementation of File I/O for multidimensional array structures
*   Development of Global median-based sorting and thresholding
*   Creation of Adaptive Local spatial algorithms and boundary constraint checking
