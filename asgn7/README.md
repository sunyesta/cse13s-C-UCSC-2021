# LZ78 data compression

Compresses and decompresses text using the LZ78 algorithm.

## Flags

### ./encode
Compresses text

Flag | Description 
-------|-------------------
-v |print the compression statistics to stderr
-i &lt;input&gt; |Specify input to compress (default: stdin)
-o &lt;output&gt; |Specify output of compressed input (default: stdout)
-h |Display program help and usage

### ./decode
Decompresses text

Flag | Description 
-------|-------------------
-v |print the compression statistics to stderr
-i &lt;input&gt; |Specify input to decompress (default: stdin)
-o &lt;output&gt; |Specify output of decompressed input (default: stdout)
-h |Display program help and usage

### ./entropy
course supplied file that calculates the entropy from the standard input

## Build
    $ make
    or
    $ make all


## Running

    $ ./encode
    $ ./decode
    $ ./entropy


## Cleaning
    $ make clean



