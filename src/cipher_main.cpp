/*  cipher_main.cpp
    by Emmanuel Butor
    6 June 2019

    This is the main cpp file that encrypts and decrypts strings and files.
*/
#include <iostream>
#include <fstream>
#include <string>

// FUNCTION PROTOTYPES
void printVerificationError(const std::string programName);
std::string* splitString (const std::string str, const char delimiter); // has allocated memory

// MAIN METHOD
int main (int argc, char* argv[])
{
    // Verify arguments ////////////////////////////////////////////
    // verify number of arguments is 4 or 5
    if (argc < 4 || argc > 5)
    {
        printVerificationError(argv[0]);
        exit(1);
    }
    ////////////////////////////////////////////////////////////////
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void printVerificationError()
// param    : const std::string programName
// desc     : prints the error when verifying valid arguments
void printVerificationError(const std::string programName)
{
    std::cerr << "usage      : " << programName << " <flags> <key> <arg>" << std::endl << std::endl;

    std::cerr << "flags      : [-e/-d] required, encrypt/decrypt mode" << std::endl;
    std::cerr << "             [-s/-f] optional, prepares to process strings or files respectively" << std::endl;
    std::cerr << "             (if omitted, -f is assumed and goes to -s if arg fails to open as file)" << std::endl;
    std::cerr << "key        : char* to be converted into an integer when deciding shift amount for cipher" << std::endl;
    std::cerr << "arg        : string or file to processed" << std::endl;
}

// std::string* splitString()
// param    : const std::string str, const char delimiter
// desc     : splits a std::string with a given char delimiter and 
//            stores it into std::string array with allocated memory (needs to be freed)
//            array format: index 0 has size of array, every other index contains the strings after splitting
std::string* splitString (const std::string str, const char delimiter)
{
    int arraySize = 1; // tracks array size, minimum size is 1
    // loop to count size of array
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == delimiter)
        {
            arraySize++;
        }
    }

    std::string* strings = new std::string[arraySize + 1]; // storage for split strings (+ 1 due to special first index in array)
    // first index contains size of array as string
    strings[0] = std::to_string(arraySize);
    int arrayIndex = 1; //start at index = 1 as index = 0 has array size
    
    // startIndex and endIndex is used with substr 
    int startIndex = 0;
    int endIndex = str.find(delimiter, startIndex); //find the first delimiter
    while (endIndex != std::string::npos) // keep finding strings until no more can be found
    {
        if (endIndex == startIndex) // if there are multiple delimiters in a row, this will occur
        {
            // put empty string into the array
            strings[arrayIndex] = "";
            // push the startIndex 1 index forward
            startIndex++;
            // update arrayIndex
            arrayIndex++;
        }
        else
        {
            // make substring starting from startIndex
            // length of substring is endIndex - startIndex
            // no need to subtract 1 due to endIndex being on the delimiter because of zero-indexing
            strings[arrayIndex] = str.substr(startIndex, endIndex - startIndex);
            
            // update arrayIndex
            arrayIndex++;
            // startIndex is set to be one more than endIndex
            startIndex = endIndex + 1;
        }
        endIndex = str.find(delimiter, startIndex);
    }
    // in all cases, the last substring in the full string is ignored
    // set that substring to be the final string in the array
    strings[arrayIndex] = str.substr(startIndex);
    return strings;
}