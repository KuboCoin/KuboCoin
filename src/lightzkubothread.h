// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2018-2019 The DogeCash developers
// Copyright (c) 2018-2019 The KuboCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef kubocoin_LIGHTzkuboTHREAD_H
#define kubocoin_LIGHTzkuboTHREAD_H

#include <atomic>
#include "genwit.h"
#include "zkubo/accumulators.h"
#include "concurrentqueue.h"
#include "chainparams.h"
#include <boost/function.hpp>
#include <boost/thread.hpp>

extern CChain chainActive;
// Max amount of computation for a single request
const int COMP_MAX_AMOUNT = 60 * 24 * 60;


/****** Thread ********/

class CLightWorker{

private:

    concurrentqueue<CGenWit> requestsQueue;
    std::atomic<bool> isWorkerRunning;
    boost::thread threadIns;

public:

    CLightWorker() {
        isWorkerRunning = false;
    }

    enum ERROR_CODES {
        NOT_ENOUGH_MINTS = 0,
        NON_DETERMINED = 1
    };

    bool addWitWork(CGenWit wit) {
        if (!isWorkerRunning) {
            LogPrintf("%s not running trying to add wit work \n", "kubocoin-light-thread");
            return false;
        }
        requestsQueue.push(wit);
        return true;
    }

    void StartLightzkuboThread(boost::thread_group& threadGroup) {
        LogPrintf("%s thread start\n", "kubocoin-light-thread");
        threadIns = boost::thread(boost::bind(&CLightWorker::ThreadLightzkuboSimplified, this));
    }

    void StopLightzkuboThread() {
        threadIns.interrupt();
        LogPrintf("%s thread interrupted\n", "kubocoin-light-thread");
    }

private:

    void ThreadLightzkuboSimplified();

    void rejectWork(CGenWit& wit, int blockHeight, uint32_t errorNumber);

};

#endif //kubocoin_LIGHTzkuboTHREAD_H
