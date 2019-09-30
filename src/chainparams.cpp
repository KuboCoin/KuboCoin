// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2018-2019 The DogeCash developers
// Copyright (c) 2018-2019 The KuboCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "stdio.h"
#include <iostream>
#include "libzerocoin/bignum.h"
#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints = 
	boost::assign::map_list_of
        (0, uint256("0x000008d67dbdf5f9bb9fba21d36e90d31a0fac2235420faf57b949fe97937dee"))
        (5000, uint256("0xc9747810067fe5db3d557a827b692b5414f850c18e26c2037bdc112d6d6e5d39"))
        (7500, uint256("0xbca6bb9a2913e5a62b73c5442327477f2ce9708d552ea21eb977fefa95612fab"))
        (10000, uint256("0x8617b5bf88166dbd7949ad3e603f50b5c04e61981095d61b42666e1f37cfd470"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1568666856, // * UNIX timestamp of last checkpoint block
    600513,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    200};        // * estimated number of transactions per day after checkpoint

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256("0x000008d67dbdf5f9bb9fba21d36e90d31a0fac2235420faf57b949fe97937dee"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1568666856,
    2305594,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x000008d67dbdf5f9bb9fba21d36e90d31a0fac2235420faf57b949fe97937dee"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1568666856,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xa0;
        pchMessageStart[1] = 0x43;
        pchMessageStart[2] = 0x16;
        pchMessageStart[3] = 0x20;
        vAlertPubKey = ParseHex("04e0d7b7044db3af06e9d31f64c1c6a68171938faf309b627f7035d7360bd36f33d331a26939681a0559599a5f5b3436c7b3f4dd2da9b63ac0e216b891d537a3ca");
        nDefaultPort = 23620;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // kubocoin starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // kubocoin: 1 day  
        nTargetSpacing = 1 * 90; //kubocoin: 1 Min
        nMaturity = 5;
        nMasternodeCountDrift = 15;
        nMasternodeCollateralLimit = 20000000; //MN collateral 20,000,000
        nMaxMoneyOut = 50000000000 * COIN; //100 Billion
        /** Height or Time Based Activations **/
        nLastPOWBlock = 350;
        nModifierUpdateBlock = INT_MAX;
        nZerocoinStartHeight = INT_MAX;
        nZerocoinStartTime = INT_MAX; // October 17, 2017 4:30:00 AM
        nBlockEnforceSerialRange = 100; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = INT_MAX; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = INT_MAX; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = INT_MAX; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's
        //nInvalidAmountFiltered = 268200*COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = INT_MAX; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nBlockDoubleAccumulated = 1050010;
        nEnforceNewSporkKey = 1425158000; //!> Sporks signed after (GMT): Tuesday, May 1, 2018 7:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1527811200; //!> Fully reject old spork key after (GMT): Friday, June 1, 2018 12:00:00 AM

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = 1686229;
      //  nSupplyBeforeFakeSerial = 4131563 * COIN;   // zerocoin supply at block nFakeSerialBlockheightEnd

        const char* pszTimestamp = "Bitcoin goes price rockets up days after Binance hack";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04e0d7b7044db3af06e9d31f64c1c6a68171938faf309b627f7035d7360bd36f33d331a26939681a0559599a5f5b3436c7b3f4dd2da9b63ac0e216b891d537a3ca") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1568666856;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 2537263;

	hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x000008d67dbdf5f9bb9fba21d36e90d31a0fac2235420faf57b949fe97937dee"));
        assert(genesis.hashMerkleRoot == uint256("0x25e845d26ed93c1c7b380871c9588e7403acf9e830e7c7832f9c5bedf80bae53"));

	vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("149.248.63.116", "149.248.63.116"));
        vSeeds.push_back(CDNSSeedData("108.61.205.10", "108.61.205.10"));
        vSeeds.push_back(CDNSSeedData("45.76.145.66", "45.76.145.66"));
        vSeeds.push_back(CDNSSeedData("149.28.186.121", "149.28.186.121"));

	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 45);
	base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
	base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 46);
	base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
	base58Prefixes[EXT_SECRET_KEY] = list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // KuboCoin BIP44 coin type is '438' (0x800001b6)
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x01)(0xb6).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        nBudgetCycleBlocks = 43200; //!< Amount of blocks in a months period of time (using 1 minutes per) = (60*24*30)
        strSporkKey = "04d58cd3576439b8bcbfe266214ab708a983aa3fa87a7afa12104f2c7022b12f682e1c44a0a99fc771a4ff925f92cdadbe1c7ccd094d7e229ef2dc5744083102f9";
        strObfuscationPoolDummyAddress = "KDv8dUifgBKkWM1nwjad7yNasQ41yA9ntR";
        nStartMasternodePayments = 1403728576; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974980842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = INT_MAX; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zkubo to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
        nProposalEstablishmentTime = 60 * 60 * 24; // Proposals must be at least a day old to make it into a budget
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
	pchMessageStart[0] = 0xa0;
        pchMessageStart[1] = 0x43;
        pchMessageStart[2] = 0x16;
        pchMessageStart[3] = 0x19;
        vAlertPubKey = ParseHex("040818ca42a8634c010ba1025a8d8a10a03ea286456f341df6a4a0aad1f123833695a662be2dc983a8da5fed7380bc9e1c3433e94422d36746cdd8e21a5f97b749");
        nDefaultPort = 30001;
        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // kubocoin: 1 day
        nTargetSpacing = 1 * 60;  // kubocoin: 1 minute
        nLastPOWBlock = 2000;
        nMaturity = 32;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 21000000 * COIN;
        nZerocoinStartHeight = INT_MAX;
        nZerocoinStartTime = INT_MAX;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1522454400; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;
        nSupplyBeforeFakeSerial = 0;
	//MineGenesis(genesis);
        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1558130910; 
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 5510938;

	hashGenesisBlock = genesis.GetHash();
//        assert(hashGenesisBlock == uint256("0x000006731efae2c29566ae9742908d09d3f6bf9543641785360585f2489d9142"));
//	assert(genesis.hashMerkleRoot == uint256("0x50e4540c67980dda3691442ee135b3102f4cdc3c2dd37bff0101cc7133b49e07"));

        vFixedSeeds.clear();
        vSeeds.clear();
       // vSeeds.push_back(CDNSSeedData("testnet.kubocoin.io", "testnet.kubocoin.io"));         // Single node address
       // vSeeds.push_back(CDNSSeedData("testnet1.kubocoin.io", "testnet1.kubocoin.io"));       // Single node address
       // vSeeds.push_back(CDNSSeedData("testnet2.kubocoin.io", "testnet2.kubocoin.io"));
	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 30);
	base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
	base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 122);
	base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
	base58Prefixes[EXT_SECRET_KEY] = list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xbc).convert_to_container<std::vector<unsigned char> >();

	convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        nBudgetCycleBlocks = 144; //!< Ten cycles per day on testnet
	strSporkKey = "04fc640bba80713c0666acda4d3ffce670307a55f90b703995c830a1e9110b07244508724b7106395f8336c78d3691ae5ba05abe3840f3a7e18d6b95acdd0de71d";
        strObfuscationPoolDummyAddress = "xp87cG8UEQgzs1Bk67Yk884C7pnQfAeo7q";
        nStartMasternodePayments = 1520837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet

        nProposalEstablishmentTime = 60 * 5; // Proposals must be at least 5 mns old to make it into a test budget
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa0;
        pchMessageStart[1] = 0x43;
        pchMessageStart[2] = 0x16;
        pchMessageStart[3] = 0x19;
        nDefaultPort = 51436;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // kubocoin: 1 day
        nTargetSpacing = 1 * 60;        // kubocoin: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nLastPOWBlock = 250;
        nMaturity = 100;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 0; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 21000000 * COIN;
        nZerocoinStartHeight = INT_MAX;
        nBlockZerocoinV2 = 300;
        nZerocoinStartTime = INT_MAX;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;
	//MineGenesis(genesis);
        //! Modify the regtest genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1558130910; 
        genesis.nBits = 0x1e0ffff0;
      	genesis.nNonce = 5510938;


	hashGenesisBlock = genesis.GetHash();
//        assert(hashGenesisBlock == uint256("0x000006731efae2c29566ae9742908d09d3f6bf9543641785360585f2489d9142"));
//        assert(genesis.hashMerkleRoot == uint256("0x50e4540c67980dda3691442ee135b3102f4cdc3c2dd37bff0101cc7133b49e07"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
