#pragma once
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>

#include "BackingStore.h"
#include "TLB.h"

class PageTable
{
  private:
    std::map<long, std::pair<long, bool>>
      pt;              // page table; maps page # to <frame #, dirty bit>
    std::set<long> fs; // a set of free frame indices ("frame table")
    std::string pageReplacementPolicy;
    std::list<long> q; // a queue (deque) of used pages, for implementing FIFO
                       // or LRU page replacement
    // BackingStore bs;
    TLB tlb;
    long getFreeFrameNum();
    friend std::ostream& operator<<(std::ostream&, const PageTable&);

  public:
    PageTable(const std::string& pageReplacementPolicy = "FIFO",
              const std::string& backingStorePath = "BACKING_STORE.bin");
    ~PageTable();
    bool pageFault = false; // a bit indicating whether the last page access
                            // resulted in page fault
    long numPageFaults = 0;
    bool tlbMiss = false; // a bit indicating whether the last page access
                          // resulted in TLB miss
    long operator[](const long pnum);
    void setDirty(long pnum, bool dirty) { pt[pnum].second = dirty; }
};
