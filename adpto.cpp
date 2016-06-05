#include <iostream>
#include <vector>
#include <stack>

//#define DEBUG 1
//#define DEBUG_MOVES 1

using namespace std;

int SIZE, K;

long long board[128][128] = {{0}};


struct Move {
    int x1, y1, x2, y2;

    Move() { };

    Move(int x1, int y1, int x2, int y2) :
            x1(x1), y1(y1), x2(x2), y2(y2) { }

    bool contains(int x, int y) {
        if (x1 == x and y1 == y)
            return true;
        if (x2 == x and y2 == y)
            return true;
        return false;
    };
};

bool operator==(Move a, Move b) {
    if (a.x1 == b.x1 and a.y1 == b.y1 and a.x2 == b.x2 and a.y2 == b.y2)
        return true;
    if (a.x1 == b.x2 and a.y1 == b.y2 and a.x2 == b.x1 and a.y2 == b.y1)
        return true;
    return false;
}

struct state {
    vector<Move> moves;
    Move previous_move;
    bool start = false;
};

void printMove(Move move) {
    cout << move.x1 << " " << move.y1 << " " << move.x2 << " " << move.y2 << " " << endl;
}

void printMoves(vector<Move> moves) {
    Move move;
    for (int i = 0; i < moves.size(); i++) {
        printMove(moves[i]);
    }
}

void printMovesReverse(vector<Move> moves) {
    Move move;
    for (int i = moves.size() - 1; i >= 0; i--) {
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

vector<Move> find_possible_moves(vector<Move> moves, Move move) {
#ifdef DEBUG_MOVES
    cout << "-------" << endl;
    printMoves(moves);
#endif

    for (vector<Move>::iterator it = moves.begin(); it != moves.end(); it++) {
        if ((*it).contains(move.x1, move.y1)) {
            moves.erase(it);
            it--;
        } else if ((*it).contains(move.x2, move.y2)) {
            moves.erase(it);
            it--;
        }
    }

#ifdef DEBUG_MOVES
    cout << "-------" << endl;
    printMoves(moves);
    cout << "-------" << endl;
#endif

    // place that gets empty:

    int x1 = move.x1;
    int y1 = move.y1;

    // horizontally
    int l, r;
    long long left = 0, right = 0;
    for (l = x1 - 1; l >= 0; l--) {
        if (board[l][y1] != 0) {
            left = board[l][y1];
            break;
        }
    }
    for (r = x1 + 1; r < SIZE; r++) {
        if (board[r][y1] != 0) {
            right = board[r][y1];
            break;
        }
    }
    if (left == right and right != 0) {
        Move temp_move(l, y1, r, y1);
        moves.push_back(temp_move);
        temp_move = Move(r, y1, l, y1);
        moves.push_back(temp_move);
    }

    // vertically
    int u, d;
    long long up = 0, down = 0;
    for (u = y1 - 1; u >= 0; u--) {
        if (board[x1][u] != 0) {
            up = board[x1][u];
            break;
        }
    }
    for (d = y1 + 1; d < SIZE; d++) {
        if (board[x1][d] != 0) {
            down = board[x1][d];
            break;
        }
    }
    if (up == down and down != 0) {
        Move temp_move(x1, u, x1, d);
        moves.push_back(temp_move);
        temp_move = Move(x1, d, x1, u);
        moves.push_back(temp_move);
    }

    // diag right-down
    int a, b;
    long long lu = 0, rd = 0;
    for (a = 0; x1 - a >= 0 and y1 - a >= 0; a++) {
        if (board[x1 - a][y1 - a] != 0) {
            lu = board[x1 - a][y1 - a];
            break;
        }
    }
    for (b = 0; x1 + b < SIZE and y1 + b < SIZE; b++) {
        if (board[x1 + b][y1 + b] != 0) {
            lu = board[x1 + b][y1 + b];
            break;
        }
    }
    if (lu == rd and lu != 0) {
        Move temp_move(x1 - a, y1 - a, x1 + b, y1 + b);
        moves.push_back(temp_move);
        temp_move = Move(x1 + b, y1 + b, x1 - a, y1 - a);
        moves.push_back(temp_move);
    }

    // diag left-down
    long long ld = 0, ru = 0;
    for (a = 0; x1 - a >= 0 and y1 + a < SIZE; a++) {
        if (board[x1 - a][y1 + a] != 0) {
            ld = board[x1 - a][y1 + a];
            break;
        }
    }
    for (b = 0; x1 + b < SIZE and y1 - b >= 0; b++) {
        if (board[x1 + b][y1 - b] != 0) {
            ru = board[x1 + b][y1 - b];
            break;
        }
    }
    if (ru == ld and ld != 0) {
        Move temp_move(x1 - a, y1 + a, x1 + b, y1 - b);
        moves.push_back(temp_move);
        temp_move = Move(x1 + b, y1 - b, x1 - a, y1 + a);
        moves.push_back(temp_move);
    }


    // place that has now new value

    int x2 = move.x2;
    int y2 = move.y2;
    long long val = board[x2][y2];

    // horizontally
    for (int l = x2 - 1; l >= 0; l--) {
        if (board[l][y2] == val) {
            Move temp_move(l, y2, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, l, y2);
            moves.push_back(temp_move);
            break;
        } else if (board[l][y2] != 0) {
            break;
        }
    }
    for (int r = x2 + 1; r < SIZE; r++) {
        if (board[r][y2] == val) {
            Move temp_move(r, y2, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, r, y2);
            moves.push_back(temp_move);
            break;
        } else if (board[r][y2] != 0) {
            break;
        }
    }

    // vertically
    for (int u = y2 - 1; u >= 0; u--) {
        if (board[x2][u] == val) {
            Move temp_move(x2, u, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, x2, u);
            moves.push_back(temp_move);
            break;
        } else if (board[x2][u] != 0) {
            break;
        }
    }
    for (int d = y2 + 1; d < SIZE; d++) {
        if (board[x2][d] == val) {
            Move temp_move(x2, d, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, x2, d);
            moves.push_back(temp_move);
            break;
        } else if (board[x2][d] != 0) {
            break;
        }
    }

    // diag left-up
    for (int a = 1; x2 - a >= 0 and y2 - a >= 0; a++) {
        if (board[x2 - a][y2 - a] == val) {
            Move temp_move(x2 - a, y2 - a, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, x2 - a, y2 - a);
            moves.push_back(temp_move);
            break;
        } else if (board[x2 - a][y2 - a] != 0) {
            break;
        }
    }

    // diag left-down
    for (int a = 1; x2 - a >= 0 and y2 + a < SIZE; a++) {
        if (board[x2 - a][y2 + a] == val) {
            Move temp_move(x2 - a, y2 + a, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, x2 - a, y2 + a);
            moves.push_back(temp_move);
            break;
        } else if (board[x2 - a][y2 + a] != 0) {
            break;
        }
    }

    // diag right-up
    for (int a = 1; x2 + a < SIZE and y2 - a >= 0; a++) {
        if (board[x2 + a][y2 - a] == val) {
            Move temp_move(x2 + a, y2 - a, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, x2 + a, y2 - a);
            moves.push_back(temp_move);
            break;
        } else if (board[x2 + a][y2 - a] != 0) {
            break;
        }
    }

    // diag right-down
    for (int a = 1; x2 + a < SIZE and y2 + a < SIZE; a++) {
        if (board[x2 + a][y2 + a] == val) {
            Move temp_move(x2 + a, y2 + a, x2, y2);
            moves.push_back(temp_move);
            temp_move = Move(x2, y2, x2 + a, y2 + a);
            moves.push_back(temp_move);
            break;
        } else if (board[x2 + a][y2 + a] != 0) {
            break;
        }
    }

#ifdef DEBUG_MOVES
    printMoves(moves);
    cout << "-------" << endl;
#endif


    return moves;
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

    while (true) {
        state current_state = game.top();
        game.pop();

        cin >> z;

        if (current_state.moves.empty()) {
            undoMove(current_state.previous_move);

#ifdef DEBUG
            cout << "=========================" << endl;
            printMove(current_state.previous_move);
            cout << "=========================" << endl;
            printBoard();
            cout << "=========================" << endl;
#endif

            state temp = game.top();
            game.pop();
            temp.moves = find_possible_moves();
            game.push(temp);

            n++;
            continue;
        }

        temp_move = current_state.moves.back();
        current_state.moves.pop_back();
        game.push(current_state);

        state new_state;
        new_state.previous_move = temp_move;
        makeMove(temp_move);

#ifdef DEBUG
        cout << "-----------" << current_state.moves.size() << "---------------" << endl;
        printMove(temp_move);
        cout << "--------------------------" << endl;
        printBoard();
        cout << "--------------------------" << endl;
#endif

        n--;
        new_state.moves = find_possible_moves(current_state.moves, temp_move);
        game.push(new_state);

        if (n <= K)
            break;

    }

    vector<Move> game_moves;

    while (!game.empty()) {
        if (game.size() > 1)
            game_moves.push_back(game.top().previous_move);
        game.pop();
    }

    printMovesReverse(game_moves);

#ifdef DEBUG
    printBoard();
#endif
}