#!/usr/bin/env python
# -*- coding: utf-8 -*-

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import habets38_swig as habets38

class qa_despreader(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def __test_aligned(self):
        spread = [0]*8
        src_data = [0,1,1,0,1,1,1,1,1, 0,0,0,0,0,0,0,0, 0,0,0,0]
        expected = (0,1,0)
        expected_offset = 0
        # set up fg
        despreader = habets38.despreader(spread, 1)
        src = blocks.vector_source_b(src_data, repeat=False)
        dst = blocks.vector_sink_b()

        self.tb.connect(src, despreader)
        self.tb.connect(despreader, dst)

        self.tb.run()
        self.assertEqual(expected, dst.data())
        # check data

    def __test_barker(self):
        spread = [1,1,1,0,0,1,0] # Barker code 7
        #                                 1 1 1 1
        #           0 1 2 3 4 5 6   7 8 9 0 1 2 3
        #                   E
        src_data = [0,0,0,1,0,0,1,  1,1,1,0,0,1,0, 0,0,0,0]
        expected = (1, 0)
        expected_offset = 0
        # set up fg
        despreader = habets38.despreader(spread, 1)
        src = blocks.vector_source_b(src_data, repeat=False)
        dst = blocks.vector_sink_b()
        error_dst = blocks.vector_sink_i()
        skip_dst = blocks.vector_sink_i()

        self.tb.connect(src, despreader)
        self.tb.connect((despreader,0), dst)
        self.tb.connect((despreader,1), error_dst)
        self.tb.connect((despreader,2), skip_dst)

        self.tb.run()
        self.assertEqual(expected, dst.data())
        self.assertEqual((1,0), error_dst.data())
        self.assertEqual((len(spread)-1,0), skip_dst.data())
        # check data

    def test_barker_skip(self):
        spread = [1,1,1,0,0,1,0] # Barker code 7
        #                                 1 1 1 1
        #           0 1 2 3 4 5 6   7 8 9 0 1 2 3
        #                   E
        src_data = [0,0,0,1,0,0,1,  0,1, 1,1,1,0,0,1,0, 0,0,0,0]
        expected = (1, 0)
        expected_offset = 0
        # set up fg
        despreader = habets38.despreader(spread, 1)
        src = blocks.vector_source_b(src_data, repeat=False)
        dst = blocks.vector_sink_b()
        error_dst = blocks.vector_sink_i()
        skip_dst = blocks.vector_sink_i()

        self.tb.connect(src, despreader)
        self.tb.connect((despreader,0), dst)
        self.tb.connect((despreader,1), error_dst)
        self.tb.connect((despreader,2), skip_dst)

        self.tb.run()
        self.assertEqual(expected, dst.data())
        self.assertEqual((1,0), error_dst.data())
        self.assertEqual((len(spread)-1,2), skip_dst.data())
        # check data

    def __test_misaligned(self):
        spread = [0,0,0,0]
        src_data = [1,1,1,0,0,0,0,0]
        expected = (1,0)
        expected_offset = 0
        # set up fg
        despreader = habets38.despreader(spread)
        src = blocks.vector_source_b(src_data)
        dst = blocks.vector_sink_b()

        self.tb.connect(src, despreader)
        self.tb.connect(despreader, dst)

        self.tb.run()
        self.assertEqual(expected, dst.data())
        self.assertEqual(expected_offset, despreader.offset())
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_despreader)
