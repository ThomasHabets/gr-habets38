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

#ifndef INCLUDED_HABETS38_SWEEPSINKV_H
#define INCLUDED_HABETS38_SWEEPSINKV_H

#include <gnuradio/block.h>
#include <habets38/api.h>

namespace gr {
namespace habets38 {

/*!
 * \brief <+description of block+>
 * \ingroup habets38
 *
 */
class HABETS38_API sweepsinkv : virtual public gr::block
{
public:
    typedef boost::shared_ptr<sweepsinkv> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of habets38::sweepsinkv.
     *
     * To avoid accidental use of raw pointers, habets38::sweepsinkv's
     * constructor is in a private implementation
     * class. habets38::sweepsinkv::make is the public interface for
     * creating new instances.
     */
    static sptr make(std::string tag, int vlen);
};

} // namespace habets38
} // namespace gr

#endif /* INCLUDED_HABETS38_SWEEPSINKV_H */
