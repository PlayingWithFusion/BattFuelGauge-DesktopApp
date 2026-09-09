#pragma once
#include <cstdint>
#include<vector>

struct FlashSegment {
   uint32_t baseAddress;
   uint32_t dataLength;
   uint8_t data[4096];
};

struct FirmwareImage {
   uint32_t version;
   uint32_t totalBytes;
   const std::vector<FlashSegment> segments;
};