#include <allocators/LinearAllocator.hpp>
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
    LinearAllocator arena(1024 * 1024);

    void* mem = arena.Allocate(sizeof(Player), alignof(Player));
    Player* P1 = new (mem) Player(42, 100.0f);

    std::cout << "Health: " << P1->Health << "\n";

    P1->~Player();
    arena.Reset();

    return 0;
}