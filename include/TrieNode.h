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

#ifndef TRIENODE_H
#define TRIENODE_H

#include <string>
#include <unordered_map>
#include <memory>

class TrieNode {
public:
    TrieNode(const std::string& part);

    ~TrieNode();

    void addChild(const std::string& part, std::unique_ptr<TrieNode> child);

    TrieNode* getChild(const std::string& part) const;

    const std::string& getNodeValue() const;

    TrieNode* getParent() const;

    void setParent(TrieNode* parent);

    static std::string getFullPath(const TrieNode* node);

    void print(const TrieNode* node = nullptr, const std::string& prefix = "") const;
private:
    std::unordered_map<std::string, std::unique_ptr<TrieNode>> children;
    std::string nodeValue;
    TrieNode* parent;
};

#endif // TRIENODE_H
