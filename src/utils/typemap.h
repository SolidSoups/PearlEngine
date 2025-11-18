#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>

class TypeMap{
private:
  std::unordered_map<std::type_index, std::any> data;

public:
  template<typename T, typename E>
  void Set(E value){
    data[std::type_index(typeid(T))] = std::move(value);
  }

  template<typename T, typename E>
  E* Get(){
    auto it = data.find(std::type_index(typeid(T)));
    if(it != data.end()){
      return std::any_cast<E>(&it->second);
    }
    return nullptr;
  }

  template<typename T>
  bool Has() const{
    return data.find(std::type_index(typeid(T))) != data.end();
  }
};
