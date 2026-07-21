#include <bits/stdc++.h>
using namespace std;

using Node = array<int, 3>; // key, value, 频率
using NodeIterator = list<Node>::iterator;
using List = list<Node>;

class LFUCache {
private:
    int capacity;
    int min_freq;
    unordered_map<int, NodeIterator> key2node;
    unordered_map<int, List*> freq2list;

    // 辅助函数：更新节点频率（抽离重复逻辑）
    void updateNodeFreq(NodeIterator node) {
        auto [k, v, f] = *node; // 这里不要引用
        
        // 1. 从原频率链表中删除节点（先获取链表指针，避免迭代器失效）
        List* old_list = freq2list[f];
        old_list->erase(node); // 删除节点
        
        // 2. 如果原频率链表为空，释放内存并从map中删除
        if (old_list->empty()) {
            freq2list.erase(f);
            delete old_list; // 释放new分配的内存，避免内存泄漏
            
            // 3. 如果删除的是最小频率的链表，更新min_freq
            if (min_freq == f) {
                min_freq++;
            }
        }
        
        // 4. 频率+1，添加到新频率链表
        int new_freq = f + 1;
        if (freq2list.find(new_freq) == freq2list.end()) {
            freq2list[new_freq] = new List();
        }
        List* new_list = freq2list[new_freq];
        new_list->push_front({k, v, new_freq}); // 插入新节点（避免原迭代器失效问题）
        
        // 5. 更新key2node的迭代器
        key2node[k] = new_list->begin();
    }

public:
    LFUCache(int capacity) : capacity(capacity), min_freq(0) {
    }

    // 析构函数：释放所有new分配的List内存，避免内存泄漏
    ~LFUCache() {
        for (auto& [freq, list_ptr] : freq2list) {
            delete list_ptr;
        }
        freq2list.clear();
    }

    int get(int key) {
        // 边界：容量为0直接返回-1
        if (capacity == 0) return -1;
        
        auto it = key2node.find(key);
        if (it == key2node.end()) {
            return -1; // 未找到
        }
        
        // 更新节点频率
        updateNodeFreq(it->second);
        
        // 返回值（注意：必须从更新后的迭代器获取，避免访问失效内存）
        auto& [k, v, f] = *key2node[key];
        return v;
    }

    void put(int key, int value) {
        // 边界：容量为0直接返回
        if (capacity == 0) return;
        
        // 情况1：key已存在，更新值并调整频率
        auto it = key2node.find(key);
        if (it != key2node.end()) {
            auto& [k, v, f] = *it->second;
            v = value; // 更新值
            updateNodeFreq(it->second); // 更新频率
            return;
        }
        
        // 情况2：key不存在，需要插入
        // 2.1 如果容量已满，先删除最不经常使用的节点（min_freq链表的最后一个）
        if (key2node.size() == capacity) {
            List* min_list = freq2list[min_freq];
            // 关键：先获取要删除的节点信息，再erase（避免访问已释放内存）
            auto last_node = --min_list->end();
            auto& [del_key, del_val, del_freq] = *last_node;
            
            // 删除key2node中的映射
            key2node.erase(del_key);
            
            // 删除链表中的节点
            min_list->erase(last_node);
            
            // 如果链表为空，释放内存并删除频率映射
            if (min_list->empty()) {
                freq2list.erase(min_freq);
                delete min_list;
            }
        }
        
        // 2.2 插入新节点（频率为1）
        int new_freq = 1;
        if (freq2list.find(new_freq) == freq2list.end()) {
            freq2list[new_freq] = new List();
        }
        List* list_1 = freq2list[new_freq];
        list_1->push_front({key, value, new_freq});
        
        // 更新key2node和min_freq
        key2node[key] = list_1->begin();
        min_freq = new_freq; // 新插入的节点频率一定是1，所以min_freq=1
    }
};

