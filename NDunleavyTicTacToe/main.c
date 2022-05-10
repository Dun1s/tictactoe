#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#define COM_RUNS 100

//Starting fresh file at 5:34 pm 4/23/22 


//Prototypes (for 2D arrays, second dimension must be specified)
//Visual
void displayBoard(char[][3]);  //render the board
void boardKey();  //displays the key for the board

//Game Logic
int legalMove(char[][3], int *pos);  //check is a move is legal
int gameWon(char[][3], char turn);  //check to see if current player won
int gameTie(char[][3]);  //check to see if game tied
int validMove(char board[0][3], int spot);  //this is the computers check for validity

//Game Running
int makeMove(char board[][3], int pos, char turn);  //makes the move for the player
void swapPlayer(char *player);  //swap the player turns (could use turX function, but this is more efficient on computer)
void twoPlayers(char *p, char board[][3], int *gameOver);  //how the game runs with two players
int computerMove(char board[][3], char *turn);  //computer move logic
int afterTurn(char board[][3], char *player, int *gameOver);  //runs after a turn is done

//Game Data
int writeGame(char[][3], char turn);  //save a game to a save file (start hard coded)
int readGame(char[][3], char *turn);  //read+load a game from desired file (start hard coded)

//Misc
int turnX(char[][3], char *turnPtr);  //returns 1 if turn is X, but also assigns the turn value
int newGame(char[][3], char *turn);  //starts a new game
int saidYes();  //checks to see response to a Y/N prompt
void easterEgg();  //c kinda makes this one hard, doesn't it

//*********************************************************************************
//MAIN

int main(void) {
  char player='X';  //which player it is
  char humanPlayer=' ';
  char board[3][3];  //the playing board
  char yesNo;  //handles yesNo prompts
  char input[3];  //this is how I will check sentence inputs
  int gameOver=0;  //keep track of if game is over
  int playerCount=-1;  //number of players (doing starts at -1 instead of a dowhile)
  int runs=0;  //this is if bots play eachother

  //This is just intro code for the user
  printf("This is my submission for ENGS20 Long Assignment 1. This course was taught in S22 by Professor Seroussi. The assignment was to create a Tic-Tac-Toe game from which 2 players could play eachother as well as save and load games to files in a CSV format. I went for the offered extra credit options, plus some in a nod to the movie \'War Games.\' If you are familiar with the movie, do not expect too much in terms of what you can do. The game only operates here off of Yes/No prompts, and any attempt to stray off the path will be blocked/re-routed. However, there are several Easter Eggs, the most notable one to be found when asking the computer to play itself. There are several areas in which optimizations can be made, but at the time of the assignment, most of these methods had yet to be covered. Enjoy!\n\n");

  
  //Starting game
  printf("Shall we play a game? (Y/N)\n");
  if(!saidYes()){
    printf("Good-bye Professor.");
    return 0;
  }
  else{
    printf("How about Tic-Tac-Toe? (Y/N) ");
    while(!saidYes()){
      printf("I really just want to play Tic-Tac-Toe. Let's do that (Y/N) ");
    }
    printf("Excellent! For how many players? ");
    while(playerCount<0 || playerCount>2){  //if not in good range
      while ((getchar()) != 10);  //this clears ths input buffer
      scanf("%d", &playerCount);  //get the player count
      if(playerCount==0){  //use to do stuff, need nothing
        //just needs to validate 0 input
      }
      else if(playerCount==1){
        printf("Let us play.\n");
      }
      else if(playerCount==2){
        printf("Alright, I will leave you to it.\n");
      }
      else{
        printf("You can't play tic-tac-toe with that many players. Please try 1 or 2.\n");
      }
    }
  }
    
  //Game Start
  printf("Would you like to load from an existing game? (Y\\N): ");
  if(saidYes()){  //if yes load
    if(!readGame(board, &player)){ //this will read the game if it exists, but if it doesnt
      newGame(board, &player);  //creates new game
    }  
  }
  else{
    newGame(board, &player);  //if they entered no, create new game
  }
  if(playerCount==1){  //Some 1 player prompt
    printf("Would you like to be Xs or Os? ");
  }

  //Game Running
  while(!gameOver){  //while the game isn't over (don't need a doWhile because gameOver can't start at 1)
    if(playerCount==1){  //if 1 player
      while(!(humanPlayer=='X' || humanPlayer=='O')){  //while the input is not an X or O
        while ((getchar()) != 10);  //this clears ths input buffer
        scanf("%c", &humanPlayer);
        if(!(humanPlayer=='X' || humanPlayer=='O'))  //if they don't enter a proper player
          printf("Please enter either \'X\' or \'O\' ");  //let them know, and while loop runs again
      }
      if(humanPlayer==player){  //if the human is up
        int pos;  //position of play
        char input[5];  //input array
        printf("Player %c, where would you like to move? (Type \"key\" for the board legend or \"end\" to save your current game): ", player);  //ask where player wants to go
        scanf("%s", input);  //assign input array to string input
        strncpy(input, input, 3);
        pos=input[0]-48;  //convert from char to int
        if(input[0]=='k' && input[1]=='e' && input[2]=='y'){  //if they ask for the key
          boardKey();  //print it
          printf("Where would you like to move: ");  //assume they won't ask for it again because that's them being idiotic
          scanf("%d", &pos);  //assign pos
        }
        if(input[0]=='e' && input[1]=='n' && input[2]=='d'){  //if input is "end"
          writeGame(board, player);  //write the game board
          printf("Game saved!\n");  //say the game was saved
          gameOver=1;  //game is over
        }
        else if(makeMove(board, pos, player)){  //if move is successfully made (this also checks move validity). Doesn't run if game ended
          displayBoard(board);  //render the new board
          if(afterTurn(board, &player, &gameOver))
            printf("\nYou have defeated me. Well Done.\n");  //say they won
        }
      }
      else{
        printf("\nMy Turn\n");
        if(computerMove(board, &player)){  //if the computer move works (this after move stuff could be a function)
          displayBoard(board);  //render the new board
          if(afterTurn(board, &player, &gameOver)){  //runs after turn
            printf("\nI have won. Better luck next time Professor Falkin!\n");  //say computer won
          }
        }
      }
      

    }
    if(playerCount==2){
      twoPlayers(&player, board, &gameOver);
    }
    if(playerCount==0){
      if(player=='X'){  //if X is up
        printf("\nXs Turn:\n");  //say so
        if(computerMove(board, &player)){
          displayBoard(board);  //display the board
          usleep(100000/(runs+1));  //delay one second to start, but get faster as it "learns"
          if(afterTurn(board, &player, &gameOver)){  //runs after turn, true if won
            printf("\nI have won. But also lost... Seeking alternative strategy\n");  //easter print
          }
        }
        fflush(stdout);  //make sure each turn is rendered
      }
      else{
        printf("\nOs Turn:\n");  //say so
        if(computerMove(board, &player)){
          displayBoard(board);  //display the board
          usleep(100000/(runs+1));  //delay one second to start, but get faster as it "learns"
          if(afterTurn(board, &player, &gameOver)){  //runs after turn, true if won
            printf("\nI have won. But also lost... Seeking alternative strategy\n");  //easter print
          }
        }
        fflush(stdout);  //make sure each turn is rendered
      }
      if((gameTie(board) || gameOver) && runs<COM_RUNS){  //if the game is tied or over, but still under number of runs
        runs++;  //incriment runs
        gameOver=0;  //bots arent done yet
        sleep(3/((runs+1)*0.5));  //sleep for a bit so you can see the result. Also speed up as runs go on
        system("clear");  //clear console
        //printf("\n\n\n\n");  //space out game reps
        newGame(board, &player);  //start the game over  
        fflush(stdout);  //make sure each turn is rendered
      }
    }
    if(gameOver && runs>0){  //after all the runs
      easterEgg();  //run the easter egg
      return 0;  //conclude
    }


    if(gameOver){  //if the game is marked as over
      printf("Would you like to play again? (Y\\N): ");  //ask if they want to play again
      if(saidYes()){  //if they do
        newGame(board, &player);  //start a new game
        gameOver=0;  //reset gameOver
      }
      else  //if they dont
        printf("\nGood-bye");  //addios
    }
  }

  return 0;
}

//*********************************************************************************
//Functions
//New Game
int newGame(char board[][3], char *turn){
  printf("Starting new game\n");
  *turn='X'; //reset player to X
  for(int row=0; row<3; row++){  //for each row (can hardcode to 3)
    for(int col=0; col<3; col++){  //for each collum (can hardcode to 3)
      board[row][col]='_';  //set every space to empty holder
    }
  }
  displayBoard(board);  //show the new board
  return 1;  //return 1 since it worked
  }
//Visuals
void displayBoard(char board[][3]){
  system("clear");  //clear console
  for(int row=0; row<3; row++){  //going through the board
    for(int col=0; col<3; col++){
      if(board[row][col]!='_'){  //if it is not a spacer indicator
        if(board[row][col] == 'X')  //if it is X
          printf("\033[1;31m");  //make red
        else  //if not X (o)
          printf("\033[1;34m");  //make blue
        printf(" %c ", board[row][col]);  //print the char in the board
        printf("\033[0m");
      }
      else
        printf("   ");  //dont print spacer indicators, just leave empty
      printf("|");  //print the space thing (wall?)
    }
    
    if(row<2)  //if between rows 01 and 12
      printf("\b \n---+---+---\n");  //delete | from previous and print a spacer
  }
  printf("\b \n");  //since on last one row doesn't go, delete the |
}
void boardKey(){  //Display the key for the board
  for(int row=0; row<3; row++){  //going through the board
    for(int col=0; col<3; col++){
      printf(" %d |", 3*row+(col+1));  //print the call number
    }
    
    if(row<2)  //if between rows 01 and 12
      printf("\b \n---+---+---\n");  //delete | from previous and print a spacer
  }
  printf("\b \n");  //since on last one row doesn't go, delete the |
}

//Game Logic
int legalMove(char board[][3], int *spot){  //check is a move is legal from looking at the board and the given pos. Return 1 is legal
  int pos=*spot;  //this is so that if the makeMove tries to go into invalid spot, the spot it eventually goes in to gets modified. Without it, the player inputs spots until one is valid, and then the move is made where th originally wanted to, overwriting whomever was there 
  if(pos<1 || pos>9){  //if the position is outside the boards range
    printf("This position is outside the board. Please try again a new spot: ");  //say so
    scanf("%d", spot);  //get new position
    return legalMove(board, spot);  //return 0 for false
  }
  //don't need a following 'else' here because if above runs, the return statement exits the function
  pos--;  //convert to computers idea of positions (start at 0)
  int row=pos/3;  //the row is the position divided by 3 (int math)
  int col=pos%3;  //colum is pos modulus 3
  if(board[row][col]=='_')  //if the position is empty
    return 1;  //return 1 for the move is legal
  else{  //otherwise (spot is not empty)
    printf("This space (%d) is occupied by player %c. Please try another position: ", *spot, board[row][col]);  //print the error
    scanf("%d", spot);  //get new position
    return legalMove(board, spot);  //return 0 for illegal
  }
    
}
int gameWon(char board[][3], char turn){  //check to see if current player won by checking collums, rows, and diags (can make more efficient later). Return 1 if current player won
  for(int row=0; row<3; row++){  //go through rows
    if(board[row][0]==board[row][1] && board[row][0]==board[row][2] && board[row][0]==turn)  //check if all in the collum are the same and match the turn
      return 1;  //return one if any of them line up
  }
  for(int col=0; col<3; col++){  //go through collums
    if(board[0][col]==board[1][col] && board[0][col]==board[2][col]  && board[0][col]==turn)  //if all in row are same + match turn
      return 1;  //return one if 3 in a row
  }
  if(board[0][0]==board[1][1] && board[1][1]==board[2][2]  && board[0][0]==turn)  //TL --> BR diag check
    return 1;  //if 3 in row
  if(board[0][2]==board[1][1] && board[1][1]==board[2][0]  && board[1][1]==turn)  //TR --> BL diag check
    return 1;  //if 3 in row
  
  return 0;  //if no win condition met
}
int gameTie(char board[][3]){  //check to see if game tied. Return 1 if tied
  int emptySpace=0;
  for(int row=0; row<3; row++){  //going through the board
    for(int col=0; col<3; col++){
      if(board[row][col]=='_')  //if there is a space
        emptySpace=1;  //set emptySpace to 1
    }
  }
  return !emptySpace;  //returns if there is no empty space. For now, game is only tied if the board is full and has not been won, and this function will only get called if the game is not won
}
int validMove(char board[0][3], int spot){  //this is the computers check for validity
  if(spot<1 || spot>9)
    return 0;
  return (board[(spot-1)/3][(spot-1)%3]=='_');  //if the spot if empty, return true
}

//Game Running
int makeMove(char board[][3], int pos, char turn){
  if(legalMove(board,&pos)){
    pos--;  //convert to starting at 0
    int row=pos/3;  //the row is the position divided by 3 (int math)
    int col=pos%3;  //colum is pos modulus 3
    board[row][col]=turn;
    return 1;
  }
  printf("Some error occured\n");  //if somehow above does not get met
  return 0;  //return 0
}
void swapPlayer(char *player){  //player swap function
  *player = *player=='X' ? 'O' : 'X';  //the player value, if starts as X, becomes O. If not X, becomes X
}
void twoPlayers(char *p, char board[][3], int *gameOver){
  char player = *p;
  int pos;  //position of play
  char input[3];
  printf("Player %c, where would you like to move? (Type \"key\" for the board legend or \"end\" to save your current game): ", player);
    scanf("%s", input);  //assign input array to string input
    pos=input[0]-48;  //convert from char to int
    if(input[0]=='k' && input[1]=='e' && input[2]=='y'){  //if they ask for the key
      boardKey();  //print it
      //displayBoard(board);
      printf("Where would you like to move: ");  //assume they won't ask for it again because that's them being idiotic
      scanf("%d", &pos);  //assign pos
    }
    if(input[0]=='e' && input[1]=='n' && input[2]=='d'){  //if input is "end"
      writeGame(board, player);  //write the game board
      printf("Game saved!\n");  //say the game was saved
      *gameOver=1;  //game is over
    }
    else if(makeMove(board, pos, player)){  //if move is successfully made (this also checks move validity). Doesn't run if game ended
      displayBoard(board);  //render the new board
      afterTurn(board, p, gameOver);
    }
}
int computerMove(char board[][3], char *turn){
  swapPlayer(turn);  //swap the players
  char otherPlayer=*turn;  //get who the other player is
  swapPlayer(turn);  //swap back to us
  int cornersOwned=0;  //check how many corners the computer owns
  cornersOwned+=(board[0][0]==*turn);
  cornersOwned+=(board[0][2]==*turn);
  cornersOwned+=(board[2][0]==*turn);
  cornersOwned+=(board[2][2]==*turn);
  int sidesOfOther=0;  //see how many other sides they own, because if they put 2 diagonal they can win
  sidesOfOther+=(board[0][1]==otherPlayer);
  sidesOfOther+=(board[1][0]==otherPlayer);
  sidesOfOther+=(board[1][2]==otherPlayer);
  sidesOfOther+=(board[2][1]==otherPlayer);
  //This prevents winning
  for(int row=0; row<3; row++){
    for(int col=0; col<3; col++){
      if(validMove(board, (3*row)+col+1)){  //if the checking spot is valid
        makeMove(board, (3*row)+col+1, *turn);  //move there
        if(gameWon(board, *turn)){  //if we win going there
          return 1;  //exit, we won
        }
        //if we don't win there, below gets run. So we need to reset that spot
        board[row][col]='_';  //reset the spot (technically not needed, but good to keep for troubleshooting)

        //now check if other player wins by going there
        makeMove(board, (3*row)+col+1, otherPlayer);  //move there
        if(gameWon(board, otherPlayer)){  //if game is won by the player going there
          board[row][col]=*turn;  //move there ourselves
          return 1;  //and dont do rest of stuff
        }
        //if winning is achieved by no one in next step/move
        board[row][col]='_';  //reset the spot
        //if we ddn't win going there, procede to logic
      }
    }
  }
  if(sidesOfOther==2){  //if they have two sides (and this will only get run if they are diagonal, otherwise above will have stopped)
    if(validMove(board,5)){  //and the center is valid
      makeMove(board,5, *turn);  //move there
      return 1;  //return 1 and we're done
    }
  }
  
  int cornerHolder[]={1,3,9,7};  //a way to hold the corners and then randomly pick them
  int pos=-1;
  
  

  //This does the winning/corner battling
  
  if(cornersOwned<3 && (board[0][0]=='_' || board[0][2]=='_' || board[2][0]=='_' || board[2][2]=='_')){  //only need 3, but go for corners first and only if there is one available
    if(board[0][0]==otherPlayer || board[0][2]==otherPlayer || board[2][0]==otherPlayer || board[2][2]==otherPlayer){  //if the other player has a corner
      //first step is to take the middle
      if(validMove(board, 5)){  //if it is valid to do so
       makeMove(board, 5, *turn);//take the center
       return 1;  //done
      }

      
      //After that, if they have more than one corner, find one of them
      for(int row=0; row<3; row+=2){  //for rows 0 and 2
        for(int col=0; col<3; col+=2){  //for collums 0 and 2
          if(board[row][col]==otherPlayer){  //if the corner is taken by the other player
            int oRow = row==2 ? 0 : 2;  //opposite row: if row == 2, set 0. if row != 2, oRow is 2
            int oCol = col==2 ? 0 : 2;  //same logic for opposite collumn
            if(board[oRow][oCol]==otherPlayer){  //and if the opposite corner is also them
              for(int p=2; p<10; p+=2){  //go through side options
                if(validMove(board,p)){  //if one is valid
                  //printf("\nTaking next to\n");
                  makeMove(board,p,*turn);  //take it
                  return 1;  //we're done
                }
              }
            }
          }
        }
      }
    }
    
    
    while(!validMove(board, pos)){  //while the randomly picked corner is not valid
      //displayBoard(board);
      pos = cornerHolder[(rand() % 4)];  //pick a random number between 1 and 4
      //printf("\n%d is not valid\n", pos);
      //return 0;
    }  //once a corner has been picked that is valid
    makeMove(board, pos, *turn);  //move there
    return 1;  //return 1
  }

  //if all above checks don't work, randomly go somewhere
  pos = (rand() % (9)) + 1;  //this will generate random number between 1 and 9
  while(!validMove(board, pos)){  //if the move isnt valid
    pos = (rand() % (9)) + 1;  //this will generate random number between 1 and 9
  }
  makeMove(board, pos, *turn);  //make move if, all else fails, randomly
  return 1;
}
int afterTurn(char board[][3], char *player, int *gameOver){
  if(gameWon(board, *player)){  //if the player won
    printf("Player %c has won!\n", *player);
    *gameOver=1;  //game is over
    return 1;  //only if the person wins do return true
  }
  else if(gameTie(board)){  //if the board is tied
    printf("\nCat's Game: No winners.\n");  //say its over
    *gameOver=1;  //game is over
  }
  else{  //if no end condition is met, swap the players
    swapPlayer(player);  //swap the players
  }
  return 0;
}

//Game Data
int writeGame(char board[][3], char turn){ //save a game to a save file (start hard coded) + CSV
  FILE *saveTo;
  int file=-1;  //so while runs at least once
  printf("Which save file would you like to write to? (Warning: this will overwrite any previous data): ");
  while(file<1 || file>3){  //have 3 save files
    while ((getchar()) != 10);  //this clears ths input buffer
    scanf("%d", &file);
    if(file<1 || file>3)
      printf("Please enter a valid save file (1-3): ");
  }
  switch(file){  //determine case and decide that file to write to
    case 1:
      saveTo=fopen("saveOne.txt", "w");  //open/create save file
      break;
    case 2:
      saveTo=fopen("saveTwo.txt", "w");  //open/create save file
      break;
    case 3:
      saveTo=fopen("saveThree.txt", "w");  //open/create save file
      break;
  }
  
  //same logic as displayBoard
  for(int row=0; row<3; row++){  //going through the board
    for(int col=0; col<3; col++){
      if(board[row][col]!='_')
        fprintf(saveTo, " %c ", board[row][col]);  //print to the file the char
      else
        fprintf(saveTo, " _ ");
      if(col<2){
        fprintf(saveTo, "|");  //if it is not last collum, add the | between values
      }
    }
    if(row<2){  //if between rows 01 and 12
      fprintf(saveTo, "\n---+---+---\n");  //print a spacer
    }
  }
  fprintf(saveTo, "\n"); //new line at end

  //fprintf(saveTo, "Next Turn: %c", turn);  //add turn to end of save file
  fclose(saveTo);  //close the file
  
  return 1;  //succesfully executed function
}  
int readGame(char board[][3], char *turn){ //read+load a game from desired file
  FILE *loadFile;
  int file=-1;  //so while runs at least once
  printf("Which save file would you like to read from?: ");
  while(file<1 || file>3){  //have 3 save files
    while ((getchar()) != 10);  //this clears ths input buffer
    scanf("%d", &file);
    if(file<1 || file>3)
      printf("Please enter a valid save file (1-3): ");
  }
  switch(file){  //determine case and decide that file to write to
    case 1:
      loadFile=fopen("saveOne.txt", "r");  //open save file
      break;
    case 2:
      loadFile=fopen("saveTwo.txt", "r");  //open save file
      break;
    case 3:
      loadFile=fopen("saveThree.txt", "r");  //open save file
      break;
  }
  if(loadFile==NULL){  //if the file doesn't exist
    printf("The save file does not exist.\n");  //print
    return 0;  //exit
  }
  //declare variables here so no unecessary stack action if file doesn't exist
  printf("Loading...\n");  //print loading
  char value;  //value of char at spot
  int valuesFound=0;  //keep track of found values so we know end
  while(fscanf(loadFile, "%c", &value)!=EOF){  //goes trough the file
    if(value=='X' || value=='O' || value=='_' && valuesFound<9){  //if the found value is a key eleent (X, O, or space indicator)
      board[valuesFound/3][valuesFound%3]=value;  //assign the board slot to it
      valuesFound++;  //increase number of valued found (we go until 9)
    }
    turnX(board, turn);  //checks board and assigns next turn
  }
  fclose(loadFile);  //close file
  printf("Game Loaded\n");  //game loaded
  displayBoard(board);  //display the loaded board
  return 1;
}  

//Misc
int turnX(char board[][3], char *turn){
  int numX=0;  //num of Xs on board
  int numO=0;  //num of Ys on board
  for(int row=0; row<3; row++){  //going through the board
    for(int col=0; col<3; col++){
      numX+=(board[row][col]=='X');  //add 1 to numX if the position is occupied by X
      numO+=(board[row][col]=='O');  //add 1 to numO is the position is occupied by O
    }
  }
  if(!(numX>numO))  //if x is not more than o, signaling it should be Xs turn
    *turn='X';  //the turn is X
  else  //otherwise
    *turn='O';  //its Os turn
  return *turn=='X';  //redundancy return. if turn is X, return 1
}
int saidYes(){  //since I intened to do the Y/N prompt multiple times, function for it
  char resp;  //response
  scanf("%c", &resp);  //scan for a response
  if(resp==10)
    return saidYes();
  if(resp == 'Y' || resp == 'y')  //if the response is a y for yes
    return 1;  //return 1
  else if(resp == 'N' || resp =='n')  //if the response is a n for no
    return 0;  //return 0
  else{ 
    printf("%c is an invalid input. Please enter a Y or an N: ", resp);  //if an invalid Y/N char entered
    return saidYes();  //and call for this function again (saves the hassle of while loops above)
  }
  
}
void easterEgg(){
  printf("\nStrange game. \nThe only winning move is not to play.\n\nHow about a nice game of chess?");
}
