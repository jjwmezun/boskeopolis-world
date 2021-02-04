#include <stdint.h>

__inline__ uint64_t test_speed()
{
    uint32_t lo, hi;
    __asm__ __volatile__( // serialize
                "xorl %%eax,%%eax \n        cpuid":::"%rax",
                "%rbx", "%rcx", "%rdx");
    __asm__ __volatile__("rdtsc":"=a"(lo), "=d"(hi));
    return (uint64_t) hi << 32 | lo;
}