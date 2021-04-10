# Assignment 3

 Generates subsequent generations of a specified grid according to the rules of the “The Game of Life.”

## Flags

Flag | Description 
-------|-------------------
-t | Specify that the Game of Life is to be played on a toroidal universe.
-s | Silence ncurses.  (don’t display the animated output and jump straight to the final output)
-n &lt;generations&gt; | Specify the number of generations that the universe goes through.<br /> The default number of generations is 100.
-i &lt;input&gt; |Specify the input file to read in order to populate the universe.<br /> By default the input should be stdin.
-o &lt;output&gt; |Specify the output file to print the final state of the universe to.<br /> By default the output should be stdout.



## Build
    $ make
    or
    $ make all


## Running

    $ ./life


## Cleaning
    $ make clean





