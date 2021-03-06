// Copyright Steinwurf ApS 2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodoc/kodoc.h>

#include <cstdint>

#include <gtest/gtest.h>

#include "test_helper.hpp"

static void test_decoder(uint32_t symbols, uint32_t symbol_size,
                         int32_t code_type, int32_t finite_field,
                         int32_t trace_enabled)
{
    kodo_factory_t decoder_factory =
        kodo_new_decoder_factory(code_type, finite_field,
                                 symbols, symbol_size,
                                 trace_enabled);

    kodo_coder_t decoder = kodo_factory_new_decoder(decoder_factory);

    // Coder methods

    EXPECT_EQ(symbols, kodo_symbols(decoder));
    EXPECT_EQ(symbol_size, kodo_symbol_size(decoder));
    EXPECT_EQ(symbols * symbol_size, kodo_block_size(decoder));
    EXPECT_GT(kodo_payload_size(decoder), symbol_size);
    EXPECT_EQ(0U, kodo_rank(decoder));

    if (code_type == kodo_full_rlnc ||
        code_type == kodo_on_the_fly)
    {
        EXPECT_TRUE(kodo_has_feedback_size(decoder) == 0);
    }
    else if (code_type == kodo_sliding_window)
    {
        EXPECT_TRUE(kodo_has_feedback_size(decoder) != 0);
        EXPECT_GT(kodo_feedback_size(decoder), 0U);
    }

    if (trace_enabled == kodo_trace_disabled)
    {
        EXPECT_TRUE(kodo_has_trace(decoder) == 0);
    }
    else if (trace_enabled == kodo_trace_enabled)
    {
        EXPECT_TRUE(kodo_has_trace(decoder) != 0);
    }

    // Decoder methods

    if (code_type == kodo_on_the_fly ||
        code_type == kodo_sliding_window)
    {
        EXPECT_TRUE(kodo_has_partial_decoding_tracker(decoder) != 0);
    }
    else if (code_type == kodo_full_rlnc)
    {
        EXPECT_TRUE(kodo_has_partial_decoding_tracker(decoder) == 0);
    }

    kodo_delete_decoder(decoder);
    kodo_delete_decoder_factory(decoder_factory);
}

TEST(TestDecoder, invoke_api)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    test_combinations(
        test_decoder,
        symbols, symbol_size, kodo_trace_disabled);

    test_combinations(
        test_decoder,
        symbols, symbol_size, kodo_trace_enabled);
}
