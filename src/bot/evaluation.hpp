#pragma once
#include "abp_model.hpp"
#include "../omok_engine.hpp"

#include <iostream>


#define preprocess \
OmokEngine::Stone last = OmokEngine::Stone::BLANK; \
int series = 0;

#define updateEvaluation \
float mul = (OmokEngine::turnToStone(board.getTurn()) == last) ? 1.f : -1.f; \
evaluation += evalPair[series] * mul;

#define process \
if (last != OmokEngine::Stone::BLANK) { \
    if (last == stones[i][j]) { \
        ++series; \
        std::cout << "sumup" << series; \
    } \
    else { \
        updateEvaluation \
        series = 1; \
    } \
} \
last = stones[i][j];


namespace OmokBot {
    template<int BOARD_SIZE>
    class Evaluation {
    public:
        float operator()(const OmokEngine::BoardState<BOARD_SIZE>& board) {
            // don't know parent node

            static float evalPair[] = {0, 1, 100, 1e5, 1e8, 1e20};

            std::array<std::array<OmokEngine::Stone, BOARD_SIZE>, BOARD_SIZE> stones = board.getStones();

            float evaluation = 0;
            for (int i = 0; i < BOARD_SIZE; ++i) {
                preprocess
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    process
                }
                updateEvaluation
            }

            for (int j = 0; j < BOARD_SIZE; ++j) {
                preprocess
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    process
                }
                updateEvaluation
            }

            for (int oi = 0; oi < BOARD_SIZE; ++oi) {
                int i = oi;
                int j = 0;
                preprocess
                for (; i < BOARD_SIZE; ++i) {
                    ++j;
                    process
                }
                updateEvaluation
            }

            for (int oj = 1; oj < BOARD_SIZE; ++oj) {
                int i = 0;
                int j = oj;
                preprocess
                for (; j < BOARD_SIZE; ++j) {
                    ++j;
                    process
                }
                updateEvaluation
            }

            for (int oi = 1; oi < BOARD_SIZE; ++oi) {
                int i = oi;
                int j = BOARD_SIZE - 1;
                preprocess
                for (; i < BOARD_SIZE; ++i) {
                    --j;
                    process
                }
                updateEvaluation
            }

            for (int oj = 0; oj < BOARD_SIZE; ++oj) {
                int i = 0;
                int j = oj;
                preprocess
                for (; j >= 0; --j) {
                    ++i;
                    process
                }
                updateEvaluation
            }

            return evaluation;
        }

        float operator()(const OmokEngine::BoardState<BOARD_SIZE>& parent, int x, int y) {
            // know parent node's evaluation, add up new evaluation

            // TODO
        }
    };

}