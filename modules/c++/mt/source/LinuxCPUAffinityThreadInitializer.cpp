/* =========================================================================
 * This file is part of mt-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2019, MDA Information Systems LLC
 *
 * mt-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */


#include "mt/LinuxCPUAffinityThreadInitializer.h"

#if !defined(__APPLE_CC__)
#if defined(__linux) || defined(__linux__)

#include <sys/Conf.h>
#include <except/Exception.h>

namespace mt
{
LinuxCPUAffinityThreadInitializer::
LinuxCPUAffinityThreadInitializer(
        std::auto_ptr<const sys::ScopedCPUMaskUnix> cpu) :
    mCPU(cpu)
{
}

void LinuxCPUAffinityThreadInitializer::initialize()
{
    pid_t tid = 0;
    tid = ::gettid();
    if (::sched_setaffinity(tid, mCPU->getSize(), mCPU->getMask()) == -1)
    {
	   throw except::Exception(Ctxt("Failed setting processor affinity"));
    }
}
}
#endif
#endif
