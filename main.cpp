#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Memory.h"
#include "PageTable.h"
#include "BuddyAllocator.h"

using namespace std;

int main(int argc, char **argv)
{
    // create page table
    PageTable T(strcmp(argv[1], "-F") == 0 ? "FIFO" : "LRU");

    // open input file
    ifstream input(argv[2]);
    if (!input) {
        cerr << "Unable to open input file\n";
        return -1;
    }

    unsigned long addr, pnum, offset, fnum;
    Memory mem;
    for (string line; getline(input, line);) {
        addr = stoul(line, nullptr, 0);

        pnum = addr >> FRAME_SIZE_BITS;     // page number
        offset = addr & FRAME_OFFSET_MASK;  // page offset

        // access the page and get its frame number
        fnum = T[pnum];

        T.setDirty(pnum, true);
        mem.write((fnum << FRAME_SIZE_BITS) | offset);
        // cerr << "Bad operation: " << op << endl;

        // output format: page #, offset, TLB hit, page fault, physical address,
        // value (only for reads)
        //        cout << pnum << ' ' << offset << ' ' << (!T.tlbMiss ? 'H' :
        //        'N') << ' '
        //            << (T.pageFault ? 'F' : 'N') << ' '
        //             << ((fnum << FRAME_SIZE_BITS) | offset);
        //        cout << endl;
    }
    ofstream out(argv[3]);

    out << mem << endl;
    out.close();
    input.close();
}
