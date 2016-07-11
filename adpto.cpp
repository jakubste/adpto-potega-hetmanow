#include <iostream>
#include <vector>
#include <stack>
#include <iomanip>
#include <algorithm>
#include <unistd.h>

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
    cout << "     ";
    for (int i = 0; i < SIZE; i++) {
        cout << setw(3) << i << " ";
    }
    cout << endl;
    cout << "     ";
    for (int i = 0; i < SIZE; i++) {
        cout << "---" << " ";
    }
    cout << endl;

    for (int i = 0; i < SIZE; i++) {
        cout << setw(3) << i << "| ";
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j])
                cout << setw(3) << board[i][j] << " ";
            else
                cout << "   " << " ";
        }
        cout << endl;
    }
}

vector<Move> delete_moves_crossing(vector<Move> moves, int x, int y) {
    for (vector<Move>::iterator it = moves.begin(); it != moves.end(); it++) {
        //horizontal
        if (it->x1 == x and x == it->x2) {
            if ((it->x1 < x and x < it->x2) or (it->x1 > x and x > it->x2)) {
                moves.erase(it--);
            }
        }
        //vertical
        if (it->y1 == y and y == it->y2) {
            if ((it->y1 < y and y < it->y2) or (it->y1 > y and y > it->y2)) {
                moves.erase(it--);
            }
        }
        //diagonal
        if (abs(it->x1 - x) == abs(it->y1 - y)) {
            if (abs(it->x2 - x) == abs(it->y2 - y)) {
                moves.erase(it--);
            }
        }
    }
    return moves;
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

vector<Move> find_moves_when_place_gets_empty(int x, int y) {
    vector<Move> moves;

    // horizontally
    int l, r;
    long long left = 0, right = 0;
    for (l = x - 1; l >= 0; l--) {
        if (board[l][y] != 0) {
            left = board[l][y];
            break;
        }
    }
    for (r = x + 1; r < SIZE; r++) {
        if (board[r][y] != 0) {
            right = board[r][y];
            break;
        }
    }
    if (left == right and right != 0) {
        Move temp_move(l, y, r, y);
        moves.push_back(temp_move);
        temp_move = Move(r, y, l, y);
        moves.push_back(temp_move);
    }

    // vertically
    int u, d;
    long long up = 0, down = 0;
    for (u = y - 1; u >= 0; u--) {
        if (board[x][u] != 0) {
            up = board[x][u];
            break;
        }
    }
    for (d = y + 1; d < SIZE; d++) {
        if (board[x][d] != 0) {
            down = board[x][d];
            break;
        }
    }
    if (up == down and down != 0) {
        Move temp_move(x, u, x, d);
        moves.push_back(temp_move);
        temp_move = Move(x, d, x, u);
        moves.push_back(temp_move);
    }

    // diag right-down
    int a, b;
    long long lu = 0, rd = 0;
    for (a = 0; x - a >= 0 and y - a >= 0; a++) {
        if (board[x - a][y - a] != 0) {
            lu = board[x - a][y - a];
            break;
        }
    }
    for (b = 0; x + b < SIZE and y + b < SIZE; b++) {
        if (board[x + b][y + b] != 0) {
            lu = board[x + b][y + b];
            break;
        }
    }
    if (lu == rd and lu != 0) {
        Move temp_move(x - a, y - a, x + b, y + b);
        moves.push_back(temp_move);
        temp_move = Move(x + b, y + b, x - a, y - a);
        moves.push_back(temp_move);
    }

    // diag left-down
    long long ld = 0, ru = 0;
    for (a = 0; x - a >= 0 and y + a < SIZE; a++) {
        if (board[x - a][y + a] != 0) {
            ld = board[x - a][y + a];
            break;
        }
    }
    for (b = 0; x + b < SIZE and y - b >= 0; b++) {
        if (board[x + b][y - b] != 0) {
            ru = board[x + b][y - b];
            break;
        }
    }
    if (ru == ld and ld != 0) {
        Move temp_move(x - a, y + a, x + b, y - b);
        moves.push_back(temp_move);
        temp_move = Move(x + b, y - b, x - a, y + a);
        moves.push_back(temp_move);
    }

    return moves;

}

vector<Move> find_possible_moves_when_place_gets_new_value(int x, int y) {
    vector<Move> moves;

    long long val = board[x][y];

    // horizontally
    for (int l = x - 1; l >= 0; l--) {
        if (board[l][y] == val) {
            Move temp_move(l, y, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, l, y);
            moves.push_back(temp_move);
            break;
        } else if (board[l][y] != 0) {
            break;
        }
    }
    for (int r = x + 1; r < SIZE; r++) {
        if (board[r][y] == val) {
            Move temp_move(r, y, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, r, y);
            moves.push_back(temp_move);
            break;
        } else if (board[r][y] != 0) {
            break;
        }
    }

    // vertically
    for (int u = y - 1; u >= 0; u--) {
        if (board[x][u] == val) {
            Move temp_move(x, u, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, x, u);
            moves.push_back(temp_move);
            break;
        } else if (board[x][u] != 0) {
            break;
        }
    }
    for (int d = y + 1; d < SIZE; d++) {
        if (board[x][d] == val) {
            Move temp_move(x, d, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, x, d);
            moves.push_back(temp_move);
            break;
        } else if (board[x][d] != 0) {
            break;
        }
    }

    // diag left-up
    for (int a = 1; x - a >= 0 and y - a >= 0; a++) {
        if (board[x - a][y - a] == val) {
            Move temp_move(x - a, y - a, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, x - a, y - a);
            moves.push_back(temp_move);
            break;
        } else if (board[x - a][y - a] != 0) {
            break;
        }
    }

    // diag left-down
    for (int a = 1; x - a >= 0 and y + a < SIZE; a++) {
        if (board[x - a][y + a] == val) {
            Move temp_move(x - a, y + a, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, x - a, y + a);
            moves.push_back(temp_move);
            break;
        } else if (board[x - a][y + a] != 0) {
            break;
        }
    }

    // diag right-up
    for (int a = 1; x + a < SIZE and y - a >= 0; a++) {
        if (board[x + a][y - a] == val) {
            Move temp_move(x + a, y - a, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, x + a, y - a);
            moves.push_back(temp_move);
            break;
        } else if (board[x + a][y - a] != 0) {
            break;
        }
    }

    // diag right-down
    for (int a = 1; x + a < SIZE and y + a < SIZE; a++) {
        if (board[x + a][y + a] == val) {
            Move temp_move(x + a, y + a, x, y);
            moves.push_back(temp_move);
            temp_move = Move(x, y, x + a, y + a);
            moves.push_back(temp_move);
            break;
        } else if (board[x + a][y + a] != 0) {
            break;
        }
    }
    return moves;
}

vector<Move> find_possible_moves(vector<Move> moves, Move move, bool reverse = false) {
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

    vector<Move> new_moves;

    if (reverse) {
        moves = delete_moves_crossing(moves, move.x1, move.y1);
        // places that has now new value
        new_moves = find_possible_moves_when_place_gets_new_value(move.x1, move.y1);
        moves.insert(moves.end(), new_moves.begin(), new_moves.end());
        new_moves = find_possible_moves_when_place_gets_new_value(move.x2, move.y2);
        moves.insert(moves.end(), new_moves.begin(), new_moves.end());

    } else {
        // place that gets empty:
        new_moves = find_moves_when_place_gets_empty(move.x1, move.y1);
        moves.insert(moves.end(), new_moves.begin(), new_moves.end());

        // place that has now new value
        new_moves = find_possible_moves_when_place_gets_new_value(move.x2, move.y2);
        moves.insert(moves.end(), new_moves.begin(), new_moves.end());
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

    stack<state> game;
    game.push(start_state);

    Move temp_move;

    char z;

    while (true) {
#ifdef DEBUG
        cout << "********************************* obecnie ruchów: " << game.size() << endl;
#endif

        state current_state = game.top();
        game.pop();

        usleep(150000);

        if (current_state.moves.empty()) {
            undoMove(current_state.previous_move);

#ifdef DEBUG
            cout << "=======cofam=============" << endl;
            printMove(current_state.previous_move);
            cout << "=========================" << endl;
            printBoard();
            cout << "=========================" << endl;
#endif

            n++;
            continue;
        }

        random_shuffle(current_state.moves.begin(), current_state.moves.end());

        temp_move = current_state.moves.back();
        current_state.moves.pop_back();
        game.push(current_state);

        state new_state;
        new_state.previous_move = temp_move;
        makeMove(temp_move);

#ifdef DEBUG
        cout << "-----------wykonuje-------" << endl;
        printMove(temp_move);
        cout << "--------------------------" << endl;
        printBoard();
        cout << "--------------------------" << endl;
#endif

        n--;
        vector<Move> beka = find_possible_moves(current_state.moves, temp_move);
        random_shuffle(beka.begin(), beka.end());
        new_state.moves = beka;
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
