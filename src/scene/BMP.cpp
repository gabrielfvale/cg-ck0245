#include "BMP.hpp"

BMP::BMP(int32_t width, int32_t height) {
  info_header.width = width;
  info_header.height = -1 * height;
  info_header.size = sizeof(BMPInfoHeader);
  file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

  info_header.bit_count = 24;
  info_header.compression = 0;
  row_stride = width * 3;
  data.resize(row_stride * height);

  uint32_t new_stride = make_stride_aligned(4);
  file_header.file_size = file_header.offset_data + data.size() + info_header.height * (new_stride - row_stride);
}

void BMP::set_pixel(int x, int y, int r, int g, int b)
{
  uint32_t channels = info_header.bit_count / 8;
  data[channels * (y * info_header.width + x) + 2] = r;
  data[channels * (y * info_header.width + x) + 1] = g;
  data[channels * (y * info_header.width + x) + 0] = b;
}

void BMP::write(const char *fname) {
  std::ofstream of{ fname, std::ios_base::binary };
  if (of) 
  {
    if (info_header.bit_count == 24)
    {
      if (info_header.width % 4 == 0)
        write_headers_and_data(of);
      else
      {
        uint32_t new_stride = make_stride_aligned(4);
        std::vector<uint8_t> padding_row(new_stride - row_stride);
        write_headers(of);
        for (int y = 0; y < info_header.height; y++)
        {
          of.write((const char*)(data.data() + row_stride * y), row_stride);
          of.write((const char*)padding_row.data(), padding_row.size());
        }
      }
    }
    else
      throw std::runtime_error("BMP file must have 24 bits/pixel");
  }
  else
    throw std::runtime_error("Unable to open the output image file");
}

void BMP::write_headers(std::ofstream &of) {
  of.write((const char*)&file_header, sizeof(file_header));
  of.write((const char*)&info_header, sizeof(info_header));
}

void BMP::write_headers_and_data(std::ofstream &of) {
  write_headers(of);
  of.write((const char*)data.data(), data.size());
}

uint32_t BMP::make_stride_aligned(uint32_t align_stride) {
  uint32_t new_stride = row_stride;
  while (new_stride % align_stride != 0)
    new_stride++;
  return new_stride;
}
