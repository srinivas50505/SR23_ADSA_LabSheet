#include <stdio.h>
#include <stdbool.h>

#define N 8 // Change N for different board sizes

// Check if placing queen at board[row][col] is safe
bool isSafe(int board[N][N], int row, int col) {
    int i, j;
    // Check this column on upper rows
    for (i = 0; i < row; i++)
        if (board[i][col] == 1)
            return false;

    // Check upper left diagonal
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j] == 1)
            return false;

    // Check upper right diagonal
    for (i = row, j = col; i >= 0 && j < N; i--, j++)
        if (board[i][j] == 1)
            return false;

    return true;
}

// Print solution board
void printBoard(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf(" %c ", board[i][j] ? 'Q' : '.');
        printf("\n");
    }
    printf("\n");
}

bool solveNQueensUtil(int board[N][N], int row) {
    if (row == N) {
        printBoard(board);
        return true; // Change to false if want all solutions
    }

    bool res = false;
    for (int col = 0; col < N; col++) {
        if (isSafe(board, row, col)) {
            board[row][col] = 1;
            res = solveNQueensUtil(board, row + 1) || res;
            board[row][col] = 0; // Backtrack
        }
    }
    return res;
}

void solveNQueens() {
    int board[N][N] = {{0}};
    if (!solveNQueensUtil(board, 0))
        printf("No solution exists\n");
}

int main() {
    solveNQueens();
    return 0;
}
