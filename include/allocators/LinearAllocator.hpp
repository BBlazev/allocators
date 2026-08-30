#ifndef LINEAR_ALLOCATOR_HPP
#define LINEAR_ALLOCATOR_HPP

#include <cstddef>
#include <utility>
#include <bit>


class LinearAllocator {
  private:
    std::byte* StartPtr;
    std::size_t Capacity;
    std::size_t Offset{0};

  public:
    explicit LinearAllocator(std::size_t Size);
    ~LinearAllocator();

    LinearAllocator(const LinearAllocator&) = delete;
    LinearAllocator& operator=(const LinearAllocator&) = delete;

    LinearAllocator(LinearAllocator&& other) noexcept;
    LinearAllocator& operator=(LinearAllocator&& other) noexcept;

    [[nodiscard]]
    void* Allocate(std::size_t Size, std::size_t Alignment) noexcept {
        //Alignment = std::bit_ceil(Alignment);
        std::size_t AlignedOffset = (Offset + Alignment - 1) & ~(Alignment - 1);

        if (AlignedOffset + Size > Capacity)
            return nullptr;

        void* ResultPtr = StartPtr + AlignedOffset;
        Offset = AlignedOffset + Size;

        return ResultPtr;
    }
    void Reset() noexcept;

    template<typename T, typename... Args>
    T* Allocate(Args&&... args) noexcept{
        void* memory = Allocate(sizeof(T), alignof(T));
        if(!memory) return nullptr;
        return new (memory) T(std::forward<Args>(args)...);
    }

    [[nodiscard]] std::size_t get_size() const noexcept;
    [[nodiscard]] std::size_t get_used() const noexcept;
    [[nodiscard]] std::size_t get_remaining() const noexcept;
};

#endif