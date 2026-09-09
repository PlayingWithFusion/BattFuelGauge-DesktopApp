#include "XxHash32.h"


XXHash32::XXHash32(uint32_t seed)
{
   state[0] = seed + Prime1 + Prime2;
   state[1] = seed + Prime2;
   state[2] = seed;
   state[3] = seed - Prime1;

   bufferSize   = 0;
   totalLength = 0;
}

bool XXHash32::Add(const void* input, uint32_t length)
{
   if (!input || length == 0) 
   {
      return false;
   }

   totalLength += length;
   const unsigned char* data = static_cast<const unsigned char*>(input);
   if (bufferSize + length < MaxBufferSize)
   {
      while (length-- > 0) 
      {
         buffer[bufferSize++] = *data++;
      }
      return true;
   }

   const unsigned char* stop = data + length;
   const unsigned char* stopBlock = stop - MaxBufferSize;
   if (bufferSize > 0)
   {
      while (bufferSize < MaxBufferSize)
      {
         buffer[bufferSize++] = *data++;
      }
      Process(buffer, state[0], state[1], state[2], state[3]);
   }

   uint32_t s0 = state[0], s1 = state[1], s2 = state[2], s3 = state[3];
   while (data <= stopBlock)
   {
      Process(data, s0, s1, s2, s3);
      data += 16;
   }

   state[0] = s0; state[1] = s1; state[2] = s2; state[3] = s3;
   bufferSize = (uint32_t)(stop - data);
   for (unsigned int i = 0; i < bufferSize; ++i) 
   {
      buffer[i] = data[i];
   }

   return true;
}

uint32_t XXHash32::Hash() const
{
   uint32_t result = static_cast<uint32_t>(totalLength);
   if (totalLength >= MaxBufferSize)
   {
      result += RotateLeft(state[0], 1) + RotateLeft(state[1], 7) +
                     RotateLeft(state[2],12) + RotateLeft(state[3],18);
   }
   else
   {
      result += state[2] + Prime5;
   }

   const unsigned char* data = buffer;
   const unsigned char* stop = data + bufferSize;
   for (; data + 4 <= stop; data += 4)
   {
      result = RotateLeft(result + *reinterpret_cast<const uint32_t*>(data) * Prime3, 17) * Prime4;
   }
   
   while (data != stop)
   {
      result = RotateLeft(result + (*data++) * Prime5, 11) * Prime1;
   }

   result ^= result >> 15; result *= Prime2;
   result ^= result >> 13; result *= Prime3;
   result ^= result >> 16;
   return result;
}

uint32_t XXHash32::Hash(const void* input, uint32_t length, uint32_t seed)
{
   XXHash32 hasher(seed);
   hasher.Add(input, length);
   return hasher.Hash();
}

std::string XXHash32::ToHex8(uint32_t v)
{
   char buf[9];
   const char* hex = "0123456789abcdef";
   for (int i = 7; i >= 0; --i)
   {
         buf[i] = hex[v & 0xF];
         v >>= 4;
   }
   buf[8] = '\0';
   return buf;
}

uint32_t XXHash32::RotateLeft(uint32_t x, unsigned char bits)
{
   return (x << bits) | (x >> (32 - bits));
}

void XXHash32::Process(const void* data, uint32_t& s0, uint32_t& s1, uint32_t& s2, uint32_t& s3)
{
   const uint32_t* block = static_cast<const uint32_t*>(data);
   s0 = RotateLeft(s0 + block[0] * Prime2, 13) * Prime1;
   s1 = RotateLeft(s1 + block[1] * Prime2, 13) * Prime1;
   s2 = RotateLeft(s2 + block[2] * Prime2, 13) * Prime1;
   s3 = RotateLeft(s3 + block[3] * Prime2, 13) * Prime1;
}
