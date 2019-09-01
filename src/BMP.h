#pragma once
#include <fstream>
#include <vector>
#include <stdexcept>

#pragma pack(push, 1)
struct BMPFileHeader {
  uint16_t file_type{0x4D42};
  uint32_t file_size{0};
  uint16_t reserved1{0};
  uint16_t reserved2{0};
  uint32_t offset_data{0};
};

struct BMPInfoHeader {
  uint32_t size{0};
  int32_t width{0};
  int32_t height{0}; // negative values for top-down
  uint16_t planes{1};
  uint16_t bit_count{0}; // bits/pixel
  uint32_t compression{0}; // uncompressed
  uint32_t size_image{0};
  int32_t x_pixels_per_meter{0};
  int32_t y_pixels_per_meter{0};
  uint32_t colors_used{0};
  uint32_t colors_important{0};
};
#pragma pack(pop)

#ifndef BMP_H
#define BMP_H

class BMP {
  private:
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    std::vector<uint8_t> data;
    uint32_t row_stride{0};
    void write_headers(std::ofstream &of);
    void write_headers_and_data(std::ofstream &of);
    uint32_t make_stride_aligned(uint32_t align_stride);

  public:
    BMP(int32_t width, int32_t height);
    void set_pixel(int x, int y, int r, int g, int b);
    void write(const char *fname);
};

#endif
