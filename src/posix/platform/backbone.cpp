/*
 *  Copyright (c) 2020, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the platform Backbone interface management on Linux.
 */

#include "openthread-posix-config.h"

#if OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE

#include "multicast_routing.hpp"
#include "platform-posix.h"
#include "common/code_utils.hpp"

char                                      gBackboneNetifName[IFNAMSIZ] = "";
unsigned int                              gBackboneNetifIndex          = 0;
static ot::Posix::MulticastRoutingManager sMulticastRoutingManager;

void platformBackboneInit(otInstance *aInstance, const char *aInterfaceName)
{
    OT_UNUSED_VARIABLE(aInstance);

    VerifyOrDie(aInterfaceName != nullptr, OT_EXIT_INVALID_ARGUMENTS);

    VerifyOrDie(strnlen(aInterfaceName, IFNAMSIZ) <= IFNAMSIZ - 1, OT_EXIT_INVALID_ARGUMENTS);
    strcpy(gBackboneNetifName, aInterfaceName);

    gBackboneNetifIndex = if_nametoindex(gBackboneNetifName);
    VerifyOrDie(gBackboneNetifIndex > 0, OT_EXIT_FAILURE);

    otLogInfoPlat("Backbone interface is configured to %s (%d)", gBackboneNetifName, gBackboneNetifIndex);

    sMulticastRoutingManager.Init(aInstance);
}

void platformBackboneUpdateFdSet(fd_set &aReadFdSet, int &aMaxFd)
{
    sMulticastRoutingManager.UpdateFdSet(aReadFdSet, aMaxFd);
}

void platformBackboneProcess(const fd_set &aReadSet)
{
    sMulticastRoutingManager.Process(aReadSet);
}

void platformBackboneStateChange(otInstance *aInstance, otChangedFlags aFlags)
{
    sMulticastRoutingManager.HandleStateChange(aInstance, aFlags);
}

#endif
