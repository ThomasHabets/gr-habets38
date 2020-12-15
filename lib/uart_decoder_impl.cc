/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "uart_decoder_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace habets38 {

namespace {
char calc_parity(const char *data, int len) noexcept {
  char ch = 0;
  for (int bit = 0; bit < len; bit++) {
    ch ^= data[bit];
  }
  return ch;
}
} // namespace

uart_decoder::sptr uart_decoder::make(int start, int bits, int parity,
                                      int stop) {
  return gnuradio::get_initial_sptr(
      new uart_decoder_impl(start, bits, parity, stop));
}

/*
 * The private constructor
 */
uart_decoder_impl::uart_decoder_impl(int start, int bits, int par, int stop)
    : gr::block("uart_decoder", gr::io_signature::make(1, 1, sizeof(char)),
                gr::io_signature::make(1, 1, sizeof(char))),
      d_start(start), d_bits(bits), d_parity(parity(par)), d_stop(stop) {
  set_history(d_start + d_bits + parity_size() + d_stop);
  set_relative_rate(1, block_size());
}

/*
 * Our virtual destructor.
 */
uart_decoder_impl::~uart_decoder_impl() {}

void uart_decoder_impl::forecast(int noutput_items,
                                 gr_vector_int &ninput_items_required) {
  /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

int uart_decoder_impl::general_work(int noutput_items,
                                    gr_vector_int &ninput_items,
                                    gr_vector_const_void_star &input_items,
                                    gr_vector_void_star &output_items) {
  auto in = static_cast<const char *>(input_items[0]);
  const auto input_size = ninput_items[0];
  auto out = static_cast<char *>(output_items[0]);
  auto output_left = noutput_items;

  int ofs = 0;
  for (; ofs < input_size && output_left > 0;) {
    // Check for start.
    if (in[ofs] != 0) {
      ofs++;
      continue;
    }
    // Check for stop.
    if (in[ofs + d_start + d_bits + parity_size()] != 1) {
      ofs++;
      continue;
    }

    // Check parity.
    const char par = in[ofs + d_start + d_bits];

    switch (d_parity) {
    case parity::none:
      // actually holding the stop bit.
      break;
    case parity::odd:
      if (calc_parity(&in[ofs + d_start], d_bits) != par) {
        ofs++;
        continue;
      }
      break;
    case parity::even:
      if (calc_parity(&in[ofs + d_start], d_bits) == par) {
        ofs++;
        continue;
      }
      break;
    }

    *out = (in[ofs + d_start] ? 1 : 0) | (in[ofs + d_start + 1] ? 2 : 0) |
           (in[ofs + d_start + 2] ? 4 : 0) | (in[ofs + d_start + 3] ? 8 : 0) |
           (in[ofs + d_start + 4] ? 16 : 0) | (in[ofs + d_start + 5] ? 32 : 0) |
           (in[ofs + d_start + 6] ? 64 : 0) | (in[ofs + d_start + 7] ? 128 : 0);
    out++;
    output_left--;
    ofs += d_start + d_bits + parity_size() + d_stop;
  }
  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each(ofs);

  // Tell runtime system how many output items we produced.
  return noutput_items - output_left;
}

} /* namespace habets38 */
} /* namespace gr */
