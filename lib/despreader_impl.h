/* -*- c++ -*- */

#ifndef INCLUDED_HABETS38_DESPREADER_IMPL_H
#define INCLUDED_HABETS38_DESPREADER_IMPL_H

#include <habets38/despreader.h>

namespace gr {
namespace habets38 {

class despreader_impl : public despreader {
private:
  const uint64_t d_spread_bits;
  const int d_spread_bits_size;
  const uint64_t d_spread_bits_mask;
  const int d_threshold;
  uint64_t d_skip = 0;

public:
  despreader_impl(const std::vector<char> &spread_bits, int threshold);
  ~despreader_impl();

  int general_work(int noutput_items, gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items) override;
};

} // namespace habets38
} // namespace gr

#endif /* INCLUDED_HABETS38_DESPREADER_IMPL_H */
