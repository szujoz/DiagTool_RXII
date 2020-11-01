#pragma once

#include "BinaryEncoder.h"

class EscapeEncoder : public BinaryEncoder
{
public:
	virtual void Encode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize) override;
	virtual void Decode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize) override;
};
