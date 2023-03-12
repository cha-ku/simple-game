#include "GameState/GameState.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
    GameState game;
    game.Initialize();
    game.Run();
    game.Destroy();
    return 0;
}
