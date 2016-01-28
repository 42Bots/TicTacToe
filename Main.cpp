#include <iostream>     /* cin, cout */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>       /* vector */

using namespace std;

// Variable declarations
int next_move = 5; // "X" = 2; "O" = 5;
int next_player = 1; // -1 = human, 1 = AI, 0 = no move;
int board[9] = {0,0,0,0,0,0,0,0,0};
const int board_size = 3;
bool game_running = true;
const int X = 2;
const int O = 5;
const int inf = 999;
const int win = 100;

// Function declarations
bool makeMove(int node, int brd[], int player);
int checkLine(int line, int brd[]);
void showBoard(const int brd[]);
char gridChar(int i);
void drawBoard(int brd[]);
int switchPlayer(int player);
int checkScore(int brd[], int player);
bool boardFull(int brd[]);
int randomMove(int brd[], int player);
int betterMove(int brd[], int player);
int forkMove(int brd[], int player);
void humanMove(int brd[], int player);
int miniMaxScore(int brd[], int player, bool maxPlayer, int depth);
int bestMove(int brd[], int player);


int main () {
    while(game_running){
        drawBoard(board);

        if (next_move==X) {cout<<"Next move: X"<<endl;}
        else if (next_move == O){cout<<"Next move: O"<<endl;}

        int node = 0;

        if (next_player == -1){
            humanMove(board, next_move);
        }

        else if (next_player == 1){
            node = bestMove(board,next_move);
            //node = betterMove(board,next_move);
            //node = aiMove(board, next_move);
            //node = randomMove(board,next_move);
            makeMove(node, board, next_move);
        }

        int score = checkScore(board, next_move);
        next_player = -next_player;

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

bool makeMove(int node, int brd[], int player) {
    bool good_move = false;

    if (brd[node]==0){
        brd[node]=player;
        good_move=true;
    }
    else {
        cout<<"Invallid move!"<<endl;
    }

    return good_move;
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
    int score = -1; // game still in progress

    for (int i = 0; i<8; i++){
        if(checkLine(i, brd)== (3 * player)){
            score = win;
        }

        else if(checkLine(i, brd)== (3 * switchPlayer(player))){
            score = -win; // loss is opposite of win
        }

        else if (boardFull(brd)){
            score = 0; // draw
        }
    }
    return score;
}

bool boardFull(int brd[]){
    bool full = true;
    for (int m=0; m < 9; m++){
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

     //first move
     int sum = 0;
     int corners[4] = {0,2,6,8};

     for (int n = 0; n<8; n++) {
        sum = sum + brd[n];
     }

     //if the board is empty, take a corner
     if (sum == 0){
        srand (time(NULL));
        int n = rand() % 4;
        mv = corners[n];
        return mv;
     }

     else if (sum == X || sum == O){
        if (brd[4]==0){
            mv = 4;
            return mv;
        }

        else {
            srand (time(NULL));
            int n = rand() % 4;
            mv = corners[n];
            return mv;
        }
     }

     //win move
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

     //block move
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

     //fork move
     if(forkMove(brd,player)>-1){
        mv = forkMove(brd, player);
        cout<<"Best Move: "<<mv<<endl;
        return mv;
     }

     //random move
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

void humanMove(int brd[], int player){
    bool done = false;

    while (done == false){
        cout<<"Your turn! Select an empty square (1-9)."<<endl;
        int box;
        cin>>box;
        box = box-1; // allow user input to be 1-9 rather than 0-8
        done = makeMove(box, brd, player);
    }
}

int miniMaxScore(int brd[], int player, bool maxPlayer, int depth) {

    // if game over, return score
    int score = checkScore(brd, player);
    if (score > 0){
        score = score-depth;
        return score;
    }
    else if (score <0 && score!=-1){
        score = score+depth;
        return score;
    }
    else if (score==0) {
        return score;
    }

    // if game not over, find the scores of each available move
    else {
        //int p = switchPlayer(player);
        bool m = !maxPlayer;
        int d = depth + 1;

        if (m) {
            score = -inf;

            for (int i=0; i<9; i++){
                if (brd[i]==0){
                    brd[i] = player;
                    int s = miniMaxScore(brd, player, m, d);
                    if (s > score){
                        score = s;
                    }
                    brd[i] = 0;
                }
            }
        }

        else {
            score = inf;

            for (int i=0; i<9; i++){
                if (brd[i]==0){
                    brd[i] = switchPlayer(player);
                    int s = miniMaxScore(brd, player, m, d);
                    if (s < score) {
                        score = s;
                    }
                    brd[i] = 0;
                }
            }
        }
    }
    return score;
}

int bestMove(int brd[], int player) {
    int score = -inf;
    int mv = -1;

   for (int i=0; i<9; i++){
        if (brd[i]==0){
            brd[i] = player;
            int s = miniMaxScore(brd, player, true, 0);
            if (s > score){
                mv = i;
                score = s;
            }

            // Add some variety in the play.
            // If two moves have the same score, the AI will randomly choose.
            else if (s == score){
                srand (time(NULL));
                int n = rand() % 2;
                if (n>0){
                    mv = i;
                }

            }
            brd[i] = 0;
        }
   }
   cout<<"Best Move: "<<mv<<" Score: "<<score<<endl;
   return mv;
}
