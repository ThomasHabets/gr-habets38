/* -*- c++ -*- */
/*
 * Copyright 2019 thomas@habets.se.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <chrono>

#include "sweepsinkv_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace habets38 {

sweepsinkv::sptr sweepsinkv::make(std::string tag, int vlen, float sampleRate)
{
    return gnuradio::get_initial_sptr(new sweepsinkv_impl(tag, vlen, sampleRate));
}


/*
 * The private constructor
 */
sweepsinkv_impl::sweepsinkv_impl(std::string tag, int vlen, float sampleRate)
    : gr::block("sweepsinkv",
                gr::io_signature::make(1, 1, vlen * sizeof(float)),
                gr::io_signature::make(1, 1, 1)),
      d_tag(pmt::intern(tag)),
      d_samprate(sampleRate),
      d_vlen(vlen),
      d_sum(vlen)
{
}

/*
 * Our virtual destructor.
 */
sweepsinkv_impl::~sweepsinkv_impl() {}

void sweepsinkv_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = 1;
}

int sweepsinkv_impl::general_work(int noutput_items,
                                  gr_vector_int& ninput_items,
                                  gr_vector_const_void_star& input_items,
                                  gr_vector_void_star& output_items)
{
    if (!d_buf.empty()) {
        const size_t cnt = std::min(d_buf.size(), (size_t)noutput_items);
        auto from = d_buf.begin();
        auto to = d_buf.begin() + cnt;
        auto out = reinterpret_cast<char*>(output_items[0]);
        std::copy(from, to, out);
        d_buf.erase(from, to);
        return cnt;
    }

    // Don't read more data if buffer is full.
    if (d_buf.size() > d_buf_max) {
        return 0;
    }

    // Convenience variables.
    auto in = reinterpret_cast<const float*>(input_items[0]);
    const size_t inlen = ninput_items[0];
    const auto startN = nitems_read(0);

    // Helper function for consuming data.
    auto consumeN = [&](size_t consume) {
        // std::cout << "consuming " << consume << std::endl;
        for (int j = 0; j < consume; j++) {
            for (int i = 0; i < d_vlen; i++) {
                d_sum[i] += *in++;
            }
        }
        d_count += consume;
        consume_each(consume);
        // Return 0 for convenience. This function never produces output.
        return 0;
    };

    std::vector<tag_t> tags;
    get_tags_in_window(tags, 0, 0, inlen, d_tag);

    if (tags.empty()) {
        // std::cout << "no tags, consuming all " << ninput_items[0] << std::endl;
        return consumeN(inlen);
    }

    // Get tag data.
    const auto& tag = tags[0];
    const auto rel = tag.offset - startN;
    const uint64_t freq = (uint64_t)pmt::to_float(tag.value);

    // If tag is not on the first sample, consume up to tag.
    if (rel > 0) {
        return consumeN(rel);
    }

    // Below: Tag is on first sample.

    // New tag is different from old tag.
    if (freq != d_freq) {
        const std::chrono::seconds sec(1);
        const auto now = std::chrono::system_clock::now();
        const auto uni = (now.time_since_epoch() / sec);
        if (d_freq > 0 && d_count > 0) {
            for (int b = 0; b < d_vlen; b++) {
                const int bucket = b - d_vlen / 2;
                const float bucket_size = static_cast<float>(d_samprate) / d_vlen;
                const auto bfreq = (d_freq + bucket_size * bucket);
                std::stringstream ss;
                ss << uni << " " << static_cast<uint64_t>(bfreq) << " " << std::setw(11)
                   << (d_sum[b] / d_count) << std::endl;
                const std::string s = ss.str();
                d_buf.insert(d_buf.end(), s.begin(), s.end());
            }
        }
        d_sum = std::vector<long double>(d_vlen);
        d_count = 0;
        d_freq = freq;
    }
    return consumeN(1);
}

} /* namespace habets38 */
} /* namespace gr */
