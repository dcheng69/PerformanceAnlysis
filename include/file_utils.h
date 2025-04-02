/* Copyright (C) 
 * 2025 - Clay Cheng
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */


#ifndef _FILE_UTILS_H

#include <string>
#include <cerrno>

/**
 * @brief Creates a directory at the specified path. If any parent directories
 *        do not exist, they will be created as well.
 *
 * @param szPath The path of the directory to be created.
 * @return True if the directory is successfully created, false otherwise.
 *         If an error occurs, the appropriate error number is set.
 */
bool CreateDir(const std::string& szPath);

/**
 * @brief Removes a directory at the specified path. Deletes all files and
 *        subdirectories recursively.
 *
 * @param szPath The path of the directory to be removed.
 * @param bSaveParentPath If true, the specified directory itself will be
 *                        preserved, but its contents will be deleted.
 * @return True if the directory is successfully removed, false otherwise.
 *         If an error occurs, the appropriate error number is set.
 */
bool RemoveDir(const std::string& szPath, bool bSaveParentPath);

#endif // !_FILE_UTILS_H
