/*  cipher_main.cpp
    by Emmanuel Butor
    6 June 2019

    This is the main cpp file that encrypts and decrypts strings and files.
*/
#include <iostream>
#include <fstream>
#include <string>

// 10 numeric characters, 26 uppercase, 26 lowercase
// format: 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
#define MAX_CHARACTERS_TO_SHIFT 62

// FUNCTION PROTOTYPES
void printFullUsage(const std::string programName);
void printErrorAndExit(const std::string error);
int calculateShift(const std::string key);
char processCharacter(const int mode, const int shift, const char character);

// MAIN METHOD
int main (int argc, char* argv[])
{
    // Verify number of arguments //////////////////////////////////
    // verify number of arguments is 4 or 5
    if (argc < 4 || argc > 5)
    {
        printFullUsage(argv[0]);
        exit(1);
    }

    ////////////////////////////////////////////////////////////////
    // Process arguments ///////////////////////////////////////////
    // store argv into std::string array called args
    std::string args[argc - 1]; 
    for (int i = 1; i < argc; i++)
    {
        args[i - 1] = argv[i];
    }

    // process all arguments
    int cipherMode = 0;         // 0 = unassigned, 1 = encrypt, 2 = decrypt
    int processedType = 0;      // optional flag, 0 = unassigned, 1 = string, 2 = file
    std::string key = "";       // key for cipher shift
    std::string processed = ""; // name of file OR string to be processed
    for (int i = 0; i < argc - 1; i++)
    {
        // received a flag for encrypt/decrypt
        if (args[i].compare("-e") == 0 || args[i].compare("-d") == 0)
        {
            if (cipherMode == 0) // unassigned, assignment is given
            {
                cipherMode = (args[i].compare("-e") == 0 ? 1 : 2); // assign 1 if it is "-e", 2 if false
            }
            else // ERROR, multiple flags received
            {
                printErrorAndExit("ERROR: multiple -e/-d flags received");
            }
        }
        // received a flag for string/file (optional)
        else if (args[i].compare("-s") == 0|| args[i].compare("-f") == 0)
        {
            if (processedType == 0)
            {
                processedType = (args[i].compare("-s") == 0 ? 1 : 2); // assign 1 if it is "-s", 2 if false
            }
            else // ERROR, multiple flags received
            {
                printErrorAndExit("ERROR: multiple -s/-f flags received");
            }
        }
        // received a random string for either key or string/file
        else
        {
            // Assigning key + string/file
            // check if anything is populated for key
            if (key.compare("") == 0)
            {
                // assign key if nothing populated for key
                key = args[i];
            }
            // else, check if anything is populated for processed
            else if (processed.compare("") == 0)
            {
                processed = args[i];
            }
            // else ERROR, there is a problematic argument
            else
            {
                printErrorAndExit("ERROR: Too many arguments for <key> and <arg>");
            }
        }    
    }
    // now cipherMode, processedType, key, and processed have all been assigned
    // verify that cipherMode is not 0 otherwise error
    if (cipherMode == 0)
    {
        printErrorAndExit("ERROR: -e or -d was not read. Please ensure that one of these flags are used.");
    }
    ////////////////////////////////////////////////////////////////
    // Begin Encryption/Decryption on String/File
    // Default: -s or -f not provided, attempt to open as a file and process it; otherwise process it as string

    // String (processedType = 1): -s provided, treat processed as string

    // File  (processedType = 2): -f provided, treat processed as filename. if file does not exist, error and exit
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void printFullUsage()
// param    : const std::string programName
// desc     : prints full usage for program
void printFullUsage(const std::string programName)
{
    std::cerr << "usage      : " << programName << " <flags> <key> <arg>" << std::endl << std::endl << std::endl;


    std::cerr << "flags      : [-e/-d] required, encrypt/decrypt mode" << std::endl;
    std::cerr << "             [-s/-f] optional, prepares to process strings or files respectively" << std::endl;
    std::cerr << "             (if omitted, -f is assumed and goes to -s if arg fails to open as file)" << std::endl;
    std::cerr << "key        : char* to be converted into an integer when deciding shift amount for cipher" << std::endl;
    std::cerr << "arg        : string or file to processed" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void printErrorAndExit()
// param    : const std::string error
// desc     : prints error and any other information and terminates program
void printErrorAndExit(const std::string error)
{
    std::cerr << error << std::endl;
    std::cerr << "Program is terminated early." << std::endl;
    exit(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int calculateShift()
// param    : const std::string key
// desc     : takes key and uses simple algorithm to calculate shift amount and returns it
int calculateShift(const std::string key)
{
    // simple algorithm for shift: 
    //  add all character ascii values in key
    //  divide that sum by number of characters in key
    //  modulo MAX_CHARACTERS_TO_SHIFT
    int shiftAmount = 0;
    for (int i = 0; i < key.length(); i++)
    {
        shiftAmount += key[i];
    }
    shiftAmount /= key.length();
    shiftAmount %= MAX_CHARACTERS_TO_SHIFT;
    abs(shiftAmount); // shiftAmount must be positive at all times
    return shiftAmount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// char processCharacter()
// param    : const int mode, const int shift, const char character
// desc     : takes character and shifts it a certain number of characters in the direction based off of mode.
//            note it will shift within this range: "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
//            it will also ignore characters that are not numbers or letters
char processCharacter(const int mode, const int shift, const char character)
{
    // verify if character is number or uppercase letter or lowercase letter
    // note ascii ranges: number [48, 57], uppercase [65, 90], lowercase [97, 122]
    // ranges when omitting non-number/-uppercase/-lowercase characters:
    //  number [0, 9], uppercase [10, 35], lowercase [36, 61]
    //  this will be used with RANGE_OF_CHARACTERS

    const std::string RANGE_OF_CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    int characterNumber = -1; // -1 = unassigned
    // define characterNumber by reassigning it to the range string defined above as an index
    if (character >= 97 && character <= 122)
    {
        characterNumber = character - 97; // set it to be between 0-25
        characterNumber += 36; // lowercase letters start at index 36
    }
    else if (character >= 65 && character <= 90)
    {
        characterNumber = character - 65; // set it to be between 0-25
        characterNumber += 10; // uppercase letters start at index 10
    }
    else if (character >= 48 && character <= 57)
    {
        characterNumber = character - 48; // set it to be between 0-9
        // no additional changes, numbers start at index 0
    }
    else
    {
        // character is not letter or number, just return the character itself
        return character;
    }
    
    // Begin shifting here, return processed character
    if (mode == 1)          // encrypting
    {
        return RANGE_OF_CHARACTERS[(characterNumber + shift) % MAX_CHARACTERS_TO_SHIFT];
    }
    else if (mode == 2)     // decrypting
    {
        return RANGE_OF_CHARACTERS[(characterNumber - shift) % MAX_CHARACTERS_TO_SHIFT];
    }
    else                    // error here
    {
        printErrorAndExit("ERROR: Invalid mode parameter passed in processCharacter");
    }
}