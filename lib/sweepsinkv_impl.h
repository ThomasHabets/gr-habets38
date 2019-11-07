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

#ifndef INCLUDED_HABETS38_SWEEPSINKV_IMPL_H
#define INCLUDED_HABETS38_SWEEPSINKV_IMPL_H

#include <habets38/sweepsinkv.h>

namespace gr {
  namespace habets38 {

    class sweepsinkv_impl : public sweepsinkv
    {
     private:
      // Nothing to declare in this block.
            const size_t d_vlen;
            const pmt::pmt_t d_tag;
            const uint64_t d_samprate;
            uint64_t d_freq;
            std::vector<long double> d_sum;
            uint64_t d_count{};

     public:
      sweepsinkv_impl(std::string tag, int vlen);
      ~sweepsinkv_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace habets38
} // namespace gr

#endif /* INCLUDED_HABETS38_SWEEPSINKV_IMPL_H */
