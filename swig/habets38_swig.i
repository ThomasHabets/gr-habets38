/* -*- c++ -*- */

#define HABETS38_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "habets38_swig_doc.i"

%{
#include "habets38/sweepsink.h"
#include "habets38/sweepsinkv.h"
#include "habets38/uart_encoder.h"
#include "habets38/uart_decoder.h"
#include "habets38/despreader.h"
%}

%include "habets38/sweepsink.h"
GR_SWIG_BLOCK_MAGIC2(habets38, sweepsink);
%include "habets38/sweepsinkv.h"
GR_SWIG_BLOCK_MAGIC2(habets38, sweepsinkv);
%include "habets38/uart_encoder.h"
GR_SWIG_BLOCK_MAGIC2(habets38, uart_encoder);
%include "habets38/uart_decoder.h"
GR_SWIG_BLOCK_MAGIC2(habets38, uart_decoder);
%include "habets38/despreader.h"
GR_SWIG_BLOCK_MAGIC2(habets38, despreader);
