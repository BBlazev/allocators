#include <allocators/LinearAllocator.hpp>
#include <allocators/PoolAllocator.hpp>
#include <iostream>

struct Player {
    int ID;
    float Health;

    Player(int i, float h) : ID{i}, Health{h} {
        std::cout << "Player " << ID << " spawned.\n";
    }

    ~Player() {
        std::cout << "Player " << ID << " destroyed.\n";
    }
};

int main() {
    // LinearAllocator arena(1024 * 1024);

    // void* mem = arena.Allocate(sizeof(Player), alignof(Player));
    // Player* P1 = new (mem) Player(42, 100.0f);

    // Player* P2 = arena.Allocate<Player>(22, 90.0f);

    // std::cout << "Health: " << P1->Health << "\n";
    // std::cout << "Health: " << P2->Health << "\n";

    // std::cout << sizeof(Player) << "\n";
    // std::cout << sizeof(arena) << "\n";

    // std::cout << "Size: " << arena.get_size() << "\n";
    // std::cout << "Used: " << arena.get_used() << "\n";
    // std::cout << "Remaining: " << arena.get_remaining() << "\n";

    // P1->~Player();
    // P2->~Player();
    // arena.Reset();

    PoolAllocator<Player> PlayerPool(5);
    Player* P1 = PlayerPool.Allocate(1,100.0f);
    Player* P2 = PlayerPool.Allocate(2,32.9f);

    PlayerPool.Deallocate(P1);

    Player* P3 = PlayerPool.Allocate(3, 100.0f);

    

    return 0;
}