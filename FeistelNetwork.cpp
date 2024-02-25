#include "FeistelNetwork.h"
#include <random>
#include "Memory.h"

unsigned long FeistelNetwork::key1 = 0;
unsigned long FeistelNetwork::key2 = 0;
unsigned long FeistelNetwork::key3 = 0;
unsigned long FeistelNetwork::half_length = 0;
unsigned long FeistelNetwork::low_mask = 0;
unsigned long FeistelNetwork::high_mask = 0;
unsigned long FeistelNetwork::addr_space_mask = 0;

unsigned long FeistelNetwork::f1(unsigned long a, unsigned long k)
{
    unsigned long tmp1 = a ^ k;
    unsigned long tmp2 = tmp1 * tmp1;
    return tmp2 & addr_space_mask;
}

void FeistelNetwork::init(void)
{
    unsigned long max_size = 1 << LINE_ADDRESS_SPACE_BITS;
    std::default_random_engine generator(time(NULL));
    std::uniform_int_distribution<unsigned long> dist(max_size / 2, max_size);

    addr_space_mask = (1 << LINE_ADDRESS_SPACE_BITS) - 1;
    half_length = LINE_ADDRESS_SPACE_BITS / 2;
    low_mask = (1 << half_length) - 1;
    high_mask = ~low_mask;
    key1 = dist(generator);
    key2 = dist(generator);
    key3 = dist(generator);
}

unsigned long FeistelNetwork::cal(unsigned long la)
{
    unsigned long l0, l1, l2, l3, r0, r1, r2, r3, ia;
    // prevent from overflow
    la &= addr_space_mask;
    l0 = la & low_mask;
    r0 = (la & high_mask) >> half_length;
    // stage1
    r1 = f1(key1, l0) ^ r0;
    l1 = l0;
    // stage2
    r2 = r1;
    l2 = f1(r1, key2) ^ l1;
    // stage3
    r3 = f1(l2, key3) ^ r2;
    l3 = l2;
    ia = ((l3 << half_length) | r3) & addr_space_mask;
    return ia;
}
