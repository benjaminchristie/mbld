#pragma once 

#include <stdint.h>

uint8_t getBit8t(uint8_t u, uint8_t idx);
uint8_t setOnBit8t(uint8_t u, uint8_t idx);
uint8_t setOffBit8t(uint8_t u, uint8_t idx);

uint8_t getBit16t(uint16_t u, uint8_t idx);
uint8_t setOnBit16t(uint16_t u, uint8_t idx);
uint8_t setOffBit16t(uint16_t u, uint8_t idx);

uint8_t getBit32t(uint32_t u, uint8_t idx);
uint8_t setOnBit32t(uint32_t u, uint8_t idx);
uint8_t setOffBit32t(uint32_t u, uint8_t idx);

uint8_t getBit64t(uint64_t u, uint8_t idx);
uint8_t setOnBit64t(uint64_t u, uint8_t idx);
uint8_t setOffBit64t(uint64_t u, uint8_t idx);
