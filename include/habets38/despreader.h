/* -*- c++ -*- */

#ifndef INCLUDED_HABETS38_DESPREADER_H
#define INCLUDED_HABETS38_DESPREADER_H

#include <gnuradio/block.h>
#include <habets38/api.h>

namespace gr {
namespace habets38 {

/*!
 * \brief <+description of block+>
 * \ingroup habets38
 *
 */
class HABETS38_API despreader : virtual public gr::block {
public:
  typedef boost::shared_ptr<despreader> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of habets38::despreader.
   *
   * To avoid accidental use of raw pointers, habets38::despreader's
   * constructor is in a private implementation
   * class. habets38::despreader::make is the public interface for
   * creating new instances.
   */
  static sptr make(const std::vector<char> &spread_bits, int threshold);
};

} // namespace habets38
} // namespace gr

#endif /* INCLUDED_HABETS38_DESPREADER_H */
