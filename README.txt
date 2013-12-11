--------------------------------------------------
osus by Hideo Bannai and Kazuya Tsuruta
Copyright (c) 2013
See file LICENSE for copying
--------------------------------------------------
For a given string s of length N, an interval [i..j] is unique if s[i..j] does not occur
anywhere else in s. A unique interval [i..j] is a Shortest Unique Substring (SUS)
at position p, if i \leq p \leq j and there is no shorter unique interval [i'..j']
where i' \leq p \leq j'.

SUSs were first considered in [2]. However, 
- their algorithm was O(N^2) time, and 
- they only considered the left-most SUS (lmSUS) of each position.

This program implements the algorithm proposed in the paper [1], which
computes the lmSUS for all positions of the input string, in O(N) time.
The algorithm can also compute all SUSs for any given position p in O(k)
time, where k is the number of SUSs at position p, if we are allowed O(N) 
time pre-processing of the string. 

If you use this code for experiments in a research paper, please cite the
paper [1] and publish the URL from which you downloaded the code.

--

The libdivsufsort-lite library by Yuta Mori is used for suffix array construction.
Scons (http://www.scons.org/) is used for the building process.
Just type "scons" if you have it installed.

-- Usage:

Usage  : ./osus [options]
Options: 
  -f iFile : file to process
  -n int   : truncate input string to size
  -q int   : query position (output all if negative (default))
  -l       : only output leftmost SUS
  -c       : (DEBUG) check results against semi-naive algorithm
  -h       : usage

------------------------------
Ex:
$ ./osus -f test.txt 
input string length = 9
0: [0,1];
1: [0,1];
2: [0,2];
3: [0,3];
4: [0,4];[2,6];[4,8];
5: [5,8];
6: [6,8];
7: [7,8];
8: [8,8];
------------------------------

--------------------------------------------------
References:

  [1] Shortest Unique Substrings Queries in Optimal Time,
      Kazuya Tsuruta, Shunsuke Inenaga, Hideo Bannai, and Masayuki Takeda,
      SOFSEM '14 Proceedings of the 40th International Conference on
      Current Trends in Theory and Practice of Computer Science.

  [2] On Shortest Unique Substring Queries
      Jian Pei, Wush Chi-Hsuan Wu, and Mi-Yen Yeh
      ICDE '13 Proceedings of the 2013 IEEE International Conference on 
      Data Engineering (ICDE 2013) Pages 937-948 
      http://dx.doi.org/10.1109/ICDE.2013.6544887
      http://www.cs.sfu.ca/~jpei/publications/Shortest%20Unique%20Substring%20Queries%20ICDE13.pdf
