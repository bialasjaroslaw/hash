#pragma once

#include <hash/md5-constants.h>

#include <bit>
#include <cstring>

constexpr uint32_t F(uint32_t X, uint32_t Y, uint32_t Z) {
  return (X & Y) | (~X & Z);
}
constexpr uint32_t G(uint32_t X, uint32_t Y, uint32_t Z) {
  return (X & Z) | (Y & ~Z);
}
constexpr uint32_t H(uint32_t X, uint32_t Y, uint32_t Z) { 
    return X ^ Y ^ Z; 
}
constexpr uint32_t I(uint32_t X, uint32_t Y, uint32_t Z) {
  return Y ^ (X | ~Z);
}

constexpr uint32_t rotate_left(uint32_t x, uint32_t n) {
  return (x << n) | (x >> (32 - n));
}

template <typename D, typename S>
void constexpr byteswap(D *dst, const S *src) {
  memcpy(dst, src, 4);
  if constexpr (std::endian::native == std::endian::big) {
    std::swap(dst + 0, dst + 3);
    std::swap(dst + 1, dst + 2);
  }
}

class md5_hasher {
 public:
  md5_hasher() = default;

  void update(const uint8_t *input_buffer, uint64_t input_len) {
    uint32_t input[16];
    uint64_t offset = _size % 64;
    _size += input_len;
    for (uint64_t i = 0; i < input_len; ++i) {
      _input[offset++] = *(input_buffer + i);
      if (offset % 64 == 0) {
        for (unsigned int j = 0; j < 16; ++j) {
          byteswap(input + j, _input + j * 4);
        }
        step(_buffer, input);
        offset = 0;
      }
    }
  }
  void finalize() {
    uint32_t input[16];
    uint64_t offset = _size % 64;
    uint64_t padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;
    update(PADDING, padding_length);
    _size -= padding_length;

    for (unsigned int j = 0; j < 14; ++j) {
      byteswap(input + j, _input + j * 4);
    }
    input[14] = static_cast<uint32_t>(_size * 8);
    input[15] = static_cast<uint32_t>((_size * 8) >> 32);

    step(_buffer, input);

    for (unsigned int i = 0; i < 4; ++i) {
      byteswap(_digest + i * 4, reinterpret_cast<uint8_t *>(_buffer + i));
    }
  }

template <typename FN>
constexpr void fun(FN f, uint32_t& w, uint32_t x, uint32_t y, uint32_t z, uint32_t in, uint32_t s)
{
    w += f(x, y, z) + in;
    w = rotate_left(w, s) + x;
}

void step(uint32_t *buffer, const uint32_t *input) {
    uint32_t a = buffer[0];
    uint32_t b = buffer[1];
    uint32_t c = buffer[2];
    uint32_t d = buffer[3];

    fun(F, a, b, c, d, input[ 0] + K[0], S[0]);
    fun(F, d, a, b, c, input[ 1] + K[1], S[1]);
    fun(F, c, d, a, b, input[ 2] + K[2], S[2]);
    fun(F, b, c, d, a, input[ 3] + K[ 3], S[ 3]);
    fun(F, a, b, c, d, input[ 4] + K[ 4], S[ 4]);
    fun(F, d, a, b, c, input[ 5] + K[ 5], S[ 5]);
    fun(F, c, d, a, b, input[ 6] + K[ 6], S[ 6]);
    fun(F, b, c, d, a, input[ 7] + K[ 7], S[ 7]);
    fun(F, a, b, c, d, input[ 8] + K[ 8], S[ 8]);
    fun(F, d, a, b, c, input[ 9] + K[ 9], S[ 9]);
    fun(F, c, d, a, b, input[10] + K[10], S[10]);
    fun(F, b, c, d, a, input[11] + K[11], S[11]);
    fun(F, a, b, c, d, input[12] + K[12], S[12]);
    fun(F, d, a, b, c, input[13] + K[13], S[13]);
    fun(F, c, d, a, b, input[14] + K[14], S[14]);
    fun(F, b, c, d, a, input[15] + K[15], S[15]);

    fun(G, a, b, c, d, input[1] + K[16], S[16]);
    fun(G, d, a, b, c, input[6] + K[17], S[17]);
    fun(G, c, d, a, b, input[11] + K[18], S[18]);
    fun(G, b, c, d, a, input[0] + K[19], S[19]);
    fun(G, a, b, c, d, input[5] + K[20], S[20]);
    fun(G, d, a, b, c, input[10] + K[21], S[21]);
    fun(G, c, d, a, b, input[15] + K[22], S[22]);
    fun(G, b, c, d, a, input[4] + K[23], S[23]);
    fun(G, a, b, c, d, input[9] + K[24], S[24]);
    fun(G, d, a, b, c, input[14] + K[25], S[25]);
    fun(G, c, d, a, b, input[3] + K[26], S[26]);
    fun(G, b, c, d, a, input[8] + K[27], S[27]);
    fun(G, a, b, c, d, input[13] + K[28], S[28]);
    fun(G, d, a, b, c, input[2] + K[29], S[29]);
    fun(G, c, d, a, b, input[7] + K[30], S[30]);
    fun(G, b, c, d, a, input[12] + K[31], S[31]);

    fun(H, a, b, c, d, input[5] + K[32], S[32]);
    fun(H, d, a, b, c, input[8] + K[33], S[33]);
    fun(H, c, d, a, b, input[11] + K[34], S[34]);
    fun(H, b, c, d, a, input[14] + K[35], S[35]);
    fun(H, a, b, c, d, input[1] + K[36], S[36]);
    fun(H, d, a, b, c, input[4] + K[37], S[37]);
    fun(H, c, d, a, b, input[7] + K[38], S[38]);
    fun(H, b, c, d, a, input[10] + K[39], S[39]);
    fun(H, a, b, c, d, input[13] + K[40], S[40]);
    fun(H, d, a, b, c, input[0] + K[41], S[41]);
    fun(H, c, d, a, b, input[3] + K[42], S[42]);
    fun(H, b, c, d, a, input[6] + K[43], S[43]);
    fun(H, a, b, c, d, input[9] + K[44], S[44]);
    fun(H, d, a, b, c, input[12] + K[45], S[45]);
    fun(H, c, d, a, b, input[15] + K[46], S[46]);
    fun(H, b, c, d, a, input[2] + K[47], S[47]);

    fun(I, a, b, c, d, input[0] + K[48], S[48]);
    fun(I, d, a, b, c, input[7] + K[49], S[49]);
    fun(I, c, d, a, b, input[14] + K[50], S[50]);
    fun(I, b, c, d, a, input[5] + K[51], S[51]);
    fun(I, a, b, c, d, input[12] + K[52], S[52]);
    fun(I, d, a, b, c, input[3] + K[53], S[53]);
    fun(I, c, d, a, b, input[10] + K[54], S[54]);
    fun(I, b, c, d, a, input[1] + K[55], S[55]);
    fun(I, a, b, c, d, input[8] + K[56], S[56]);
    fun(I, d, a, b, c, input[15] + K[57], S[57]);
    fun(I, c, d, a, b, input[6] + K[58], S[58]);
    fun(I, b, c, d, a, input[13] + K[59], S[59]);
    fun(I, a, b, c, d, input[4] + K[60], S[60]);
    fun(I, d, a, b, c, input[11] + K[61], S[61]);
    fun(I, c, d, a, b, input[2] + K[62], S[62]);
    fun(I, b, c, d, a, input[9] + K[63], S[63]);
    buffer[0] += a;
    buffer[1] += b;
    buffer[2] += c;
    buffer[3] += d;
}

  static void from_string(const char *input, uint8_t *result) {
    md5_hasher hasher;
    hasher.update(reinterpret_cast<const uint8_t *>(input), strlen(input));
    hasher.finalize();
    memcpy(result, hasher._digest, 16);
  }
  void result(uint8_t *result) { memcpy(result, _digest, 16); }
  virtual ~md5_hasher() = default;

 private:
  uint64_t _size = 0;
  uint32_t _buffer[4] = {A, B, C, D};
  uint8_t _input[64] = {};
  uint8_t _digest[16] = {};
};