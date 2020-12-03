/*
* High Speed modem to transfer data in a 2,7kHz SSB channel
* =========================================================
* Author: DJ0ABR
*
*   (c) DJ0ABR
*   www.dj0abr.de
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
* 
*/

#include "hsmodem.h"


#include <cstddef>
#include <iostream>
#include <string>

#include "fec/schifra_galois_field.hpp"
#include "fec/schifra_galois_field_polynomial.hpp"
#include "fec/schifra_sequential_root_generator_polynomial_creator.hpp"
#include "fec/schifra_reed_solomon_encoder.hpp"
#include "fec/schifra_reed_solomon_decoder.hpp"
#include "fec/schifra_reed_solomon_block.hpp"
#include "fec/schifra_error_processes.hpp"

/* Finite Field Parameters */
const std::size_t field_descriptor                =   8;
const std::size_t generator_polynomial_index      = 120;
const std::size_t generator_polynomial_root_count =  FECLEN;

/* Reed Solomon Code Parameters */
const std::size_t code_length = FECBLOCKLEN;
const std::size_t fec_length  =  FECLEN;
const std::size_t data_length = code_length - fec_length;

/* Instantiate Finite Field and Generator Polynomials */
const schifra::galois::field field(field_descriptor,
                            schifra::galois::primitive_polynomial_size06,
                            schifra::galois::primitive_polynomial06);

schifra::galois::field_polynomial generator_polynomial(field);

/* Instantiate Encoder and Decoder (Codec) */
typedef schifra::reed_solomon::encoder<code_length,fec_length,data_length> encoder_t;
typedef schifra::reed_solomon::decoder<code_length,fec_length,data_length> decoder_t;




int cfec_Reconstruct(uint8_t *darr, uint8_t *destination)
{
schifra::reed_solomon::block<code_length,fec_length> rxblock;

   for(std::size_t i=0; i<code_length; i++)
     rxblock.data[i] = darr[i];
   
   const decoder_t decoder(field, generator_polynomial_index);
   if (!decoder.decode(rxblock))
   {
      // FEC decoding not possible
      return 0;
   }

   for(std::size_t i=0; i<data_length; i++)
       destination[i] = rxblock[i];
   
   return 1;
}

void GetFEC(uint8_t *txblock, int len, uint8_t *destArray)
{
schifra::reed_solomon::block<code_length,fec_length> block;

    // fill payload into an FEC-block
   for(std::size_t i=0; i<data_length; i++)
       block.data[i] = txblock[i];

   /* Transform message into Reed-Solomon encoded codeword */
   const encoder_t encoder(field, generator_polynomial);
   if (!encoder.encode(block))
   {
      // encoding not possible, should never happen
      return;
   }

   // get result out of the FEC block
   for(std::size_t i=0; i<code_length; i++)
     destArray[i] = block[i];
}

void initFEC()
{
   if (!schifra::make_sequential_root_generator_polynomial( field,
                                                            generator_polynomial_index,
                                                            generator_polynomial_root_count,
                                                            generator_polynomial))
   {
      std::cout << "Error - Failed to create sequential root generator!" << std::endl;
      return;
   }
}

