//
// main.cpp: by Hideo Bannai and Kazuya Tsuruta
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
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <cassert>
#include <time.h>
#include "osus.hpp"
#include "divsufsort.h"
#include <unistd.h>

void print_usage(int argc, char * argv []){
  std::cout << "Usage  : " << argv[0] << " [options]" << std::endl
	    << "Options: " << std::endl
	    << "  -f iFile : file to process" << std::endl
            << "  -n int   : truncate input string to size" << std::endl
	    << "  -q int   : query position (output all if negative (default))" << std::endl
	    << "  -l       : only output leftmost SUS" << std::endl
	    << "  -c       : (DEBUG) check results against semi-naive algorithm" << std::endl
	    << "  -h       : usage"
	    << std::endl;    
  return;
}

// read string from file (with limit N, and N = 0 for no limit) 
void stringFromFile(const std::string & fileName, std::string & s, size_t N = 0){
  struct stat st;
  size_t fileSize;
  if(stat(fileName.c_str(), &st)){
    std::cerr << "failed to stat file: " << fileName << std::endl;
    return;
  }
  std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary);
  if(!ifs){
    std::cerr << "failed to read file: " << fileName << std::endl;
    return;
  }
  if(N > 0) fileSize = N;
  else fileSize = st.st_size;
  if(fileSize != static_cast<size_t>(static_cast<int>(fileSize))){
    std::cerr << "ERROR: The file size is too big to fit in int. Cannot process." << std::endl;
    return;
  }
  s.resize(fileSize);
  ifs.read(reinterpret_cast<char*>(&s[0]), fileSize);
}

int main(int argc, char * argv[]){
  int ch;

  bool help = false, check = false, leftmost = false;
  std::string inFile, queryFile, s;
  std::string fileName;
  size_t N = 0;
  int query = -1;

  while ((ch = getopt(argc, argv, "f:chn:q:l")) != -1) {
    switch(ch){
    case 'f':
      inFile = optarg; break;
    case 'h':
      help = true; break;
    case 'c': // check
      check = true; break;
    case 'n':
      N = atoi(optarg); break;
    case 'q':
      query = atoi(optarg); break;
    case 'l':
      leftmost = true; break;
    }
  }
  if(help || inFile.empty()){
    print_usage(argc, argv);
    return -1;
  }

  stringFromFile(inFile, s, N);

  std::cerr << "input string length = " << s.size() << std::endl;

  OSUS susinfo(s);

  if(check){
    // code for debug //////////////////////////////////////////////
    std::cerr << "checking results against naive..." << std::flush;
    for(size_t p = 0; p < s.size(); p++){
      std::vector<std::pair<size_t,size_t> > 
	sus1 = susinfo.query(p), sus2 = susinfo.query_naive(p);
      if(sus1 != sus2){
	std::cout << " MISMATCH!" << std::endl;
	std::cout << "OSUS : ";
	for(size_t i = 0; i < sus1.size(); i++){
	  std::cout << "[" << sus1[i].first << "," << sus1[i].second << "];";
	}
	std::cout << std::endl;
	std::cout << "NAIVE: ";
	for(size_t i = 0; i < sus2.size(); i++){
	  std::cout << "[" << sus2[i].first << "," << sus2[i].second << "];";
	}
	std::cout << std::endl;
	return 1;     	
      }
    }
    std::cerr << "results OK!" << std::endl;
  } else if(query >= 0){
    if(leftmost == true){
      std::pair<size_t,size_t> lmsus = susinfo.lmSUS(query);
      std::cout << "leftmost SUS at position: " << query << ": ";
      std::cout << "[" << lmsus.first << "," << lmsus.second << "];" << std::endl;
    } else {
      std::vector<std::pair<size_t,size_t> > sus = susinfo.query(query);
      std::cout << "SUSs at position: " << query << ": ";
      for(size_t i = 0; i < sus.size(); i++){
	std::cout << "[" << sus[i].first << "," << sus[i].second << "];";
      }
      std::cout << std::endl;
    }
  } else {
    if(leftmost == true){
      std::vector<std::pair<size_t,size_t> > sus = susinfo.lmSUS();
      for(size_t p = 0; p < s.size(); p++){
	std::cout << p << ": [" << sus[p].first << "," << sus[p].second << "];" << std::endl;
      }
    } else {
      for(size_t p = 0; p < s.size(); p++){
	std::vector<std::pair<size_t,size_t> > sus = susinfo.query(p);
	std::cout << p << ": ";
	for(size_t i = 0; i < sus.size(); i++){
	  std::cout << "[" << sus[i].first << "," << sus[i].second << "];";
	  if(leftmost == true) break;
	}
	std::cout << std::endl;
      }
    }
  }
  return 0;
}
