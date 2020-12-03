#pragma once

#include "liquid.h"

typedef struct _SYMTRACK_ {
    // parameters
    int             filter_type;        // filter type (e.g. LIQUID_FIRFILT_RRC)
    unsigned int    k;                  // samples/symbol
    unsigned int    m;                  // filter semi-length
    float           beta;               // filter excess bandwidth
    int             mod_scheme;         // demodulator

    // automatic gain control
    agc_crcf           agc;                // agc object
    float           agc_bandwidth;      // agc bandwidth

    // symbol timing recovery
    symsync_crcf       symsync;            // symbol timing recovery object
    float           symsync_bandwidth;  // symsync loop bandwidth
    liquid_float_complex symsync_buf[8];     // symsync output buffer
    unsigned int    symsync_index;      // symsync output sample index

    // equalizer/decimator
    eqlms_cccf         eq;                 // equalizer (LMS)
    unsigned int    eq_len;             // equalizer length
    float           eq_bandwidth;       // equalizer bandwidth
    enum {
        SYMTRACK_EQ_CM,     // equalizer strategy: constant modulus
        SYMTRACK_EQ_DD,     // equalizer strategy: decision directed
        SYMTRACK_EQ_OFF,    // equalizer strategy: disabled
    }               eq_strategy;

    // nco/phase-locked loop
    nco_crcf           nco;                // nco (carrier recovery)
    float           pll_bandwidth;      // phase-locked loop bandwidth

    // demodulator
    modem         demod;              // linear modem demodulator

    // state and counters
    unsigned int    num_syms_rx;        // number of symbols recovered
} SYMTRACK;
