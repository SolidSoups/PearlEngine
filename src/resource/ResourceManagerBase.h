#pragma once


class ResourceManagerBase{
public:
    virtual ~ResourceManagerBase() = default;
    virtual void DestroyAll() = 0;
};
