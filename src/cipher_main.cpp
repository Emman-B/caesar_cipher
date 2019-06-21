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

// define constants for encryption/decryption modes
#define MODE_ENCRYPT 1
#define MODE_DECRYPT 2

// define constants for string/file types
#define MODE_STRING 1
#define MODE_FILE 2

// FUNCTION PROTOTYPES
void printFullUsage(std::string programName);
void printErrorAndExit(std::string error);
int calculateShift(std::string key);
char processCharacter(int mode, int shift, char character);
std::string processString(std::string key, std::string stringToProcess, int cipherMode);
std::ofstream processFile(std::string key, std::string fileName, std::ifstream* fileToProcess, int cipherMode);

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
    int targetType = 0;         // optional flag, 0 = unassigned, 1 = string, 2 = file
    std::string key = "";       // key for cipher shift
    std::string target = "";    // name of file OR string to be processed
    for (int i = 0; i < argc - 1; i++)
    {
        // received a flag for encrypt/decrypt
        if (args[i].compare("-e") == 0 || args[i].compare("-d") == 0)
        {
            if (cipherMode == 0) // unassigned, assignment is given
            {
                cipherMode = (args[i].compare("-e") == 0 ? MODE_ENCRYPT : MODE_DECRYPT); // assign 1 if it is "-e", 2 if false
            }
            else // ERROR, multiple flags received
            {
                printErrorAndExit("ERROR: multiple -e/-d flags received");
            }
        }
        // received a flag for string/file (optional)
        else if (args[i].compare("-s") == 0|| args[i].compare("-f") == 0)
        {
            if (targetType == 0)
            {
                targetType = (args[i].compare("-s") == 0 ? MODE_STRING : MODE_FILE); // assign 1 if it is "-s", 2 if false
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
            // else, check if anything is populated for target
            else if (target.compare("") == 0)
            {
                target = args[i];
            }
            // else ERROR, there is a problematic argument
            else
            {
                printErrorAndExit("ERROR: Too many arguments for <key> and <arg>");
            }
        }    
    }
    // now cipherMode, targetType, key, and target have all been assigned
    // verify that cipherMode is not 0 otherwise error
    if (cipherMode == 0)
    {
        printErrorAndExit("ERROR: -e or -d was not read. Please ensure that one of these flags are used.");
    }
    // verify that a key and a target were both provided
    if (key.compare("") == 0 || target.compare("") == 0)
    {
        printErrorAndExit("ERROR: Key or target was not provided.");
    }

    ////////////////////////////////////////////////////////////////
    // Begin Encryption/Decryption on String/File
    switch (targetType)
    // I used extra braces for each of the cases as an error kept getting thrown
    {
        //==============================================================
        // String (targetType = 1): -s provided, treat target as string
        case MODE_STRING:
        {
            std::cout << "Doing string " << (cipherMode == MODE_ENCRYPT ? "encryption" : "decryption") << std::endl;

            // set output to be the final encrypted/decrypted string
            std::string output = processString(key, target, cipherMode);

            exit(0);
            break;
        }
        //==============================================================
        // File  (targetType = 2): -f provided, treat target as filename. if file does not exist, error and exit
        case MODE_FILE:
        {
            // file for reading
            std::ifstream readFile;
            // open the file (it closes in the "if" part of the next conditional)
            readFile.open(target);
            // process the file only if it is open
            if (readFile.is_open())
            {
                std::cout << "File opened successfully. " << (cipherMode == MODE_ENCRYPT ? "Encrypting " : "Decrypting ") << target << "." << std::endl;
                // file for outputting the encrypted/decrypted file
                std::ofstream output = processFile(key, target, &readFile, cipherMode);
                
                // close the input file and output file
                output.close();
                readFile.close();
                // exit successfully
                exit(0);
            }
            // otherwise, throw an error if it failed to open
            else
            {
                printErrorAndExit("ERROR: File " + target + " failed to open.");
            }
            break;
        }
        //==============================================================
        // Default: -s and -f not provided, attempt to open as a file and process it; otherwise process it as string
        default:
        {
            std::cout << "-s and -f were not provided. Attempting to open " << target << " as a file.\n";
            std::ifstream readFile;  // readFile is the file being opened
            readFile.open(target);   // attempt to open the file
            // if the file doesn't open, treat it like a string instead
            if (!readFile.is_open())
            {
                // processString returns the encrypted/decrypted string
                std::cout << "File failed to open. Treating " << target << " as a string.\n";
                std::string output = processString(key, target, cipherMode);

                // exit successfully
                exit(0);
            }
            // if the file does open, prepare to write to a file
            else
            {
                std::cout << "File opened successfully. " << (cipherMode == MODE_ENCRYPT ? "Encrypting " : "Decrypting ") << target << std::endl;
                std::ofstream output = processFile(key, target, &readFile, cipherMode);
                // close the input file and output file
                output.close();
                readFile.close();
                // exit successfully
                exit(0);
            }
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void printFullUsage()
// param    : std::string programName
// desc     : prints full usage for program
void printFullUsage(std::string programName)
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
// param    : std::string error
// desc     : prints error and any other information and terminates program
void printErrorAndExit(std::string error)
{
    std::cerr << error << std::endl;
    std::cerr << "Program is terminated early." << std::endl;
    exit(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int calculateShift()
// param    : std::string key
// desc     : takes key and uses simple algorithm to calculate shift amount and returns it
int calculateShift(std::string key)
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
// param    : int mode, int shift, char character
// desc     : takes character and shifts it a certain number of characters in the direction based off of mode.
//            note it will shift within this range: "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
//            it will also ignore characters that are not numbers or letters
char processCharacter(int mode, int shift, char character)
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
        if (characterNumber < shift) // handle when (characterNumber - shift) is negative
        {
            return RANGE_OF_CHARACTERS[MAX_CHARACTERS_TO_SHIFT + (characterNumber - shift)];
        }
        return RANGE_OF_CHARACTERS[(characterNumber - shift) % MAX_CHARACTERS_TO_SHIFT];
    }
    else                    // error here
    {
        printErrorAndExit("ERROR: Invalid mode parameter passed in processCharacter");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// std::string processString()
// param    : std::string key, std::string stringToProcess, int cipherMode
// desc     : takes a key to calculate shift and runs the caesar cipher on the stringToProcess
//            cipherMode is used to determine whether to encrypt or decrypt
std::string processString(std::string key, std::string stringToProcess, int cipherMode)
{
    std::string output = "";            // this is for outputting to stdout
    int shift = calculateShift(key);    // receive shift amount
    for (int i = 0; i < stringToProcess.length(); i++)
    {
        output += processCharacter(cipherMode, shift, stringToProcess[i]);
    }
    
    std::cout << "Key: " << key << std::endl;
    std::cout << "String: " << stringToProcess << std::endl;
    std::cout << (cipherMode == MODE_ENCRYPT ? "Encrypted" : "Decrypted") << " string: " << output << std::endl;
    
    // exit successfully
    return output;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// std::ofstream processFile()
// param    : std::string key, std::string fileName, std::ifstream* fileToProcess, int cipherMode
// desc     : takes a key to calculate shift and runs the caesar cipher on the fileToProcess
//            cipherMode is used to determine whether to encrypt or decrypt
std::ofstream processFile(std::string key, std::string fileName, std::ifstream* fileToProcess, int cipherMode)
{
    // return output
    std::ofstream output;
    // name of output file should be "<originalfile>.encrypted" or "<originalfile>.decrypted"
    output.open(fileName + (cipherMode == MODE_ENCRYPT ? ".encrypted" : ".decrypted"));
    // loop through all the characters in the file, store each char into character
    int character = (*fileToProcess).get();
    while(character != EOF)
    {
        output << processCharacter(cipherMode, calculateShift(key), (char)character);
        character = (*fileToProcess).get();
    }

    std::cout << "Key: " << key << std::endl;
    std::cout << "Input file: " << fileName << std::endl;
    std::cout << "Output file: " << fileName + (cipherMode == MODE_ENCRYPT ? ".encrypted" : ".decrypted") << std::endl;
    // return output to be dealt with in main()
    return output;
}