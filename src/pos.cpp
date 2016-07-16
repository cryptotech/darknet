// Copyright (c) 2014-2016 The BlackCoin Core developers
// Copyright (c) 2012-2013 The PPCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pos.h"

#include "chain.h"
#include "chainparams.h"
#include "hash.h"
#include "uint256.h"

// Stake Modifier (hash modifier of proof-of-stake):
// The purpose of stake modifier is to prevent a txout (coin) owner from
// computing future proof-of-stake generated by this txout at the time
// of transaction confirmation. To meet kernel protocol, the txout
// must hash with a future stake modifier to generate the proof.
uint256 ComputeStakeModifier(const CBlockIndex* pindexPrev, const uint256& kernel)
{
    if (!pindexPrev)
        return uint256(); // genesis block's modifier is 0

    CHashWriter ss(SER_GETHASH, 0);
    ss << kernel << pindexPrev->nStakeModifier;
    return ss.GetHash();
}

// Check whether the coinstake timestamp meets protocol
bool CheckCoinStakeTimestamp(int64_t nTimeBlock, int64_t nTimeTx, const CBlock& block)
{
    if (block.IsProofOfStake())
        return (nTimeBlock == nTimeTx) && ((nTimeTx & Params().nStakeTimestampMask) == 0);
    else
        return (nTimeBlock == nTimeTx);
}

// Simplified version of CheckCoinStakeTimestamp() to check header-only timestamp
bool CheckStakeBlockTimestamp(int64_t nTimeBlock, const CBlock& block)
{
    return CheckCoinStakeTimestamp(nTimeBlock, nTimeBlock, block);
}
