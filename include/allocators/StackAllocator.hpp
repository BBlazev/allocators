#ifndef STACK_ALLOCATOR_HPP
#define STACK_ALLOCATOR_HPP

#include <cstddef>
#include <utility>
#include <bit>
#include <new>

class StackAllocator{
    private:
        std::byte* StartPtr;
        std::size_t Capacity;
        std::size_t Offset{0};

        struct Header{
            std::size_t BlockOffset{0};
        };


    public:

        explicit StackAllocator(std::size_t Size)
            : StartPtr{new std::byte[Size]}
            , Capacity{Size}
        {}

        ~StackAllocator() {
            delete[] StartPtr;
        }

        StackAllocator(const StackAllocator&) = delete;
        StackAllocator& operator=(const StackAllocator&) = delete;

        StackAllocator(StackAllocator&& other) noexcept 
            : StartPtr{other.StartPtr}
            , Capacity{other.Capacity}
            , Offset{other.Offset}
        {
            other.StartPtr = nullptr;
            other.Capacity = 0;
            other.Offset = 0;
        }

        StackAllocator& operator=(StackAllocator&& other) noexcept {
            if(this != &other){
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


        /*
            We always store [header][data][header][data][...][...]...
            eg. Allocate(...);
            Lets say our offset is 50 at starting ponint
            
        */
        [[nodiscard]]
        void* Allocate(std::size_t Size, std::size_t Alignment) noexcept {

            std::size_t HeaderSize = sizeof(Header);
            //After we add HeaderSize, Offset is now 58, and with alignment is 64
            std::size_t AlignedOffset = 
            (Offset + HeaderSize + Alignment -1) & ~(Alignment - 1);
            //Classic check
            if(AlignedOffset + Size > Capacity)
                return nullptr;
            //Because at Offset 64 starts USER DATA, our header has to start at that offset - sizeof(Header)
            void* HdrPtr = StartPtr + (AlignedOffset - HeaderSize);

            //We insert previous Offset, 50, into headers address
            Header* HeaderAddress = new(HdrPtr) Header();
            HeaderAddress->BlockOffset = Offset;

            //Calculate pointer where user data should start, at 64
            void* ResultPtr = StartPtr + AlignedOffset;
            //Update Offset to be 64 + ...
            Offset = AlignedOffset + Size;

            return ResultPtr;
        }

        void Deallocate(void* address) noexcept {
            Header* hdrptr = reinterpret_cast<Header*>(
                static_cast<std::byte*>(address) - sizeof(Header));

            Offset = hdrptr->BlockOffset;
        }

        template<typename T>
        void Deallocate(T* object) noexcept{
            object->~T();
            Deallocate(static_cast<void*>(object));
            object = nullptr;
        }

        void Reset() noexcept {
            Offset = 0;
        }

        template<typename T, typename... Args>
        T* Allocate(Args&&... args) noexcept{
            void* memory = Allocate(sizeof(T), alignof(T));
            if(!memory) return nullptr;
            return new(memory) T(std::forward<Args>(args)...);
        }




};


#endif