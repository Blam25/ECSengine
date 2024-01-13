//
// Created by Martvall on 13/01/2024.
//
#include "GameEngine.cpp"

int main(int argc, char *argv[]) {

    initGame();

    auto player = createPlayer(400, 400);
    createNPC(00, 200);
    createNPC(300, 100);

    launchGame(argc, argv);

    return 0;
}