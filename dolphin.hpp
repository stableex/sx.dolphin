#pragma once

#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>

namespace dolphin {

    using eosio::asset;
    using eosio::symbol;
    using eosio::extended_symbol;
    using eosio::symbol_code;
    using eosio::name;
    using eosio::multi_index;
    using eosio::singleton;

    using std::vector;
    using std::pair;

    /**
     * pools
     */

    struct pooltoken {
        uint32_t        weight;
        extended_symbol symbol;
        asset           reserve;
    };

    struct [[eosio::table]] pools_row {
        uint64_t            id;
        symbol_code         code;
        uint16_t            swap_fee;
        uint64_t            total_lptoken;
        uint32_t            create_time;
        uint32_t            last_update_time;
        vector <pooltoken>  tokens;

        uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index< "pools"_n, pools_row > pools;

    /**
     * ## STATIC `get_fee`
     *
     * Get total fee
     *
     * ### returns
     *
     * - `{uint8_t}` - total fee (trade + protocol)
     *
     * ### example
     *
     * ```c++
     * const uint8_t fee = dolphin::get_fee();
     * // => 30
     * ```
     */
    static uint8_t get_fee()
    {
        return 30;
    }

    /**
     * ## STATIC `get_reserves`
     *
     * Get reserves for a pair
     *
     * ### params
     *
     * - `{uint64_t} pair_id` - pair id
     * - `{symbol} sort` - sort by symbol (reserve0 will be first item in pair)
     *
     * ### returns
     *
     * - `{pair<asset, asset>}` - pair of reserve assets
     *
     * ### example
     *
     * ```c++
     * const uint64_t pair_id = 1;
     * const symbol sort = symbol{"EOS", 4};
     *
     * const auto [reserve0, reserve1] = dolphin::get_reserves( pair_id, sort );
     * // reserve0 => "4638.5353 EOS"
     * // reserve1 => "13614.8381 DOP"
     * ```
     */
    static pair<pair<asset, uint64_t>, pair<asset, uint64_t>> get_reserves( const uint64_t pair_id, const symbol sort )
    {
        // table
        dolphin::pools _pools( "dolphinsswap"_n, "dolphinsswap"_n.value );
        auto pool = _pools.get(pair_id, "DolphinLibrary: INVALID_PAIR_ID");

        return sort == pool.tokens[0].symbol.get_symbol() ?
            pair<pair<asset, uint64_t>, pair<asset, uint64_t>>{{ pool.tokens[0].reserve, pool.tokens[0].weight}, { pool.tokens[1].reserve, pool.tokens[1].weight} } :
            pair<pair<asset, uint64_t>, pair<asset, uint64_t>>{{ pool.tokens[1].reserve, pool.tokens[1].weight}, { pool.tokens[0].reserve, pool.tokens[0].weight} };
    }
}