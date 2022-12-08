## JABDDL
Just Another BDD Library

## Description
A simple library to create and view functions using bdds.

## Installation
The library is written in c++11.
To use it clone this repository and compile the sources using the run.sh script

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
    TODO

## Project status
Currently finishing to setup input parsing and main function to handle processing of functions and variables.