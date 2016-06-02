#include <iostream>

using namespace std;

int SIZE, K;

long long board[128][128] = {{0}};

int main() {

    cin >> SIZE >> K;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> board[i][j];
        }
    }

}