// See https://graphics.stanford.edu/~seander/bithacks.html

INLINE uint8_t fast_log2 (uint8_t);
INLINE uint8_t reverse_bits(uint8_t);

uint8_t fast_log2 (uint8_t value) {
  uint8_t b;
  if (value == 0) value = 1;
  while ((b = value & (value - 1))) value = b;
  return value;
}

uint8_t reverse_bits(uint8_t b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}
