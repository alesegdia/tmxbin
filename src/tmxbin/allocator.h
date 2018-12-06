#pragma once

#include <cstdlib>

namespace tmxbin {


class Allocator
{
public:
    virtual ~Allocator();

    template <typename T>
    T* allocT(size_t num_elements=1)
    {
        return static_cast<T*>(alloc(sizeof(T), num_elements));
    }

    template<typename T, typename... Args>
    T* construct(Args&&... args)
    {
        return new (allocT<T>()) T(args...);
    }

    template <typename T>
    void deallocT(T* memory)
    {
        dealloc(memory);
    }

    virtual void* alloc(size_t element_size, size_t element_count) = 0 ;
    virtual void dealloc(void* memory) = 0 ;

};

class DefaultAllocator : public Allocator
{
public:
    virtual void* alloc(size_t element_size, size_t element_count) final
    {
        return malloc(element_size * element_count);
    }

    virtual void dealloc(void* memory) final
    {
        free(memory);
    }
};


}
