#pragma once

#define PLATFORM_SIM

#include <stdint.h>

#include "SimClock.hpp"
#include "platform/BaseRateLimiter.hpp"
#include "platform/BaseRateTask.hpp"

using Clock = SimClock;

typedef BaseRateLimiter<
    Clock,
    unsigned long,
    float> RateLimiter;

typedef BaseRateTask<RateLimiter> RateTask;
