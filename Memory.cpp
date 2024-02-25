#include "Memory.h"
#include <fstream>
#include <iostream>
#include "FeistelNetwork.h"
#include "Line.h"

#ifdef FEISTEL
#define START_GAP
#endif

unsigned long Memory::num_line_ex_gap = NUM_LINES - 1;
unsigned long Memory::trigger_times = 100;

Memory::Memory() : start_line(0), gap_line(NUM_LINES - 1), write_count(0)
{
    array = new Line[NUM_LINES];
    if (!array) {
        std::cerr << "Failed to create class Memory.\n";
        exit(-1);
    }
    FeistelNetwork::init();
}

Memory::~Memory()
{
    delete[] array;
}
/*
    Gap Movement.
*/
void Memory::update_gap()
{
    if (gap_line == 0) {
        start_line = (start_line + 1) % num_line_ex_gap;
        copy_line(num_line_ex_gap, gap_line);
        gap_line = num_line_ex_gap;
    } else {
        copy_line(gap_line - 1, gap_line);
        gap_line -= 1;
    }
}

unsigned long Memory::ia_to_pa(unsigned long inter_line_num)
{
    unsigned long phy_line_num =
        (inter_line_num + start_line) % num_line_ex_gap;
    unsigned long result =
        (phy_line_num >= gap_line) ? phy_line_num + 1 : phy_line_num;
    return result;
}

#ifdef FEISTEL
unsigned long Memory::la_to_ia(unsigned long log_line_num)
{
    return FeistelNetwork::cal(log_line_num);
}
#else
unsigned long Memory::la_to_ia(unsigned long log_line_num)
{
    return log_line_num;
}
#endif

/*
    Mapping of Logical Address to Physical Address.
*/
#ifdef START_GAP
unsigned long Memory::la_to_pa(unsigned long log_line_num)
{
    unsigned long ia = la_to_ia(log_line_num);
    unsigned long pa = ia_to_pa(ia);
    return pa;
}
#else
unsigned long Memory::la_to_pa(unsigned long log_line_num)
{
    return log_line_num;
}
#endif


/*
    Update Memory info
*/
bool Memory::write(unsigned long log_addr)
{
    unsigned long log_lnum = (log_addr >> LINE_SIZE_BITS);
    unsigned long phy_lnum = la_to_pa(log_lnum);
    array[phy_lnum].write();

    write_count++;
    if (write_count >= trigger_times) {
        update_gap();
        write_count = 0;
    }
    return true;
}

bool Memory::copy_line(unsigned long from_line_num, unsigned long to_line_num)
{
    // no need to read now
    array[to_line_num].write();
    return true;
}

void Memory::print()
{
    std::ofstream outfile("line_count.txt");
}

std::ostream &operator<<(std::ostream &strm, const Memory &mem)
{
    for (size_t i = 0; i < NUM_LINES; ++i) {
        strm << mem.array[i].read() << std::endl;
    }
    return strm;
}
