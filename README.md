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
<flags>     [-e|-d] encrypt/decrypt
            [-s|-f] <arg> is a string/filename
<key>       - string input for encryption/decryption
<arg>       - string/filename to encrypt/decrypt