// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2021      CoinGreen Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "amount.h"

#include "tinyformat.h"

const std::string CURRENCY_UNIT = "COINGREEN";

CFeeRate::CFeeRate(const CAmount& nFeePaid, size_t nBytes_)
{
    assert(nBytes_ <= uint64_t(std::numeric_limits<int64_t>::max()));
    int64_t nSize = int64_t(nBytes_);

    if (nSize > 0)
        nGreenLeavesPerK = nFeePaid * 1000 / nSize;
    else
        nGreenLeavesPerK = 0;
}

CAmount CFeeRate::GetFee(size_t nBytes_) const
{
    assert(nBytes_ <= uint64_t(std::numeric_limits<int64_t>::max()));
    int64_t nSize = int64_t(nBytes_);

    // Dogecoin: Round up to the nearest 1000 bytes so we get round tx fees
    if (nSize % 1000 > 0) {
        nSize = nSize + 1000 - (nSize % 1000);
    }

    CAmount nFee = nGreenLeavesPerK * nSize / 1000;

    if (nFee == 0 && nSize != 0) {
        if (nGreenLeavesPerK > 0)
            nFee = CAmount(1);
        if (nGreenLeavesPerK < 0)
            nFee = CAmount(-1);
    }

    return nFee;
}

std::string CFeeRate::ToString() const
{
    return strprintf("%d.%08d %s/kB", nGreenLeavesPerK / COIN, nGreenLeavesPerK % COIN, CURRENCY_UNIT);
}
