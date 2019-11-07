/* -*- c++ -*- */
/*
 * Copyright 2019 Thomas Habets <thomas@habets.se>.
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

#include <numeric>
#include <chrono>

#include <gnuradio/io_signature.h>
#include "sweepsink_impl.h"

namespace gr {
  namespace habets38 {

    sweepsink::sptr
    sweepsink::make(const std::string& label)
    {
      return gnuradio::get_initial_sptr
        (new sweepsink_impl(label));
    }


    /*
     * The private constructor
     */
          sweepsink_impl::sweepsink_impl(const std::string& label)
      : gr::sync_block("sweepsink",
              gr::io_signature::make(1, 1, sizeof(float)),
                       gr::io_signature::make(0, 0, 0)),
        d_label(pmt::intern(label))
    {}

    /*
     * Our virtual destructor.
     */
    sweepsink_impl::~sweepsink_impl()
    {
    }

    int
    sweepsink_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      const auto startN = nitems_read(0);
      /////////////

      std::vector<tag_t> tags;
      get_tags_in_window(tags, 0, 0, noutput_items, d_label);
      if (tags.empty()) {
              //std::cout << "no tag!\n";
              d_count += noutput_items;
              d_sum = std::accumulate(&in[0], &in[noutput_items], d_sum);
              return noutput_items;
      }

      // Get data.
      const auto& tag = tags[0];
      const auto rel = tag.offset - startN;
      const uint64_t value = (uint64_t)pmt::to_float(tag.value);
      //std::cout << "tag at " << rel << std::endl;
      if (rel > 0) {
              d_count += rel;
              d_sum = std::accumulate(&in[0], &in[rel], d_sum);
              return rel;
      }

      if (value != d_value) {
              const std::chrono::seconds sec(1);
              const auto now = std::chrono::system_clock::now();
              const auto uni = (now.time_since_epoch() / sec);
              if (d_value > 0) {
                      std::cout << uni << " " << d_value << " " << (d_sum/d_count) << std::endl;
              }
              d_sum = 0;
              d_count = 0;
              d_value = value;
      }
      d_sum += in[0];
      d_count++;
      return 1;
    }

  } /* namespace habets38 */
} /* namespace gr */
