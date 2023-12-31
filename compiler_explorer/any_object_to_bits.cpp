
#include <string>
#include <cstddef>
#include <cstdint>
#include <cstring>

template <class T> int v1(const T &v) {
  int c =0;
  std::byte storage[sizeof(T)];
  memcpy((void *)storage, &v, sizeof(v));

  std::byte base{1u << 7u};
  for (size_t i = 0; i < sizeof(T) * 8; ++i) {
    uint8_t byte_idx = i / 8;
    uint8_t bit_idx = i % 8;
    char bit = ((storage[byte_idx] & (base >> bit_idx)) == std::byte{0}) ? '0' : '1';
    if (bit == '1') ++c;
  }

  return c;
}



void f() {
    volatile int v = v1(1);
}