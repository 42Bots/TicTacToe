#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>       /* vector */

using namespace std;

// Variable declarations
int next_move = 2; // "X" = 2; "O" = 5;
int board[9] = {0,0,0,0,0,0,0,0,0};
const int board_size = 3;
bool game_running = true;
const int X = 2;
const int O = 5;

// Function declarations
void makeMove(int node, int brd[], int player);
int checkLine(int line, int brd[]);
void showBoard(const int brd[]);
char gridChar(int i);
void drawBoard(int brd[]);
int switchPlayer(int player);
int checkScore(int brd[], int player);
bool boardFull(int brd[]);
int gameScore(int brd[], int player);
int randomMove(int brd[], int player);
int betterMove(int brd[], int player);
int forkMove(int brd[], int player);

int main () {
    while(game_running){
        drawBoard(board);
        if (next_move==X) {cout<<"Next move: X - "<<next_move<<endl;}
        else if (next_move == O){cout<<"Next move: O - "<<next_move<<endl;}

        int node = 0;
        //cin>>node;
        //node = randomMove(board,next_move);
        node = betterMove(board,next_move);
        makeMove(node, board, next_move);
        int score = gameScore(board, next_move);
        if (score!=-1){
            game_running = false;
            drawBoard(board);
            cout<<"GAME OVER!"<<endl;
            }
        next_move = switchPlayer(next_move);
        cout<<"----------------------------------------"<<endl;
    }
}

int lines[8][3] = {
    {0,1,2}, {3,4,5},{6,7,8}, // rows
    {0,3,6}, {1,4,7},{2,5,8}, // columns
    {0,4,8}, {2,4,6}          // diagonals
};

void makeMove(int node, int brd[], int player) {
    if (brd[node]==0){
        brd[node]=player;
    }
    else {
        cout<<"Invallid move!"<<endl;
    }
}

int checkLine(int line, int brd[]) {
    int sum = 0;

    for (int i=0; i<board_size; i++) {
        sum = sum+brd[lines[line][i]];
    }

    return sum;
}

void showBoard(const int brd[]){
    for (int m=0; m<board_size; m++){
        for (int n=0; n<board_size; n++) {
            char p = '-';
            if (brd[m*board_size+n]==2) {
                p = 'X';
            }

            else if (brd[m*board_size+n]==5) {
                p = 'O';
            }

            cout<<p<<' ';
        }
        cout<<endl;
    }
}

char gridChar(int i) {
    switch(i) {
        case 2:
            return 'X';
        case 5:
            return 'O';
        default:
            return ' ';
    }
}

void drawBoard(int brd[]) {
    printf("\n");
    printf(" %c | %c | %c\n",gridChar(brd[0]),gridChar(brd[1]),gridChar(brd[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(brd[3]),gridChar(brd[4]),gridChar(brd[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(brd[6]),gridChar(brd[7]),gridChar(brd[8]));
    printf("\n");
}

int switchPlayer(int player){
    int mv = 0;
    if (player == 2) {
        mv=5;
    }
    else mv = 2;
    return mv;
}

int checkScore(int brd[], int player) {
    int score = -1;

    for (int i = 0; i<8; i++){
        if(checkLine(i, brd)== (3 * player)){
            score = 10;
        }

        else if(checkLine(i, brd)== (3 * switchPlayer(player))){
            score = -10;
        }

        else if (boardFull(brd)){
            score = 0;
        }
    }
    return score;
}

bool boardFull(int brd[]){
    bool full = true;
    for (int m=0; m < (board_size * board_size); m++){
        if (brd[m]==0) {
            full = false;
            }
        }
    return full;
}

int gameScore(int brd[], int player){
    int game_score = -1;

    for (int i=0; i<8; i++){
        int score = checkLine(i, brd);
        if (score == (player * 3)){
            game_score = 10;
        }

        else if (score == (switchPlayer(player)*3)){
            game_score = -10;
        }
    }

    if (game_score==-1 && boardFull(brd)){
        game_score = 0;
    }

    return game_score;
};

int randomMove(int brd[], int player){
    vector<int> freeNodes;
    srand (time(NULL));
    for (int i=0; i<9; i++){
        if (brd[i]==0){
            freeNodes.push_back(i);
        }
    }
    int r = freeNodes.size();
    int n = rand() % r;
    int m = freeNodes[n];
    return m;
}

int betterMove(int brd[], int player){
     int mv = -1;

     for (int n = 0; n<8; n++){
        int s = checkLine(n, brd);
        if (s == (2 * player)){
            cout<<"I can win!"<<endl;
            for (int k=0; k<3; k++){
                if (board[lines[n][k]] == 0){
                    mv = lines[n][k];
                    cout<<"Best Move: "<<mv<<endl;
                    return mv;
                }
            }
        }
     }

     for (int n = 0; n<8; n++){
        int s = checkLine(n, brd);
        if (s == (2 * switchPlayer(player))){
            cout<<"I have to block!"<<endl;
            for (int k=0; k<3; k++){
                if (board[lines[n][k]] == 0){
                    mv = lines[n][k];
                    cout<<"Best Move: "<<mv<<endl;
                    return mv;
                }
            }
        }
     }

     if(forkMove(brd,player)>-1){
        mv = forkMove(brd, player);
        cout<<"Best Move: "<<mv<<endl;
        return mv;
     }

     if (mv==-1){
        cout<<"Playing random!"<<endl;
        mv = randomMove(brd, player);
     }
     cout<<"Best Move: "<<mv<<endl;
     return mv;
}

int forkMove(int brd[], int player){
    int fork_move = -1;
    for (int i = 0; i<9; i++){
        int l = 0;
        if (brd[i]==0){
            makeMove(i, brd, player);
            for (int m = 0; m<8; m++){
                if (checkLine(m, brd)== (2 * player)){
                    l = l+1;
                }
            }
            if (l>1){
               fork_move = i;
               cout<<"I can fork!"<<endl;
            }
            brd[i]=0;
            return fork_move;
        }
    }
    return fork_move;
}
