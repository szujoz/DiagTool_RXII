// https://github.com/osannolik/calmeas/blob/master/target/stm32f4-example/Inc/crc8.h
#pragma once

#include <cstdint>

#define CRC8_8         (0)
#define CRC8_MAXIM     (1)
#define CRC8_SAE_J1850 (2)
#define CRC8_AUTOSAR   (3)
#define CRC8_CDMA2000  (4)

#define CRC8_USED_POLY CRC8_MAXIM

class Crc8
{
public:
	Crc8();
	
	void PushByte(const uint8_t byte);
	uint8_t GetCrc();
	
	static uint8_t GetBlockCrc(const uint8_t data[], size_t len);
    static bool CheckBlockCrc(const uint8_t data[], size_t len);
	
private:
	uint8_t crc;
};
