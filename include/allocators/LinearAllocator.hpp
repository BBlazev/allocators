#ifndef LINEAR_ALLOCATOR_HPP
#define LINEAR_ALLOCATOR_HPP

#include <bit>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <new>

class LinearAllocator {
  private:
    std::byte* StartPtr;
    std::size_t Capacity;
    std::size_t Offset{0};

  public:
    LinearAllocator(std::size_t Size) : Capacity{Size} { StartPtr = new std::byte[Size]; }

    ~LinearAllocator() { delete[] StartPtr; }

    LinearAllocator(const LinearAllocator&) = delete;
    LinearAllocator& operator=(const LinearAllocator&) = delete;

    LinearAllocator(LinearAllocator&& other) noexcept
        : StartPtr{other.StartPtr}
        , Capacity{other.Capacity}
        , Offset{other.Offset}
    {
        other.StartPtr = nullptr;
        other.Capacity = 0;
        other.Offset = 0;
    }

    LinearAllocator& operator=(LinearAllocator&& other) noexcept {
      if(this != &other){
        delete[] StartPtr;
        Capacity = other.Capacity;
        StartPtr = other.StartPtr;
        Offset = other.Offset;
  
        other.StartPtr = nullptr;
        other.Capacity = 0;
        other.Offset = 0;
      }
      return *this;
    }

    [[nodiscard]]void* Allocate(std::size_t Size, std::size_t Alignment = alignof(std::max_align_t)) noexcept {
        
      Alignment = std::bit_ceil(Alignment);
        std::size_t AlignedOffset = (Offset + Alignment - 1) & ~(Alignment - 1);

        if (AlignedOffset + Size > Capacity)
            return nullptr;
        void* ResultPtr = StartPtr + AlignedOffset;
        Offset = AlignedOffset + Size;

        return ResultPtr;
    }
    void Reset() noexcept { Offset = 0; }
};

#endif // !LINEAR_ALLOCATOR_HPP
