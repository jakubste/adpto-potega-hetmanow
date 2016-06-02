#include <iostream>
#include <vector>
#include <stack>

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
    bool start = false;
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
                        temp_move = Move(i, k, i, j);
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
                        temp_move = Move(k, j, i, j);
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
                        temp_move = Move(i + k, j + k, i, j);
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
                        temp_move = Move(i + k, j - k, i, j);
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

void printMove(Move move) {
    cout << move.x1 << " " << move.y1 << " " << move.x2 << " " << move.y2 << " " << endl;
}

void printMoves(vector<Move> moves) {
    Move move;
    for (int i = 0; i < moves.size(); i++) {
        printMove(moves[i]);
    }
}

void printBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void makeMove(Move move) {
    board[move.x1][move.y1] = 0;
    board[move.x2][move.y2] *= 2;
}

void undoMove(Move move) {
    board[move.x2][move.y2] /= 2;
    board[move.x1][move.y1] = board[move.x2][move.y2];
}

int main() {

    cin >> SIZE >> K;

    int n = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> board[i][j];
            if (board[i][j] != 0) {
                n++;
            }
        }
    }

    state start_state;
    start_state.moves = find_possible_moves();
    start_state.start = true;

    stack<state> game;
    game.push(start_state);

    Move temp_move;

    /*
     * TODO:
     * * board without possible moves
     *
     */

    char z;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        state current_state = game.top();
        game.pop();

        cin >> z;

        cout << game.size() << " " << current_state.moves.size() << endl;

        if (current_state.moves.empty()) {
            undoMove(current_state.previous_move);

            cout << "=========================" << endl;
            printMove(current_state.previous_move);
            cout << "=========================" << endl;
            printBoard();
            cout << "=========================" << endl;

            n++;
            continue;
        }

        temp_move = current_state.moves.back();
        current_state.moves.pop_back();
        game.push(current_state);

        state new_state;
        new_state.previous_move = temp_move;
        makeMove(temp_move);

        cout << "-----------"<< current_state.moves.size() <<"---------------" << endl;
        printMove(temp_move);
        cout << "--------------------------" << endl;
        printBoard();
        cout << "--------------------------" << endl;

        n--;
        new_state.moves = find_possible_moves();
        game.push(new_state);

        if (n <= K)
            break;

    }
#pragma clang diagnostic pop




    //  DEBUG PRINTING

    cout << n << endl;

    vector<Move> game_moves;

    while (!game.empty()) {
        game_moves.push_back(game.top().previous_move);
        game.pop();
    }

    printMoves(game_moves);

    printBoard();

}