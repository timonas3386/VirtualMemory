#include "PageTable.h"
#include "Memory.h"
#include "TLB.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

PageTable::PageTable(const string &pageReplacementPolicy,
                     const string &backingStorePath) {
    if (!(pageReplacementPolicy == "FIFO" || pageReplacementPolicy == "LRU"))
        throw invalid_argument("Page replacement policy must be FIFO or LRU");
    else
        this->pageReplacementPolicy = pageReplacementPolicy;

    // initialize free frames set; at first all frames are free
    for (size_t j = 0; j < NUM_PHYSICAL_MEM_FRAMES; j++)
        fs.insert(fs.end(), j);
}

PageTable::~PageTable() {
}

long PageTable::operator[](const long pnum) {
    /*
    Get the number (index) of the frame in physical memory corresponding to
    given page number (pnum) in logical memory; this is done prior to every
    access to Memory_tmp. Page faults are resolved automatically.
    */
    long fnum;
    std::map<long, std::pair<long, bool>>::iterator itr;

    pageFault = tlbMiss = false;
    fnum = -1;

    if (tlb.contains(pnum)) // look in TLB first
        fnum = tlb[pnum];
    else { // TLB miss; look in page table
        tlbMiss = true;
        if ((itr = pt.find(pnum)) != pt.end()) {
            fnum = itr->second.first;
        } else {
            // page fault!
            pageFault = true;
            numPageFaults++;
            // read the requested page from backing store into a free frame
            fnum = getFreeFrameNum(); // somehow find a free frame
            pt[pnum] = make_pair(fnum, false);
        }

        tlb[pnum] = fnum; // update TLB
    }

    if (pageFault) {
        // with either FIFO or LRU, if a page is newly brought into memory, its
        // index is inserted at tail of queue
        q.push_back(pnum);
    } else {
        // in the case of no page fault, FIFO does nothing;
        // LRU will move the recently accessed page number to the tail of queue
        if (pageReplacementPolicy == "LRU") {
            q.remove(pnum);
            q.push_back(pnum);
        }
    }

    return fnum;
}

long PageTable::getFreeFrameNum() {
    /*
    Find a free frame and return its frame number
    */
    long fnum;
    if (!fs.empty()) {
        fnum = *fs.begin(); // always get the free frame with the smallest frame
                            // number
        fs.erase(fnum);     // remove fnum from the set of free frames
    } else {                // need to replace a page, q is full (since we don't delete a page
        // by itself, fs.size() == 64 - q.size())
        // either FIFO or LRU replaces the page at the front of the queue
        long pnum = q.front(); // victim page number
        q.pop_front();
        fnum = pt[pnum].first;

        // if a dirty page, write its memory frame to backing store
        if (pt[pnum].second) {
            // bs.write(pnum, fnum);
            setDirty(pnum, false); // reset dirty bit
        }

        pt[pnum].first = -1; // reset victim page's frame number
    }
    return fnum;
}

// print all valid entries of page table, page # followed by frame #
ostream &
operator<<(ostream &strm, const PageTable &PT) {
    for (auto const &kv : PT.pt) {
        if (kv.second.first != -1)
            strm << kv.first << ' ' << kv.second.first << endl;
    }
    return strm;
}
