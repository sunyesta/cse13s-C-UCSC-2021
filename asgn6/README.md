# Assignment 6: The Great Firewall of Santa Cruz: Bloom Filters, Linked Lists and Hash Tables


 Reads through an input text and detects if there are any "unallowed" words in it and will notify the user about them. If there are avalible replacements for those words, it will notify the user about them too.

## Flags

Flag | Description 
-------|-------------------
-m | will enable the move-to-front rule.
-h &lt;size&gt; | Specifies that the hash table will have size entries (the default will be 10000).
-f &lt;size&gt; | Specifies that the Bloom filter will have size entries (the default will be 2<sup>20</sup>).





## Build
    $ make
    or
    $ make all


## Running

    $ ./banhammer


## Cleaning
    $ make clean


