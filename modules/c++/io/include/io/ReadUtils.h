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

#ifndef __IO_READ_UTILS_H__
#define __IO_READ_UTILS_H__

#include <string>
#include <vector>

#include <sys/Conf.h>
#include <except/Context.h>

namespace io
{
/*!
 * Reads the contents of a file (binary or text), putting the raw bytes in
 * 'buffer'.  These are the exact bytes of the file, so text files will not
 * contain a null terminator.
 *
 * \param pathname Pathname of the file to read in
 * \param buffer Raw bytes of the file
 */
void CODAAPI readFileContents(const std::string& pathname,
                              std::vector<sys::byte>& buffer);

/*!
 * Reads the contents of a file into a string.  The file is assumed to be a
 * text file.
 *
 * \param pathname Pathname of the file to read in
 * \param[out] str Contents of the file
 */
void CODAAPI readFileContents(const std::string& pathname, std::string& str);

/*!
 * Reads the contents of a file into a string.  The file is assumed to be a
 * text file.
 *
 * \param pathname Pathname of the file to read in
 *
 * \return Contents of the file
 */
std::string CODAAPI readFileContents(const std::string& pathname);

}

#endif
