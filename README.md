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
To clean up, use
```bash
make clean
```

The format for the input in the command line is as follows:
```bash
./cipher <flags> <key> <arg>
```
Flags:  - [-e]/[-d] (required) determines mode, either encrypting[-e] or decrypting[-d] <arg>
        - [-s]/[-f] (optional) determines whether to treat <arg> as a string[-s] or file[-f]; if omitted, it attempts to open <arg> as a file and output a file, if it fails to open it treats it as a string
Key:    - keys are of type string; the first string read from the command line is set as the key; the key decides the shift amount
Arg:    - arg are of type string; the second string read from the command line is set as the arg; the arg is the string or name of the file          being processed
There cannot be more than one flag of [-e] or [-d]. The same is said with [-s] or [-f].