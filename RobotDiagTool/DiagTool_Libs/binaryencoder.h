#pragma once

#include <cstddef> /* size_t */
#include <cstdint>

class BinaryEncoder
{
public:
    virtual void Encode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize) = 0;
    virtual void Decode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize) = 0;
};
