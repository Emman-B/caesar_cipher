/*  cipher_main.cpp
    by Emmanuel Butor
    6 June 2019

    This is the main cpp file that encrypts and decrypts strings and files.
*/
#include <iostream>
#include <fstream>
#include <string>


int main (int argc, char* argv[])
{
    // Verify valid number of arguments (4-5)
    if (argc < 4 || argc > 5)
    {
        std::cerr << "usage      : " << argv[0] << " <flags> <key> <arg>" << std::endl << std::endl;

        std::cerr << "flags      : [-e/-d] required, encrypt/decrypt mode" << std::endl;
        std::cerr << "             [-s/-f] optional, prepares to process strings or files respectively" << std::endl;
        std::cerr << "             (if omitted, -f is assumed and goes to -s if arg fails to open as file)" << std::endl;
        std::cerr << "key        : char* to be converted into an integer when deciding shift amount for cipher" << std::endl;
        std::cerr << "arg        : string or file to processed" << std::endl;
        exit(1);
    }
    return 0;
}