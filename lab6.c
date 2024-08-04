#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void readWordPuzzle(const int Size, char puzzle[][Size]);
char* readWord(int* wordSize);
void printWordPuzzle(const int Size, char puzzle[][Size]);
void printWord(char* word, const int wordSize);

void search(const int Size, char puzzle[][Size], const int wordSize,
            char* word);
bool inBounds(int row, int col, const int Size);

int main(void) {
  const int Size = 20;
  char puzzle[Size][Size];
  readWordPuzzle(Size, puzzle);
  int wordSize = 0;
  char* word = readWord(&wordSize);

  printWordPuzzle(Size, puzzle);
  printf("The word you are looking for is\n");
  printWord(word, wordSize);
  printf("\n");
  printf("Do you want to search? (Y or N)\n");
  char isSearch;
  scanf(" %c", &isSearch);
  if (isSearch == 'Y' || isSearch == 'y') {
    search(Size, puzzle, wordSize, word);
  }
  free(word);
  return 0;
}

char* readWord(int* wordSize) {
  printf("How many characters are there in the word?\n");
  scanf("%d", wordSize);
  char* word = (char*)malloc(*wordSize * sizeof(char));

  printf("What is the word that you are looking for?\n");
  scanf("%s", word);

  return word;
}

void readWordPuzzle(const int Size, char puzzle[][Size]) {
  printf("Please enter the word puzzle:\n");
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      scanf(" %c", &puzzle[i][j]);
    }
  }
}

void printWordPuzzle(const int Size, char puzzle[][Size]) {
  printf("The word puzzle entered is\n");
  for (int i = 0; i < Size; i++) {
    for (int j = 0; j < Size; j++) {
      printf("%c ", puzzle[i][j]);
    }
    printf("\n");
  }
}

void printWord(char* word, const int wordSize) {
  for (int i = 0; i < wordSize; i++) {
    printf("%c ", word[i]);
  }
}

// checks if the row/col is in bounds (not outer edges)
bool inBounds(int row, int col, const int Size) {
  return (row >= 0 && row < Size && col >= 0 && col < Size);
}

bool check1D(const int Size, char puzzle[][Size], const int wordSize,
             char* word, int rowLoc, int colLoc, int dir) {
  /*
  NW N NE
  W     E
  SW S SE

  SE, S, SW, W, NW, N, NE, E
  stores the position of the other 8 directions relative to the main/first
  character found
  */
  int x[] = {1, 1, 1, 0, -1, -1, -1, 0};
  int y[] = {1, 0, -1, -1, -1, 0, 1, 1};

  // stores position of current letter on the word puzzle
  int newRow = rowLoc;
  int newCol = colLoc;

  // go through each letter of the WORD and compares with the current letter on
  // WORD PUZZLE
  for (int i = 1; i < wordSize; i++) {
    newRow = newRow + x[dir];  // row index position of new letter relative to
                               // main letter in a particular direction
    newCol = newCol + y[dir];  // col index position of new letter relative to
                               // main letter in a particular direction

    // check if the new letter (direction varied) matches the next letter in the
    // WORD, if they dont match return false so it can exit function, will be
    // called again with anew direction otherwise for loop will continue as long
    // as letters match, and will return TRUE if it exits loop successfully
    if (!inBounds(newRow, newCol, Size) || puzzle[newRow][newCol] != word[i]) {
      return false;
    }
  }
  return true;
}

char* directionText(int dir) {
  char* directions[] = {"south-east", "south", "south-west", "west",
                        "north-west", "north", "north-east", "east"};
  return directions[dir];
}

/*
wordFound bool variable = FALSE
outer 2 for loops go through each letter in the WORD PUZZLE (row and then
column)
compares that letter with the first letter of the WORD if they match go
through another for loop that checks the letters in each of the 8 directions
surrounding the letter through the check1D function (returns a bool value of the
final answer - if word can be found in 1D) YES
      - print message
      - change dir, r, c to max values so that it stops going through loop
      - change wordFound bool variable = TRUE

after going through nested for loops and if statements, if the word is not found
wordFound = FALSE then print word not found message
*/
void search(const int Size, char puzzle[][Size], const int wordSize,
            char* word) {
  bool wordFound = false;
  // for loop goes through entire puzzle and tries to find first letter
  for (int r = 0; r < Size; r++) {
    for (int c = 0; c < Size; c++) {
      if (word[0] == puzzle[r][c]) {
        for (int dir = 0; dir < 8; dir++) {
          if (check1D(Size, puzzle, wordSize, word, r, c, dir)) {
            printf(
                "%s can be found at row , col = (%d, %d) in the %s "
                "direction.\n",
                word, r + 1, c + 1, directionText(dir));
            dir = 7;
            r = Size - 1;
            c = Size - 1;
            wordFound = true;
          }
        }
      }
    }
  }
  if (!wordFound) {
    printf("%s cannot be found!", word);
  }
}
