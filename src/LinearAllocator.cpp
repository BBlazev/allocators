#include <allocators/LinearAllocator.hpp>
#include <cstdio>

LinearAllocator::LinearAllocator(std::size_t Size)
    : StartPtr{new std::byte[Size]}
    , Capacity{Size}
{}

LinearAllocator::~LinearAllocator() {
    delete[] StartPtr;
}

LinearAllocator::LinearAllocator(LinearAllocator&& other) noexcept
    : StartPtr{other.StartPtr}
    , Capacity{other.Capacity}
    , Offset{other.Offset}
{
    other.StartPtr = nullptr;
    other.Capacity = 0;
    other.Offset = 0;
}

LinearAllocator& LinearAllocator::operator=(LinearAllocator&& other) noexcept {
    if (this != &other) {
        delete[] StartPtr;

        StartPtr = other.StartPtr;
        Capacity = other.Capacity;
        Offset = other.Offset;

        other.StartPtr = nullptr;
        other.Capacity = 0;
        other.Offset = 0;
    }
    return *this;
}



void LinearAllocator::Reset() noexcept {
    Offset = 0;
}

std::size_t LinearAllocator::get_size() const noexcept {
    return Capacity;
}

std::size_t LinearAllocator::get_used() const noexcept {
    return Offset;
}

std::size_t LinearAllocator::get_remaining() const noexcept {
    return Capacity - Offset;
}