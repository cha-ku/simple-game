#include "GameState/GameState.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main() {
    GameState game;
    game.Initialize();
    game.Run();
    game.Destroy();
    return 0;
}
