#pragma once

class FeistelNetwork
{
public:
    static void init(void);
    static unsigned long cal(unsigned long la);

private:
    static unsigned long f1(unsigned long low_val, unsigned long high_val);
    static unsigned long key1, key2, key3;

    static unsigned long half_length, low_mask, high_mask, addr_space_mask;
};
