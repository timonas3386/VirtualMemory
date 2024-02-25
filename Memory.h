#pragma once
#include <iostream>
/*
Define the page (frame) size, logical and physical memory size; declare
the Memory_tmp array (but not define it).
*/
const unsigned long PHY_ADDRESS_SPACE_BITS = 32;
const unsigned long PHY_ADDRESS_SPACE_SIZE = (1UL << PHY_ADDRESS_SPACE_BITS);
const unsigned long FRAME_SIZE_BITS =
    12;  // lower 12 bits of logical memory address

const unsigned long
    NUM_LOGICAL_MEM_FRAMES =  // logical memory has 48 bits virtual address
                              // frames/pages (each logical address
    (1UL << PHY_ADDRESS_SPACE_BITS);  // has 16 bits)

const unsigned long NUM_PHYSICAL_MEM_FRAMES =
    (1UL << (PHY_ADDRESS_SPACE_BITS -
             FRAME_SIZE_BITS));  // physical memory has 4GB
const unsigned long FRAME_SIZE =
    (1UL << FRAME_SIZE_BITS);  // 2^FRAME_SIZE_BITS, Default is 4k, same as page
                               // size (in number of Lines)
const unsigned long FRAME_OFFSET_MASK =
    FRAME_SIZE - 1UL;  // mask to preserve the lower 12 bits
const unsigned long TLB_NUM =
    64;  // max number of entries in the translation lookaside buffer

const unsigned long LINE_SIZE_BITS = 6;
const unsigned long LINE_SIZE = 1UL << LINE_SIZE_BITS;
const unsigned long LINE_ADDRESS_SPACE_BITS =
    PHY_ADDRESS_SPACE_BITS - LINE_SIZE_BITS;
const unsigned long NUM_LINES = 1UL << LINE_ADDRESS_SPACE_BITS;

class Line;
class Memory
{
public:
    Memory();
    ~Memory();
    bool write(unsigned long addr);
    bool read(unsigned long addr);
    void print();
    friend std::ostream &operator<<(std::ostream &strm, const Memory &mem);

private:
    void update_gap();
    bool copy_line(unsigned long from, unsigned long to);
    unsigned long la_to_pa(unsigned long lnum);
    unsigned long la_to_ia(unsigned long lnum);
    unsigned long ia_to_pa(unsigned long lnum);
    Line *array;
    unsigned long start_line, gap_line;
    unsigned long write_count;
    static unsigned long
        num_line_ex_gap;  // number of lines (Excluding GapLine)
    static unsigned long trigger_times;
};
