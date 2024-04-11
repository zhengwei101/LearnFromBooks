//  Code for Beginning C++ Through Game Progrmming(4th Edition)
//  tic-tac-toe.cpp
//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//global constants
const char X = 'X'; //棋子
const char O = 'O'; //另一个棋子
const char EMPTY = ' '; //棋盘上的空方格
const char TIE = 'T'; //和棋
const char NO_ONE = 'N'; // 用于表示还没有任何一方胜出的字符

//function prototypes
void instructions(); //显示游戏指南
char askYesNo(string question); //询问是或否，接受一个问题作为参数,返回'y'或'n'
int askNumber(string question, int high, int low=0);//询问一定范围内的数字，返回low到high之间的数字
char humanPiece();//确定玩家的棋子，返回'X'或'O'
char opponent(char piece);//计算给定棋子的应对棋子，接受'X'或'O'，返回'X'或'O'
void displayBoard(const vector<char> &board);//在屏幕上显示棋盘
char winner(const vector<char> &board);//确定，游戏的胜者，返回'X','O','T','N'
bool isLegal(const vector<char> &board, int move);//确定一招棋的合法性
int humanMove(const vector<char>& board, char human);//获取人类玩家的一招棋
int computerMove(vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

int main()
{
    int move;
    const int NUM_SQUARES = 9;
    vector<char> board(NUM_SQUARES, EMPTY);
    instructions();
    char human = humanPiece();
    char computer = opponent(human);
    char turn = X;
    displayBoard(board);
    while (winner(board) == NO_ONE) {
        if(turn == human){
            move = humanMove(board, human);
            board[move] = human;
        }
        else{
            move = computerMove(board, computer);
            board[move] = computer;
        }
        displayBoard(board);
        turn = opponent(turn);
    }
    announceWinner(winner(board), computer, human);
    return 0;
}

void instructions(){
    cout << "Welcome to the ultimate man-machine showdown: Tic-Tac-Toe.\n";
    cout << "--where human brain is pit against silicon processor\n\n";
    cout << "Make you move known by entering a number, 0-8. The number\n";
    cout << "corresponds to the desired board position, as illustrated:\n\n";
    cout << " 0 | 1 | 2\n";
    cout << " ---------\n";
    cout << " 3 | 4 | 5\n";
    cout << " ---------\n";
    cout << " 6 | 7 | 8\n\n";
    cout << "Prepare yourself, human. The battle is about to begin.\n\n";
}

char askYesNo(string question){
    char response;
    do
    {
        cout << question << " (y/n): ";
        cin >> response;
    } while(response != 'y' && response != 'n');
    return response;
}

int askNumber(string question, int high, int low){
    int number = 0;
    do {
        cout << question << " (" << low << "-" << high << "): ";
        cin >> number;
    }while(number > high || number < low);
    return number;
}

char humanPiece(){
    char go_first = askYesNo("Do you require the first move?");
    if(go_first == 'y')
    {
        cout << "\nThen take the first move. You will need it.\n";
        return X;
    }
    else
    {
        cout << "\nYour bravery will be your undoing...I will go first.\n";
        return O;
    }
}

char opponent(char piece){
    if(piece == X){
        return O;
    }
    else{
        return X;
    }
}

void displayBoard(const vector<char> &board){
    cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
    cout << "\n\t" << "---------";
    cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
    cout << "\n\t" << "---------";
    cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
    cout << "\n\n";
}

char winner(const vector<char> &board){
    const int WINNING_ROWS[8][3] = {
        {0,1,2},
        {3,4,5},
        {6,7,8},
        {0,3,6},
        {1,4,7},
        {2,5,8},
        {0,4,8},
        {2,4,6}
    };
    
    const int TOTAL_ROWS = 8;
    for(int row=0; row<TOTAL_ROWS; ++row)
    {
        if((board[WINNING_ROWS[row][0]] != EMPTY) &&
           (board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
           (board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
        {
            return board[WINNING_ROWS[row][0]];
        }
    }
    
    if(count(board.begin(), board.end(), EMPTY) == 0)
        return TIE;
    
    return NO_ONE;
}

bool isLegal(const vector<char> &board, int move){
    return board[move] == EMPTY;
}

int humanMove(const vector<char>& board, char human){
    cout << "Now It's " << human << " To Move\n";
    int move = askNumber("Where will you move?", ((int)board.size()-1));
    while(!isLegal(board, move))
    {
        cout << "\nThat square is already occpied, foolish human.\n";
        move = askNumber("Where will you move?", ((int)board.size() -1));
    }
    cout << "Fine...\n";
    return move;
}

int computerMove(vector<char> board, char computer){
    unsigned int move = 0;
    bool found = false;
    //if computer can win on next move, that's the move to make
    while (!found && move < board.size()) {
        if(isLegal(board, move))
        {
            board[move] = computer;
            found = (winner(board) == computer);
            board[move] = EMPTY;
        }
        if(!found)
        {
            ++move;
        }
    }
    
    //otherwise, if human can win on next move, that's the move to make
    if(!found)
    {
        move = 0;
        char human = opponent(computer);
        while (!found && move < board.size()) {
            if(isLegal(board, move))
            {
                board[move] = human;
                found = (winner(board) == human);
                board[move] = EMPTY;
            }
            if(!found)
            {
                ++move;
            }
        }
    }
    
    //otherwise, moving to the best open square is the move to make
    if(!found)
    {
        move = 0;
        unsigned int i = 0;
        const int BEST_MOVES[] = {4,0,2,6,8,1,3,5,7};
        //pick best open square
        while (!found && i<board.size()) {
            move = BEST_MOVES[i];
            if(isLegal(board, move))
            {
                found = true;
            }
            ++i;
        }
    }
    cout << "I shall take square number " << move << endl;
    return move;
}

void announceWinner(char winner, char computer, char human){
    if(winner == computer)
    {
        cout << winner << "'s won!\n";
        cout << "As I predicted, human, I am triumphant once more -- proof\n";
        cout << "that computers are superior to humans in all regards.\n";
    }
    else if( winner == human)
    {
        cout << winner << "'s won!\n";
        cout << "No, no! It cannot be! Somehow you tricked me, human.\n";
        cout << "But never again! I, the computer, so swear it!\n";
    }
    else
    {
        cout << "It's a tie.\n";
        cout << "You were most lucky, human, and somehow managed to tie me.\n";
        cout << "Celebrate...for this is the best you will ever achieve.\n";
    }
}

