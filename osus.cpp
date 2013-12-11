//
// osus.cpp: by Hideo Bannai and Kazuya Tsuruta
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

#include <iostream>
#include "osus.hpp"
#include "divsufsort.h"

static void Create_LCP_RANK(const std::string &s, size_t lcp[], size_t rank[]){
  const unsigned char * text = reinterpret_cast<const unsigned char *>(s.c_str());
  int  i, h, x;
  int N = s.size();
  int  * sa   = new int[N];

  // create suffix array, rank array, lcp array, //////////
  divsufsort(text, sa, N);  
  for(i = 0; i < N; i++) rank[sa[i]] = i;  
  lcp[0] = 0;
  for(i = h = 0; i < N; i++){
    x = rank[i];
    if(x > 0){
      const unsigned char * p0, * p1, * ep = text + N;
      int j = sa[x-1];
      p1 = text + i + h;
      p0 = text + j + h;
      while((p0 != ep) && (p1 != ep) && (*p1 == *p0)){
        p1++; p0++; h++;
      }
      lcp[x] = h;
      if(h > 0) h--;
    }
  }
  delete [] sa; // we don't need suffix array anymore
}

static inline size_t cover(const std::pair<size_t,size_t> mI, size_t p){  
  return (std::max(mI.second,p) - std::min(mI.first, p) + 1);
}

// get length of MUI starting at position p if one exists (0 if it does not exist)
static inline size_t getMUI(size_t p, size_t rank [], size_t lcp [], size_t N){
  size_t  minimalLength = 0, x;
  x = rank[p];
  // length of shortest substring starting at position p that is unique.
  // i.e.: one longer than max of lcp between neighboring suffixes
  minimalLength = std::max(lcp[x], (x+1 < N) ? lcp[x+1] : 0) + 1;
  if(p+1 < N){ // if the suffix starting at position p is longer than 1
    size_t y = rank[p+1];
    // length of right minimal unique substring starting at position p+1
    size_t ly = std::max(lcp[y], (y+1 < N) ? lcp[y+1] : 0) + 1;
    if(ly < minimalLength) minimalLength = 0; // not MUI since shorter substring is still unique
  }
  return minimalLength;
}


OSUS::OSUS(const std::string & s){
  size_t N = s.size(), i, j;
  lcp  = new size_t[N];
  rank = new size_t[N];
  Create_LCP_RANK(s, lcp, rank);  

  // this->muInts: an array holding the meaningful MUIs from left to right.
  // this->S: an array of Length N that holds the index in MUI which gives SUS(i)
  S.resize(N);
  for(i = 0; i < N; i++){
    // 1. check if there is a MUI starting at position i
    //    if so, set its length to minimalLength (minmalLength = 0 if there is none)
    size_t minimalLength = getMUI(i, rank, lcp, N);
    
    // 2. determine the left most MUI which starts before i, and is a member of SUS(i)
    // this should be the same MUI as at position i-1, or the next MUI in muInts
    if(i == 0) S[0] = 0;
    else if((S[i-1] + 1 >= muInts.size()) || cover(muInts[S[i-1]],i) <= cover(muInts[S[i-1]+1],i))
      S[i] = S[i-1];
    else S[i] = S[i-1] + 1;

    // 3. if there exists a MUI starting at position i, we update previous positions 
    // and muInts if needed.
    if(minimalLength > 0){
      std::pair<size_t,size_t> newMUI(i, i + minimalLength - 1); // the new MUI
      if(muInts.size() > 0){
	// find the rightmost position  0 < j <= i that doesn't need to be updated 
	// (S[0] can never be updated, so we don't need an exit condition)
	for(j = i; ; j--)
	  if(cover(muInts[S[j]],j) <= cover(newMUI,j)){ break; }
	if(j == i){
	  // if updates aren't needed, MUIs after S[i] and are longer are meaningless; 
	  // remove them (from the back)
	  while(muInts.back() != muInts[S[i]]){
	    if(cover(muInts.back(),i) <= cover(newMUI, i)) break;
	    muInts.pop_back();
	  }
	} else {
	  // if updates are needed, the MUIs after the ones that give SUS(j) are meaningless;
	  // remove them (from the back)
	  size_t lastmui = S[j];
	  while(muInts.back() != muInts[S[j]]){
	    if(cover(muInts.back(), j) <= cover(muInts[lastmui],j)) break;
	    muInts.pop_back();
	  }
	  for(j++; j <= i; j++) S[j] = muInts.size(); // update to newest MUI
	}
      }
      muInts.push_back(newMUI); 
    }
  }
}

OSUS::~OSUS(){
  delete [] rank;
  delete [] lcp;
}

// compute all SUS of position p in O(k) time, where k is the number of such SUSs.
std::vector<std::pair<size_t,size_t> > OSUS::query(size_t p){
  std::vector<std::pair<size_t,size_t> > sus;
  size_t i = S[p], l;
  sus.push_back(std::make_pair(std::min(muInts[i].first, p), 
			       std::max(muInts[i].second,p)));
  l = cover(muInts[i], p); i++;
  while(i < muInts.size() && (cover(muInts[i], p) <= l)){
    sus.push_back(std::make_pair(std::min(muInts[i].first, p), 
				 std::max(muInts[i].second,p)));
    i++;
  }
  return sus;
}



// a semi-naive algorithm that computes all SUS of position p in O(N) time.
std::vector<std::pair<size_t,size_t> > OSUS::query_naive(size_t p){
  size_t N = S.size(), susl = N;
  std::vector<std::pair<size_t,size_t> > sus;

  // check to the left of position p (including p)
  for(size_t i = p + 1; i-- > 0;){ // loop: i = p down to 0, taking care that i is unsigned
    size_t minimalLength = getMUI(i, rank, lcp, N);
    if(minimalLength > 0){
      std::pair<size_t,size_t> newMUI(i, i + minimalLength - 1);
      if(cover(newMUI, p) < susl){
	sus.clear(); susl = cover(newMUI, p);
      }
      if(cover(newMUI, p) == susl){
	sus.push_back(std::make_pair(std::min(p,newMUI.first),
				     std::max(p,newMUI.second)));
      }
      if(newMUI.second < p) break; // the first one that doesn't extend to p is the last.
    }
  }
  reverse(sus.begin(), sus.end()); // reverse since we add SUSs from right to left

  // check to the right of position p (after p)
  for(size_t i = p + 1; i < N; i++){
    size_t minimalLength = getMUI(i, rank, lcp, N);
    if(minimalLength > 0){
      std::pair<size_t,size_t> newMUI(i, i + minimalLength - 1);
      if(cover(newMUI, p) < susl){
	sus.clear(); susl = cover(newMUI, p);
      }
      if(cover(newMUI, p) == susl){
	sus.push_back(std::make_pair(std::min(p,newMUI.first),
				     std::max(p,newMUI.second)));
      }
      break; // the first one is the last one.
    }
  }
  return sus;
}

std::pair<size_t,size_t> OSUS::lmSUS(size_t p){
  std::pair<size_t,size_t> I(std::min(p, muInts[S[p]].first),
			     std::max(p, muInts[S[p]].second));
  return(I);
}

// return left most sus only.
std::vector<std::pair<size_t,size_t> > OSUS::lmSUS(){
  std::vector<std::pair<size_t,size_t> > LeftmostSUS;
  size_t N = S.size();
  for(size_t p = 0; p < N; p++){
    std::pair<size_t,size_t> I = muInts[S[p]];
    LeftmostSUS.push_back(std::make_pair(std::min(p, I.first),
					 std::max(p, I.second)));
  }
  return LeftmostSUS;
}
