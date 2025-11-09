#include <iostream>
#include <memory>
#include "omok_engine.hpp"


int main() {
    using Board = OmokEngine::BoardState<15>;
    using Turn = OmokEngine::Turn;
    using Stone = OmokEngine::Stone;
    using PutResult = Board::PutResult;

    std::shared_ptr<Board> board = std::make_shared<Board>(Turn::WHITE);

    for (int i=7; i<12; ++i) {
        for (int j=4; j<6; ++j) {
            PutResult putResult = board->putStoneAt(i, j);
            std::cout << "Print stone at: (" << i << ", " << j << ")\n";

            if (putResult == PutResult::GAME_END) {
                std::cout << "Game End!\n";
            }
        }
    }

    return 0;
}