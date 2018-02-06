/* =========================================================================
 * This file is part of io-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2017, MDA Information Systems LLC
 *
 * io-c++ is free software; you can redistribute it and/or modify
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

#include <io/FileInputStream.h>
#include <io/ReadUtils.h>

namespace io
{
void readFileContents(const std::string& pathname,
                      std::vector<sys::byte>& buffer)
{
    io::FileInputStream inStream(pathname);
    buffer.resize(inStream.available());
    if (!buffer.empty())
    {
        inStream.read(&buffer[0], buffer.size(), true);
    }
}

void readFileContents(const std::string& pathname, std::string& str)
{
    std::vector<sys::byte> buffer;
    readFileContents(pathname, buffer);

    if (buffer.empty())
    {
        str.clear();
    }
    else
    {
        str.assign(reinterpret_cast<char*>(&buffer[0]), buffer.size());
    }
}

std::string readFileContents(const std::string& pathname)
{
    std::string str;
    readFileContents(pathname, str);
    return str;
}
}
