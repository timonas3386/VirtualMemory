#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "BackingStore.h"
using namespace std;

/*
Read page at pnum (0 based index) into buffer
*/
void BackingStore::read(long pnum) {
    assert(!store.eof());

    // seek to the page at pnum
    if (store.seekg(pnum * FRAME_SIZE, ios::beg))
        store.read((char *)buff, FRAME_SIZE);
    else
        throw runtime_error("Seek error");
}

/*
Write the physical memory frame at fnum to the page at pnum on backing store
*/
void BackingStore::write(long pnum, long fnum) {
    assert(!store.eof());

    // seek to the page at pnum
    if (store.seekg(pnum * FRAME_SIZE, ios::beg))
        ; // store.write((char*)Memory_tmp[fnum], FRAME_SIZE);
    else
        throw runtime_error("Seek error");
}
