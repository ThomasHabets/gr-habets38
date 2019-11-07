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

#ifndef INCLUDED_HABETS38_SWEEPSINK_H
#define INCLUDED_HABETS38_SWEEPSINK_H

#include <habets38/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace habets38 {

    /*!
     * \brief <+description of block+>
     * \ingroup habets38
     *
     */
    class HABETS38_API sweepsink : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<sweepsink> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of habets38::sweepsink.
       *
       * To avoid accidental use of raw pointers, habets38::sweepsink's
       * constructor is in a private implementation
       * class. habets38::sweepsink::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string& label);
    };

  } // namespace habets38
} // namespace gr

#endif /* INCLUDED_HABETS38_SWEEPSINK_H */
