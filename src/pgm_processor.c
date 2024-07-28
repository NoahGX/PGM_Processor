#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // Include for directory processing

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height) {
    int maxGreyVal; // Variable to store maximum grey value
    char magicNum1; // Variable for storing first character of file's magic number
    char magicNum2; // Variable for storing second character of file's magic number
    FILE *imagePGM = NULL;
    imagePGM = fopen(filename, "r"); // Open file for reading

    // Error handling for invalid files
    if (imagePGM == NULL) {
        printf("Error: No such file or directory.\n");
        exit(1);
    }

    // Error handling for invalid file types
    fscanf(imagePGM, "%c%c", &magicNum1, &magicNum2); // Scan the provided file type
    if (magicNum1 != 'P' || magicNum2 != '5') {
        printf("Error: File is of incorrect type.\n");
        exit(1);
    }

    // Scan the image dimensions and store them
    fscanf(imagePGM, "%d %d", width, height);
    // Scan the maximum grey value and store it
    fscanf(imagePGM, "%d ", &maxGreyVal);

    // Dynamically allocate storage for 2D matrix rows using image dimensions
    PixelGray** thisMatrix = (PixelGray**)malloc((*height) * sizeof(PixelGray*));
    // Error handling for matrix rows
    if (thisMatrix == NULL) {
        printf("Error: Unable to allocate memory for matrix rows.\n");
        exit(1);
    }

    // Dynamically allocate storage for 2D matrix columns using image dimensions
    for (int i = 0; i < *height; i++) {
        thisMatrix[i] = (PixelGray*)malloc((*width) * sizeof(PixelGray));
        // Error handling for matrix columns
        if (thisMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for matrix columns.\n");
            for (int i = 0; i < *height; i++) {
                free(thisMatrix[i]); // If memory allocation fails, free memory for rows
            }
            free(thisMatrix); // Free memory for pointer array
            exit(1);
        }
        
        // Read from file and fill 2D matrix
        for (int j = 0; j < *width; j++) {
            fread(&(thisMatrix[i][j].value), 1, 1, imagePGM);
        }
    }
    // Close file and return 2D matrix
    fclose(imagePGM);
    return thisMatrix;
}

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height) {
    char magicNum[] = "P5"; // Magic number for PGM file
    int maxGreyVal = 255; // Maximum grey value for PGM file
    FILE *newPGM = NULL;
    newPGM = fopen(filename, "w"); // Open new file for writing

    // Error handling for invalid files
    if (newPGM == NULL) {
        printf("Error: No such file or directory.\n");
        exit(1);
    }
    
    // Print magic number to new file
    fprintf(newPGM, "%s\n", magicNum);
    // Print image dimensions to new file
    fprintf(newPGM, "%d %d\n", *width, *height);
    // Print maximum grey value to new file
    fprintf(newPGM, "%d\n", maxGreyVal);

    // Copy data from the provided 2D matrix to new file
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            fwrite(&(matrix[i][j].value), 1, 1, newPGM);
        }
    }
    // Close file and return to main function
    fclose(newPGM);
}

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height) {
    // Dynamically allocate storage for 2D matrix rows using image dimensions
    PixelGray** secondMatrix = (PixelGray**)malloc((*height) * sizeof(PixelGray*));
    // Error handling for matrix rows
    if (secondMatrix == NULL) {
        printf("Error: Unable to allocate memory for new matrix rows.\n");
        exit(1);
    }

    // Dynamically allocate storage for 2D matrix columns using image dimensions
    for (int i = 0; i < *height; i++) {
        secondMatrix[i] = (PixelGray*)malloc((*width) * sizeof(PixelGray));
        // If memory allocation fails, free memory to avoid memory leaks
        if (secondMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for new matrix columns.\n");
            for (int i = 0; i < *height; i++) {
                free(secondMatrix[i]); // Free memory for rows
            }
            free(secondMatrix); // Free memory for pointer array
            exit(1);
        }

        // Thresholding
        for (int j = 0; j < *width; j++) {
            if (matrix[i][j].value > 80) {
                secondMatrix[i][j].value = 255;
            }
            else {
                secondMatrix[i][j].value = 0;
            }
        }
    }
    // Return 2D matrix
    return secondMatrix;
}

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height) {
    // Dynamically allocate storage for 2D matrix rows using image dimensions
    PixelGray** thirdMatrix = (PixelGray**)malloc((*height) * sizeof(PixelGray*));
    // Error handling for matrix rows
    if (thirdMatrix == NULL) {
        printf("Error: Unable to allocate memory for new matrix rows.\n");
        exit(1);
    }

    // Dynamically allocate storage for 2D matrix columns using image dimensions
    for (int i = 0; i < *height; i++) {
        thirdMatrix[i] = (PixelGray*)malloc((*width) * sizeof(PixelGray));
        // If memory allocation fails, free memory to avoid memory leaks
        if (thirdMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for new matrix columns.\n");
            for (int i = 0; i < *height; i++) {
                free(thirdMatrix[i]); // Free memory for rows
            }
            free(thirdMatrix); // Free memory for pointer array
            exit(1);
        }

        // Rotating
        for (int j = 0; j < *width; j++) {
            thirdMatrix[i][j].value = matrix[*height - 1 - j][i].value;
        }
    }
    // Return 2D matrix
    return thirdMatrix;
}

// Helper function to check if a file is a PGM file
int isPGMfile(const char* filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return 0;
    } else if (strcmp(dot, ".pgm") == 0) {
        return 1;
    } else {
        return 0;
    }
}

// Main function to process each PGM file in current directory
int main() {
    DIR *dir;
    struct dirent *ent;
    char *dirName = "."; // Current directory

    if ((dir = opendir(dirName)) != NULL) {
        // Process all the files and directories within directory
        while ((ent = readdir(dir)) != NULL) {
            if (isPGMfile(ent->d_name)) {
                int width, height; // Variables to hold width and height
                char inputFileName[256];
                char outputFileName[256];

                // Construct full path for each file
                snprintf(inputFileName, sizeof(inputFileName), "%s/%s", dirName, ent->d_name);

                printf("Processing %s\n", inputFileName);
                PixelGray** image_original = readPGM(inputFileName, &width, &height);

                // Threshold the image
                PixelGray** image_thresh = threshold(image_original, &width, &height);
                snprintf(outputFileName, sizeof(outputFileName), "%s/threshold_%s", dirName, ent->d_name);
                writePGM(outputFileName, image_thresh, &width, &height);

                // Rotate the image
                PixelGray** image_rotate = rotate(image_original, &width, &height);
                snprintf(outputFileName, sizeof(outputFileName), "%s/rotate_%s", dirName, ent->d_name);
                writePGM(outputFileName, image_rotate, &width, &height);

                // Free the allocated memory
                for (int i = 0; i < height; ++i) {
                    free(image_original[i]);
                    free(image_thresh[i]);
                    free(image_rotate[i]);
                }
                free(image_original);
                free(image_thresh);
                free(image_rotate);
            }
        }
        closedir(dir);
    } else {
        // Could not open directory
        perror("Could not open directory");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}