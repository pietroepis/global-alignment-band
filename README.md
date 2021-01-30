# global-alignment-band
Bioinformatics Project - UniMiB

This program improves the computation of the optimal global alignment of two sequences (Needleman-Wunsch algorithm), by using a band.\
_BLOSUM62_ was used as score matrix of reference

**Input**\
Input comes from `input.txt` file, that must respect following structure:\
l1 s1 l2 s2\
These values are separated by exactly one space, and have this meaning:\
- l1: Lenght of the first string
- s1: First string (l1 characters long)
- l2: Lenght of the second string
- s2: Second string (l2 characters long)

Allowed characters for s1 and s2 are: 'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V', 'B', 'Z' e 'X'

**Output**\
Output will be written in `output.txt` file\
This file includes the alignment value and the representation of aligned s1 and s2 (`-` stands for an indel)
