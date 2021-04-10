# Assignment 4

Encodes and decodes text with optional bit noise.

## Flags

### ./gen
Flag | Description 
-------|-------------------
-i &lt;input&gt; |Specify the input file for the program to read in order to encode
-o &lt;output&gt; |Specify the output file to print the encoded text

### ./dec
Flag | Description 
-------|-------------------
-i &lt;input&gt; |Specify the input file for the program to read in order to decode
-o &lt;output&gt; |Specify the output file to print the decoded text

### ./err
Flag | Description 
-------|-------------------
-s &lt;seed&gt; |seed
-e &lt;error rate&gt; |error rate

## Build
    $ make
    or
    $ make all


## Running

    $ ./gen
    $ ./err
    $ ./dec


## Cleaning
    $ make clean


