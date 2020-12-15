/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "despreader_impl.h"
#include <gnuradio/io_signature.h>

#include <cstdlib>
namespace gr {
namespace habets38 {

namespace {
constexpr bool debug = false;

struct Candidate {
  int offset;
  int value = 0;
  Candidate(int offset) : offset(offset) {}
};

int get_candidate(const uint64_t spread_bits, const char *data) { return 0; }

uint64_t str2bits(const std::vector<char> &spread_bits) {
  uint64_t spread = 0;
  for (auto t : spread_bits) {
    spread = (spread << 1) | t;
    // std::clog << "spread: " << std::hex << int(t) << " " << spread <<
    // std::endl;
  }
  return spread;
}
uint64_t str2bits(const char *spread_bits, const int bits) {
  uint64_t spread = 0;
  for (int n = 0; n < bits; n++) {
    spread = (spread << 1) | spread_bits[n];
    // std::clog << "spread: " << std::hex << int(spread_bits[n]) << " " <<
    // spread << std::endl;
  }
  return spread;
}

} // namespace

despreader::sptr despreader::make(const std::vector<char> &spread_bits,
                                  int threshold) {
  return gnuradio::get_initial_sptr(
      new despreader_impl(spread_bits, threshold));
}

/*
 * The private constructor
 */
despreader_impl::despreader_impl(const std::vector<char> &spread_bits,
                                 int threshold)
    : gr::block("despreader", gr::io_signature::make(1, 1, sizeof(char)),
                gr::io_signature::makev(
                    1, 3, {sizeof(char), sizeof(int), sizeof(int)})),
      d_spread_bits(str2bits(spread_bits)),
      d_spread_bits_size(spread_bits.size()),
      d_spread_bits_mask((1 << spread_bits.size()) - 1),
      d_threshold(threshold) {
  set_min_output_buffer(d_spread_bits_size * 2);
  set_history(d_spread_bits_size);
  std::clog << "Spread bits size in despreader: " << d_spread_bits_size
            << std::endl;
}

/*
 * Our virtual destructor.
 */
despreader_impl::~despreader_impl() {}
int despreader_impl::general_work(int noutput_items,
                                  gr_vector_int &ninput_items,
                                  gr_vector_const_void_star &input_items,
                                  gr_vector_void_star &output_items) {
  auto in = static_cast<const char *>(input_items[0]);
  auto out = static_cast<char *>(output_items[0]);
  auto error =
      output_items.size() >= 2 ? static_cast<int *>(output_items[1]) : nullptr;
  auto skip =
      output_items.size() >= 3 ? static_cast<int *>(output_items[2]) : nullptr;

  const auto input_count = ninput_items[0];
  // auto drops = static_cast<char*>(output_items[1]);
  if (debug) {
    std::clog << "Called with in=" << input_count << " out=" << noutput_items
              << std::endl;
  }
  int threshold = d_spread_bits_size / 4;
  int ofs = 0;
  int out_ofs = 0;
  for (; ofs < input_count - d_spread_bits_size &&
         out_ofs < noutput_items - d_spread_bits_size;) {
    const auto inbits = str2bits(&in[ofs], d_spread_bits_size);
    const auto xored = (d_spread_bits ^ inbits);
    const auto bits_set = __builtin_popcount(xored);
    const auto score = abs(bits_set - d_spread_bits_size / 2);
    const char found_bit = bits_set > d_spread_bits_size / 2;
    const int err = [&] {
      if (found_bit) {
        return d_spread_bits_size - bits_set;
      } else {
        return bits_set;
      }
    }();
    if (debug) {
      std::clog << "... data" << std::hex << " inbits=" << inbits
                << " spread_bits=" << d_spread_bits << " xored: " << xored
                << " bitsset: " << bits_set << " score: " << score
                << " err: " << err << std::endl;
    }
    if (err > d_threshold) {
      if (debug) {
        std::clog << " bad score at " << ofs << std::endl;
      }
      ofs++;
      d_skip++;
      continue;
    }
    if (debug) {
      std::clog << "... Good at " << std::dec << ofs << " err=" << err
                << std::endl;
    }
    *out++ = found_bit;
    if (error) {
      *error++ = err;
    }
    if (skip) {
      *skip++ = d_skip;
      d_skip = 0;
    }
    out_ofs++;
    ofs += d_spread_bits_size;
  }
  if (debug) {
    std::clog << std::dec << "Consuming " << (ofs + 1) << " output " << out_ofs
              << std::endl;
  }
  consume_each(ofs);
  return out_ofs;
}

#if 0
    int
    despreader_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
            auto in = static_cast<const char*>(input_items[0]);
            auto out = static_cast<char*>(output_items[0]);

            std::clog << "Generating " << noutput_items << " items\n";
      // Do <+signal processing+>

            for (int n = 0; n < noutput_items; n++) {
                    const auto inbits = str2bits(&in[0], d_spread_bits_size+d_spread_bits_size-1);

                    std::clog << "INPUT:  " << std::hex << inbits << std::endl;
                    std::clog << "SPREAD: " << std::hex << d_spread_bits << std::endl;

                    int best_ofs = 0;
                    int best_score = 0;
                    bool best_value = false;
                    for (int ofs = d_spread_bits_size-1; ofs >=0; ofs--) {
                            const auto xored = (d_spread_bits ^ (inbits>>(d_spread_bits_size-1-ofs))) & d_spread_bits_mask;
                            const auto bits_set = __builtin_popcount(xored);
                            const auto score = abs(bits_set - d_spread_bits_size/2);
                            if (score > best_score) {
                                    best_score = score;
                                    best_ofs = ofs;
                                    best_value = bits_set > d_spread_bits_size/2;
                            }
                    }
                    std::clog << "BEST: " << std::hex << best_value << " " << best_ofs << std::endl;
                    out[n] = in[decimation() * n];

                    return 1;
            }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
#endif
} /* namespace habets38 */
} /* namespace gr */
