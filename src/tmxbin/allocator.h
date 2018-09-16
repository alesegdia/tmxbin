#pragma once

#include <cstdlib>

namespace tmxbin {


class Allocator
{
public:
    virtual ~Allocator();

    template <typename T>
    T* alloc(size_t num_elements)
    {
        return static_cast<T*>(alloc(sizeof(T), num_elements));
    }

    template <typename T>
    void dealloc(T* memory)
    {
        free(memory);
    }

protected:
    virtual void* alloc(size_t element_size, size_t element_count) = 0 ;
    virtual void dealloc(void* memory) = 0 ;

};

class DefaultAllocator : public Allocator
{
private:
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
