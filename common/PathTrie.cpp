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

#include "PathTrie.h"
#include "TrieNode.h"
#include <algorithm>
#include <iostream>
#include <sstream>

PathTrie::PathTrie() : root(std::make_unique<TrieNode>("/")) {}

PathTrie::~PathTrie() = default;

TrieNode* PathTrie::insert(const std::string& path) {
    TrieNode* node = root.get();
    std::istringstream ss(path);
    std::string part;

    // Validate the input path
    if(path == "") {
        return nullptr;
    }
    if(path == "/") {
        return root.get();
    }
    // Path does not start with '/'
    if(!path.empty() && path.front() != '/') {
        return nullptr;
    }
    // Exclude the first empty string before the '/'
    std::getline(ss, part, '/');
    while (std::getline(ss, part, '/')) {
        if (part.empty()) {
            // Error log
            return nullptr;
        }
        if (!node->getChild(part)) {
            auto newNode = std::make_unique<TrieNode>(part);
            newNode->setParent(node);
            node->addChild(part, std::move(newNode));
        }
        node = node->getChild(part);
    }

    return node;
}

void PathTrie::print() {
    root.get()->print();
}

TrieNode* PathTrie::GetRoot() {
    return root.get();
}
