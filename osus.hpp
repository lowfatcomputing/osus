//
// osus.hpp: by Hideo Bannai and Kazuya Tsuruta
// This is an implementation of the algorithm presented in:
// Shortest unique substrings in optimal time
// by Kazuya Tsuruta, Hideo Bannai, Shunsuke Inenaga, and Masayuki Takeda
// accepted to SOFSEM 2014.
//
// which is an improvement of the algorithm in:
// On Shortest Unique Substring Queries
// by Jian Pei, Wush Chi-Hsuan Wu, and Mi-Yen Yeh
// http://www.cs.sfu.ca/~jpei/publications/Shortest%20Unique%20Substring%20Queries%20ICDE13.pdf
//

#ifndef __OSUS_HPP__
#define __OSUS_HPP__
#include <vector>
#include <string>

class OSUS {
  std::vector<std::pair<size_t,size_t> > muInts;
  std::vector<size_t> S;
  size_t * lcp;
  size_t * rank;

public:
  // preprocess string s to compute SUSs
  OSUS(const std::string & s);

  // destructor
  ~OSUS();
  
  // compute all SUS of position p in O(k) time, where k is the number of such SUSs.
  std::vector<std::pair<size_t,size_t> > query(size_t p);

  // a semi-naive algorithm that computes all SUS of position p in O(N) time.
  std::vector<std::pair<size_t,size_t> > query_naive(size_t p);

  // return the left most element of SUS(p).
  std::pair<size_t,size_t> lmSUS(size_t p);

  // return the left most element of SUS(p) for all positions p in s.
  std::vector<std::pair<size_t,size_t> > lmSUS();
};

#endif//__OSUS_HPP__
