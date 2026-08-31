#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP

#include <cstddef>
#include <new>
#include <utility>

template<typename T>
class PoolAllocator{
    static_assert(sizeof(T) >= sizeof(void*));
private:
    struct Node {
        Node* next;
    };
    std::size_t Capacity;
    std::byte* StartPtr;
    Node* Head{nullptr};

public:

    explicit PoolAllocator(std::size_t Size)
        : Capacity{Size}
        , StartPtr(new std::byte[Capacity * sizeof(T)])
    {
        Head = reinterpret_cast<Node*>(StartPtr);
        Node* Current = Head;
        for (std::size_t i = 0; i < Capacity - 1; ++i){
            std::byte* NextAddress = StartPtr + ((i + 1) * sizeof(T));
            Current->next = reinterpret_cast<Node*>(NextAddress);
            Current = Current->next;
        }
        Current->next = nullptr;
    }

    ~PoolAllocator(){
        delete[] StartPtr;
    }

    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    PoolAllocator(PoolAllocator&& other) noexcept
        : StartPtr(other.StartPtr)
        , Capacity(other.Capacity)
        , Head(other.Head)
    {
        other.StartPtr = nullptr;
        other.Head = nullptr;
        other.Capacity = 0;
    }

    PoolAllocator& operator=(PoolAllocator&& other) noexcept {
        if(this != &other) {
            delete[] StartPtr;

            StartPtr = other.StartPtr;
            Capacity = other.Capacity;
            Head = other.Head;

            other.StartPtr = nullptr;
            other.Head = nullptr;
            other.Capacity = 0;
        }
        return *this;
    }

    T* Allocate() noexcept {
        if(Head == nullptr)
            return nullptr;
        Node* FreeBlock = Head;
        Head = Head->next;
        return reinterpret_cast<T*>(FreeBlock);
    }

    template<typename...Args>
    T* Allocate(Args&&...args) noexcept {
        void* memory = Allocate();
        if(!memory) 
            return nullptr;
        return new (memory) T(std::forward<Args>(args)...);
    }

    void Deallocate(T* object) noexcept {
        if(!object) return;
        object->~T();
        Free(object);
    }

    void Free(void* Object) noexcept {
        if(Object==nullptr) 
            return;

        Node* RecycledBlock = reinterpret_cast<Node*>(Object);
        RecycledBlock->next = Head;
        Head = RecycledBlock;
    }


};



#endif