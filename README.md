## JABDDL
Just Another BDD Library

## Description
A simple library to create and view functions using bdds.

## Installation
The library is written in c++17.
To use it clone this repository and run:
    ```bash
    foo@bar:~$ mkdir build && cd build
    foo@bar:~$ cmake ..
    foo@bar:~$ make
    ```

## Usage 
The program will take an input.txt file formatted in the following way:

4                               //number of variables
3                               //number of functions
x1 x3 x4 x2                     //Variables names 
F1 = ite(x1, x2, 0)             //functions ...
F2 = ite(x3, x4, 1)             //...
F3 = ite(F2, F1+x1, F1-x1)      //...
print(F3)                       //prints of functions
                                //...

## Authors
Daniele Nicoletti - daniele.nicoletti@studenti.univr.it

## License
JABDDL

Copyright © 2023 Daniele Nicoletti 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Project status
Currently finishing to setup input parsing and main function to handle processing of functions and variables.