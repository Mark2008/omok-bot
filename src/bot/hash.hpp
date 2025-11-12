#pragma once
#include <array>
#include <random>
#include "../omok_engine.hpp"


namespace OmokBot {
    template<int BOARD_SIZE, int POSSIBLE>
    class ZobristHash {
        std::array<std::array<std::array<size_t, POSSIBLE>, BOARD_SIZE>, BOARD_SIZE> table;
        std::mt19937 gen;
        std::uniform_int_distribution<size_t> distribution;

    public:
        ZobristHash() {
            gen = std::mt19937(1234);
            distribution = std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max());
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    for (int k = 0; k < POSSIBLE; ++k) {
                        table[i][j][k] = randomInt();
                    }
                }
            }
        }

        size_t randomInt() {
            return distribution(gen);
        }

        size_t operator()(const OmokEngine::BoardState<BOARD_SIZE>& board) const {
            size_t h = 0;
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    OmokEngine::Stone stone = board.getStones()[i][j];
                    if (stone != OmokEngine::Stone::BLANK) {
                        h ^= table[i][j][(int)stone];
                    }
                }
            }
            return h;
        }
    };
}