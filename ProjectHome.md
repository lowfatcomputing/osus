implementation of shortest unique substring queries algorithm.

For a given string s of length N, an interval [i..j] is unique if s[i..j] does not occur
anywhere else in s. A unique interval [i..j] is a Shortest Unique Substring (SUS)
at position p, if i \leq p \leq j and there is no shorter unique interval [i'..j']
where i' \leq p \leq j'.

SUSs were first considered in (2). However,
  * their algorithm was O(N^2) time, and
  * they only considered the left-most SUS (lmSUS) of each position.

This program implements the algorithm proposed in the paper (1), which
computes the lmSUS for all positions of the input string, in O(N) time.
(For strange reasons, we have changed the name of the implementation from TSUS to OSUS ;-P)

The algorithm can also compute all SUSs for any given position p in O(k)
time, where k is the number of SUSs at position p, if we are allowed O(N)
time pre-processing of the string.

References:

(1) Shortest Unique Substrings Queries in Optimal Time, Kazuya Tsuruta, Shunsuke Inenaga, Hideo Bannai, and Masayuki Takeda, SOFSEM '14 Proceedings of the 40th International Conference on Current Trends in Theory and Practice of Computer Science. http://sofsem14.ics.upjs.sk/

(2) On Shortest Unique Substring Queries, Jian Pei, Wush Chi-Hsuan Wu, and Mi-Yen Yeh, ICDE '13 Proceedings of the 2013 IEEE International Conference on Data Engineering (ICDE 2013) Pages 937-948 http://dx.doi.org/10.1109/ICDE.2013.6544887, http://www.cs.sfu.ca/~jpei/publications/Shortest%20Unique%20Substring%20Queries%20ICDE13.pdf