#pragma once

#include <array>
#include <optional>
#include <utility>
#include <memory>


namespace OmokEngine {
    enum class Stone {
        BLANK = 0, 
        BLACK = 1, 
        WHITE = 2
    };


    enum class Turn {
        BLACK = 1,
        WHITE = 2
    };
    

    Stone turnToStone(Turn turn) {
        if (turn == Turn::WHITE) return Stone::WHITE;
        return Stone::BLACK;
    }

    Turn inverseTurn(Turn turn) {
        if (turn == Turn::WHITE) return Turn::BLACK;
        return Turn::WHITE;
    }


    template<int BOARD_SIZE>
    class BoardState {
    private:
        std::array<std::array<Stone, BOARD_SIZE>, BOARD_SIZE> stones;
        Turn turn;
        int ply = 0;

    public:
        BoardState(Turn turn) : turn(turn) {
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    stones[i][j] = Stone::BLANK;
                }
            }
        }

        BoardState(const BoardState& boardState) {
            stones = boardState.stones;
            turn = boardState.turn;
        }

        std::optional<Stone> getStoneAt(int x, int y) const {
            if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
                return std::nullopt;
            }
            return stones[y][x];
        }

        bool checkWinningStoneAt(int x, int y) const {
            // 5개 연속인지 확인합니다
            const int dx[] = {-1,0,1,1};
            const int dy[] = {1,1,1,0};

            for (int d = 0; d < 4; ++d) {
                int cnt = 1;
                
                int cx = x;
                int cy = y;
                
                std::optional<Stone> centerStone = getStoneAt(x, y);

                if (!centerStone) {
                    return false;
                }

                for (int cap = 0; cap < 4; ++cap) {
                    cx += dx[d];
                    cy += dy[d];
                    std::optional<Stone> getResult = getStoneAt(cx, cy);

                    if (!getResult || *getResult != *centerStone) {
                        break;
                    }
                    ++cnt;
                }

                cx = x;
                cy = y;
                for (int cap = 0; cap < 4; ++cap) {
                    cx -= dx[d];
                    cy -= dy[d];
                    std::optional<Stone> getResult = getStoneAt(cx, cy);
                    
                    if (!getResult || *getResult != *centerStone) {
                        break;
                    }
                    ++cnt;
                }

                if (cnt == 5) {
                    return true;
                }
            }

            return false;
        }

        enum class PutResult {
            FAIL, SUCCESS, GAME_END
        };

        PutResult putStoneAt(int x, int y) {
            // 성공 여부를 반환
            std::optional<Stone> getResult = getStoneAt(x, y);

            if (!getResult || *getResult != Stone::BLANK) {
                return PutResult::FAIL;
            }

            stones[y][x] = turnToStone(turn);
            turn = inverseTurn(turn);
            ++ply;
            
            bool winning = checkWinningStoneAt(x, y);
            if (winning) 
                return PutResult::GAME_END;
            return PutResult::SUCCESS;
        }

        bool removeStoneAt(int x, int y) {
            // 제거 성공 여부를 반환
            std::optional<Stone> getResult = getStoneAt(x, y);

            if (!getResult) {
                return false;
            }

            stones[y][x] = Stone::BLANK;
            turn = inverseTurn(turn);
            --ply;
            return true;
        }

        const std::array<std::array<Stone, BOARD_SIZE>, BOARD_SIZE>& getStones() const {
            return stones;
        }

        bool operator==(const BoardState& other) const {
            if (ply != other.ply) return false;
            return stones == other.stones;
        }

        Turn getTurn() const {
            return turn;
        }
    };
}