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

#include "TrieNode.h"
#include <iostream>

TrieNode::TrieNode(const std::string& part) : nodeValue(part), parent(nullptr) {}

TrieNode::~TrieNode() = default;

void TrieNode::addChild(const std::string& part, std::unique_ptr<TrieNode> child) {
    children[part] = std::move(child);
}

TrieNode* TrieNode::getChild(const std::string& part) const {
    auto it = children.find(part);
    return it != children.end() ? it->second.get() : nullptr;
}

const std::string& TrieNode::getNodeValue() const {
    return nodeValue;
}

TrieNode* TrieNode::getParent() const {
    return parent;
}

void TrieNode::setParent(TrieNode* parent) {
    this->parent = parent;
}

std::string TrieNode::getFullPath(const TrieNode* node) {
    if (!node) {
        return "";
    }

    std::string path;
    for(const TrieNode * tempNode = node;
            tempNode != nullptr;
            tempNode = tempNode->parent) {
        if(path == "") {
            path = tempNode->nodeValue;
            continue;
        }
        if(tempNode->nodeValue != "/") {
            path = tempNode->nodeValue + "/" + path;
        } else {
            path = "/" + path;
        }
    }
    return path;
}

void TrieNode::print(const TrieNode* node, const std::string& prefix) const {
    if (node == nullptr) {
        node = this;
    }

    // Print current node
    std::cout << prefix << "└── " << node->nodeValue << std::endl;

    // Iterator all child nodes
    for (const auto &it : node->children) {
        // Print children recursively
        print(it.second.get(), prefix + "    ");
    }
}
