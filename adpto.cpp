#include <iostream>
#include <vector>

using namespace std;

int SIZE, K;

long long board[128][128] = {{0}};


struct Move {
    int x1, y1, x2, y2;

    Move() { };

    Move(int x1, int y1, int x2, int y2) :
            x1(x1), y1(y1), x2(x2), y2(y2) { }
};

struct state {
    vector<Move> moves;
    Move previous_move;
};


vector<Move> find_possible_moves() {
    vector<Move> moves;
    long long curr;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            curr = board[i][j];
            if (curr != 0) {
                // horizontally
                for (int k = j + 1; k < SIZE; k++) {
                    if (board[i][k] == curr) {
                        Move temp_move(i, j, i, k);
                        moves.push_back(temp_move);
                        break;
                    } else if (board[i][k] != 0) {
                        break;
                    }
                }
                // vertically
                for (int k = i + 1; k < SIZE; k++) {
                    if (board[k][j] == curr) {
                        Move temp_move(i, j, k, j);
                        moves.push_back(temp_move);
                        break;
                    } else if (board[k][j] != 0) {
                        break;
                    }
                }
                // diagonal right
                for (int k = 1; i + k < SIZE && j + k < SIZE; k++) {
                    if (board[i + k][j + k] == curr) {
                        Move temp_move(i, j, i + k, j + k);
                        moves.push_back(temp_move);
                        break;
                    } else if (board[i + k][j + k] != 0) {
                        break;
                    }
                }
                // diagonal left
                for (int k = 1; i + k < SIZE && j - k >= 0; k++) {
                    if (board[i + k][j - k] == curr) {
                        Move temp_move(i, j, i + k, j - k);
                        moves.push_back(temp_move);
                        break;
                    } else if (board[i + k][j - k] != 0) {
                        break;
                    }
                }
            }
        }
    }
    return moves;
}

void printMoves(vector<Move> moves) {
    Move move;
    for (int i = 0; i < moves.size(); i++) {
        move = moves[i];
        cout << move.x1 << " " << move.y1 << " " << move.x2 << " " << move.y2 << " " << endl;
    }
}

int main() {

    cin >> SIZE >> K;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> board[i][j];
        }
    }

    vector<Move> moves = find_possible_moves();
    printMoves(moves);

}