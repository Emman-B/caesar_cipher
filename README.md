# Caesar Cipher

This is an implementation of a Caesar Cipher. It can encrypt and decrypt files and strings using a provided key.
I used Ubuntu on the Windows Subsystem for Linux to compile it using g++. This is implemented with C++14.

## Author

Emmanuel Butor

## Usage

Download the repository. Once downloaded, compile it using the command: 
```bash
make
```
To clean up (remove the compiled files), use
```bash
make clean
```

The format for the input in the command line is as follows:
```bash
./cipher <flags> <key> <arg>
```
For the flags, there are four flags available. [-e] and [-d] are used for encrypting and decrypting the target respectively, and it is required to use one of the two flags. You cannot use both flags together. [-s] and [-f] are used for explicitly defining the target as a string and file respectively, and it is optional to use these flags. You cannot use both flags together.

Key and arg are both strings. Key takes a string and uses a simple formula to convert the string into a number that determines the shift amount for the Caesar cipher. Arg is either the target string or the target file that is to be encrypted or decrypted, determined by the [-e] or [-d] flag.