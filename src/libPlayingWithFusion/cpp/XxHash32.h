
#pragma once
#include <string>
#include <cstdint>

// ──────────────────────────────────────────────────────────────
// xxHash32 – single-header, zero dependencies (public domain / MIT)
// ──────────────────────────────────────────────────────────────
class XXHash32
{
public:
   explicit XXHash32(uint32_t seed);
   bool     Add(const void* input, uint32_t length);
   uint32_t Hash() const;

   static uint32_t    Hash(const void* input, uint32_t length, uint32_t seed);
   static std::string ToHex8(uint32_t v);

private:
   static constexpr uint32_t Prime1 = 2654435761U;
   static constexpr uint32_t Prime2 = 2246822519U;
   static constexpr uint32_t Prime3 = 3266489917U;
   static constexpr uint32_t Prime4 =  668265263U;
   static constexpr uint32_t Prime5 =  374761393U;
   static constexpr uint32_t MaxBufferSize = 16;

   uint32_t      state[4];
   unsigned char buffer[MaxBufferSize];
   unsigned int  bufferSize;
   uint32_t      totalLength;

   static uint32_t RotateLeft(uint32_t x, unsigned char bits);
   static void     Process(const void* data, uint32_t& s0, uint32_t& s1, uint32_t& s2, uint32_t& s3);
};
