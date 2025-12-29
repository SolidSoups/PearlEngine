#pragma once


#include <iterator>
#include <unordered_map>
#include <utility>
#include <memory>
#include <iostream>

template<typename Key, typename Value>
class StaticRegistry{
private:
    std::unordered_map<Key, std::unique_ptr<Value>> m_Registry;

public:
    void Register(const Key &_key, std::unique_ptr<Value> &&_value){
        m_Registry.insert_or_assign(_key, std::move(_value));
    }

    Value* Get(const Key &_key){
        auto it = m_Registry.find(_key);
        if(it == m_Registry.end()) 
            return nullptr;
        return it->second.get();
    }
};
