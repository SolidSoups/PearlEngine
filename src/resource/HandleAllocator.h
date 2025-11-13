#pragma once

#include <vector>
#include "Handle.h"

template<typename T>
class HandleAllocator{
  std::vector<HandleID> m_FreeIDs;
  std::vector<HandleGenID> m_Generations;
  HandleID m_NextHandle = 1;
  size_t m_TotalAllocations = 0;

public:
  const size_t GetFreeIDCount() const { return m_FreeIDs.size(); }
  const size_t GetTotalAllocations() const { return m_TotalAllocations; }

public:
  Handle<T> Allocate(){
    m_TotalAllocations++;

    HandleID id;
    if(!m_FreeIDs.empty()){
      id = m_FreeIDs.back();
      m_FreeIDs.pop_back();
    } else{
      id = m_NextHandle++;
      if(m_Generations.size() <= id){
        m_Generations.resize(id + 1);
      }
    }

    return {id, ++m_Generations[id]};
  };

  void Free(Handle<T> handle){
    if(IsValid(handle)){
      m_FreeIDs.push_back(handle.id);
    }
  }

  bool IsValid(Handle<T> handle) const{
    return  handle.id > 0 &&
            handle.id < m_Generations.size() &&
            m_Generations[handle.id] == handle.generation;
  }
};
