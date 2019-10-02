// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2018-2019 The DogeCash developers
// Copyright (c) 2018-2019 The KuboCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef kubocoin_GENWIT_H
#define kubocoin_GENWIT_H


#include <iostream>
#include "bloom.h"
#include "libzerocoin/Denominations.h"
#include "net.h"

class CGenWit {

    public:

    CGenWit();

    CGenWit(const CBloomFilter &filter, int startingHeight, libzerocoin::CoinDenomination den, int requestNum, CBigNum accWitValue = 0);

    bool isValid(int chainActiveHeight);

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(filter);
        filter.setNotFull();
        READWRITE(startingHeight);
        READWRITE(den);
        READWRITE(requestNum);
        READWRITE(accWitValue);
    }

    const CBloomFilter &getFilter() const;

    int getStartingHeight() const;

    libzerocoin::CoinDenomination getDen() const;

    int getRequestNum() const;

    CNode *getPfrom() const;

    void setPfrom(CNode *pfrom);

    const CBigNum &getAccWitValue() const;

    const std::string toString() const;

private:
    CBloomFilter filter;
    int startingHeight;
    libzerocoin::CoinDenomination den;
    int requestNum;
    CBigNum accWitValue;
    CNode* pfrom;
};


#endif //kubocoin_GENWIT_H
