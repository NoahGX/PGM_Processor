# PGM Processor

## Overview
The `pgm_processor` is a C program designed to process PGM (Portable Graymap Format) image files. It provides functionalities for reading PGM images, applying thresholding and rotation transformations, and then writing the results back to new PGM files. The program processes all PGM files found in the current directory.

## Features
  - **Read PGM Images:** Load PGM format images into a 2D array of grayscale pixels.
  - **Write PGM Images:** Save modified images back into PGM format.
  - **Image Thresholding:** Convert images into binary (black and white) based on pixel intensity thresholds.
  - **Image Rotation:** Rotate images by 90 degrees counter-clockwise.
  - **Batch Processing:** Automatically processes all `.pgm` files in the current directory.

## Usage
To run the project:
  ```
  gcc -o pgm_processor pgm_processor.c
  ./pgm_processor
  ```
Use a C compiler like `gcc` and run the executable in a directory containing `.pgm` files.

## Prerequisites
  - A C compiler such as `gcc`.
  - Standard C libraries.
  - Operating system with support for directory processing functions (e.g., POSIX compliant).

## Input
The program expects `.pgm` files in the current directory. These files should be in binary PGM format (`P5`), with dimensions and maximum gray value specified.

## Output
For each input PGM file, the program outputs two files:
  - **Threshold Image:** A binary image where pixel values are set to either 0 or 255 based on a predefined threshold.
  - **Rotated Image:** An image rotated 90 degrees counter-clockwise.
The output files are named `threshold_<original_filename>.pgm` and `rotate_<original_filename>.pgm` and are saved in the current directory.

## Notes
  - Ensure that the directory permissions allow file reading and writing.
  - This program only processes binary PGM files (`P5`).
  - Memory allocation failures or unsupported file formats will cause the program to exit with an error message.
  - The program uses a hard-coded threshold value of 80 for binary conversion, which might not be suitable for all images.
