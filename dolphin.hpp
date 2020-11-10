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

    const name id = "dolphin"_n;
    const name code = "dolphinsswap"_n;
    const string description = "Dolphin Converter";

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
     * - `{symbol} from - which symbol we convert from
     * - `{symbol} to - which symbol we convert to
     *
     * ### returns
     *
     * - `{pair<pair<asset, uint64_t>, pair<asset,uint64_t>>}` - pair of reserve assets as {reserve, weight} pair
     *
     * ### example
     *
     * ```c++
     * const uint64_t pair_id = 1;
     * const symbol from = symbol{"EOS", 4};
     * const symbol to = symbol{"DOP", 4};
     *
     * const auto [reserve0, reserve1] = dolphin::get_reserves( pair_id, from, to );
     * // reserve0 => {"4638.5353 EOS", 50}
     * // reserve1 => {"13614.8381 DOP", 50}
     * ```
     */
    static pair<pair<asset, uint64_t>, pair<asset, uint64_t>> get_reserves( const uint64_t pair_id, const symbol from, const symbol to )
    {
        // table
        dolphin::pools _pools( "dolphinsswap"_n, "dolphinsswap"_n.value );
        auto poolit = _pools.find(pair_id);
        check(poolit!=_pools.end(), "DolphinLibrary: INVALID_PAIR_ID "+to_string(pair_id) + " for " + from.code().to_string()+"->"+to.code().to_string());
        pair<asset, uint64_t> res1, res2;

        for(auto& token: poolit->tokens){
            if(token.symbol.get_symbol()==from)
                res1 = { token.reserve, token.weight};
            if(token.symbol.get_symbol()==to)
                res2 = { token.reserve, token.weight};
        }

        return {res1, res2};
    }
}