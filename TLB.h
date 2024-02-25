#pragma once
#include "Memory.h"
#include <list>
#include <map>
/*
Translation lookaside buffer, a cache for page table
(not really needed in this page table implementation, just for demo purpose)
*/
class TLB {
  private:
    std::map<long, long> tb; // underlying table mapping page number to frame
                             // number; will be a subset of page table
    std::list<long> q;       // TLB has its own queue of used pages and implements LRU
  public:
    long &operator[](const long pnum);
    long contains(const long pnum) { return tb.count(pnum); }
};
