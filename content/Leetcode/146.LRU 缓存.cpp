#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

// 预定义别名，增加可读性
using Node = pair<int, int>;
using NodeIterator = list<Node>::iterator;

class LRUCache {
private:
    int capacity;
    list<Node> cacheList; // 双向链表：存储 {key, value}
    unordered_map<int, NodeIterator> nodeMap; // 哈希表：存储 key 到 链表迭代器 的映射

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        if (nodeMap.find(key) == nodeMap.end()) return -1;

        // 核心操作：将访问过的节点移动到链表头部 (Most Recently Used)
        // splice 参数：目标位置，源链表，要移动的迭代器
        cacheList.splice(cacheList.begin(), cacheList, nodeMap[key]);
        
        return nodeMap[key]->second;
    }

    void put(int key, int value) {
        if (nodeMap.find(key) != nodeMap.end()) {
            // 1. 已存在：更新值，移动到头部
            nodeMap[key]->second = value;
            cacheList.splice(cacheList.begin(), cacheList, nodeMap[key]);
        } else {
            // 2. 不存在：检查容量
            if (cacheList.size() == capacity) {
                // 驱逐最久未使用的节点 (链表尾部)
                int lastKey = cacheList.back().first;
                nodeMap.erase(lastKey);
                cacheList.pop_back();
            }
            // 插入新节点到头部
            cacheList.push_front({key, value});
            nodeMap[key] = cacheList.begin();
        }
    }
};
