#include <iostream>
#include <vector>
#include <stack>

#include <iomanip>
#include <algorithm>
#include <queue>

#define DEBUG 1

using namespace std;

int SIZE, K;

long long board[128][128] = {{0}};

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

pair<int, int> directions[8] = {{-1, 0},
                                {-1, 1},
                                {0,  1},
                                {1,  1},
                                {1,  0},
                                {1,  -1},
                                {0,  -1},
                                {-1, -1}};

struct Move {
    int x1, y1, x2, y2;

    Move() { };

    Move(int x1, int y1, int x2, int y2) :
            x1(x1), y1(y1), x2(x2), y2(y2) { }

};

stack<Move> moves;

struct fig {
    int x, y;
    long long v;

    fig(int x, int y, long long v) :
            x(x), y(y), v(v) { }
};

bool operator<(fig a, fig b) {
    return a.v > b.v;
}

fig find_figure(int x, int y, long long v, int dx, int dy) {
    x += dx;
    y += dy;

    while (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
        if (board[x][y] == v) {
            return fig(x, y, v);
        } else if (board[x][y] != 0) {
            return fig(-1, -1, -1);
        }
        x += dx;
        y += dy;
    }

    return fig(-1, -1, -1);
}

bool find_your_destiny(int curr_num, priority_queue<fig> queens) {

    if (curr_num <= K) {
        return true;
    }

#ifdef DEBUG
    printBoard();
    cout << queens.size() << endl;
#endif

    while (!queens.empty()) {
        fig curr = queens.top();
        queens.pop();
        for (int i = 0; i < 8; i++) {

            pair<int, int> dir = directions[i];

#ifdef DEBUG
            cout << curr.x << " " << curr.y << " " << dir.first << " " << dir.second << endl;
#endif
            curr.v = board[curr.x][curr.y];
            fig hit = find_figure(curr.x, curr.y, curr.v, dir.first, dir.second);
            if (hit.v == -1)
                continue;

            priority_queue<fig> new_queens, temp_queue;
            fig temp_fig(0,0,0);
            while (!queens.empty()) {
                temp_fig = queens.top();
                queens.pop();
                temp_queue.push(temp_fig);
                if (temp_fig.x != hit.x && temp_fig.y != hit.y) {
                    new_queens.push(temp_fig);
                }
            }
            queens = temp_queue;

            board[curr.x][curr.y] = 0;
            board[hit.x][hit.y] *= 2;

            hit.v *= 2;
            new_queens.push(hit);

            if (find_your_destiny(curr_num - 1, new_queens)) {
                moves.push(Move(curr.x, curr.y, hit.x, hit.y));
                return true;
            }

#ifdef DEBUG
            cout << "reversing: " << curr.x << " "<< curr.y << " " << hit.x << " " << hit.y << endl;
            printBoard();
#endif
            board[curr.x][curr.y] = curr.v;
            board[hit.x][hit.y] = curr.v;
#ifdef DEBUG
            printBoard();
#endif

        }
    }
}

int main() {

    cin >> SIZE >> K;

    int n = 0;

    priority_queue<fig> figures;
    fig temp_fig(0, 0, 0);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> board[i][j];
            if (board[i][j] != 0) {
                n++;
                temp_fig.x = i;
                temp_fig.y = j;
                temp_fig.v = board[i][j];
                figures.push(temp_fig);
            }
        }
    }


#ifdef DEBUG
    printBoard();
#endif

    bool out = find_your_destiny(n, figures);

    if (out) {
        while(!moves.empty()) {
            Move move = moves.top();
            moves.pop();
            cout << move.x1 << " " << move.y1 << " " << move.x2 << " " << move.y2 << endl;
        }
    }


#ifdef DEBUG
    printBoard();
#endif
}
