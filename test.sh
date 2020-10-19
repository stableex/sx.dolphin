#!/bin/bash

# build
eosio-cpp __tests__/basic.cpp -I ../

# unlock wallet
cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# create accounts
cleos create account eosio basic EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# deploy
cleos set contract basic . basic.wasm basic.abi

# getreserve
# 1
cleos -v push action basic getreserves '[1, "4,EOS"]' -p basic
# //=> EOS/DOP

# getfee
cleos -v push action basic getfee '[]' -p basic
# //=> 30
