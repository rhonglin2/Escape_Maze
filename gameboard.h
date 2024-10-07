#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        int HeroRow; // Hero's position row
	    int HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        /*
            void getHeroPosition(size_t& row, size_t& col)

            getter for Hero's position, set row and col
                which are passed-by-reference
            to Hero's row (HeroRow) and Hero's column (HeroCol)
        */
        void getHeroPosition(size_t& row, size_t& col)
        {
            row = HeroRow;
            col = HeroCol;    
        }

        /*
            void setHeroPosition(size_t& row, size_t& col)

            setter for Hero's position, set HeroRow and HeroCol
            to row and col which are passed by value
        */
        void setHeroPosition(size_t row, size_t col)
        {
            HeroRow = row;
            HeroCol = col;
        }

        /*
            void findHero()

            updates the Hero's position
                int HeroRow;
                int HeroCol;

            finds the Hero on the board and updates
                HeroRow and HeroCol with the Hero's updated position
            if Hero cannot be found on board, then set Hero's position to (-1, -1)
        */
        void findHero()
        {
            size_t row = -1;
            size_t col = -1;

            for(size_t r = 0; r < numRows; r++)
            {
                for(size_t c = 0; c < numCols; c++)
                {
                    if(board(r,c)->isHero())
                    {
                        row = r;
                        col = c;
                    }
                }
            }

            setHeroPosition(row, col);
        }

        
        /*
            bool makeMoves(char HeroNextMove)
            
            Allow user to input their next move 
            Get the attempted move position for the Hero
            Move the hero, catching/resolving any potential collision exceptions...
                attempted move out-of-bounds: change row &/or col to stay on board
                attempted move into a barrier: change row &/or col to stay off barrier
                attempted move to the exit: remove hero from board, hero escaped!
                attempted move into a hole: remove hero from board, hero did not escape
                attempted move to a baddie: remove hero from board, hero did not escape
                attempted move to an empty space: actual move is the attempted move
            For each baddie (regular Monster, super Monster, or Bat) on the board...
                check that this baddies hasn't already moved this round
                get its attempted move position
                move the baddie, catching/resolving any potential collision exceptions...
                    attempted move out-of-bounds: change row &/or col to stay on board
                    attempted move into a barrier: change row &/or col to stay off barrier
                    attempted move to the exit: change row &/or col to stay off exit
                    attempted move into a hole: remove baddie from board
                    attempted move to hero: remove hero from board, hero did not escape
                    attempted move to a baddie: ignore attempted move, no position change
                    attempted move to an empty space: actual move is the attempted move 
            
            return true if Hero is still on board at the end of the round
            return false if Hero is not on board at the end of the round
        */
        bool makeMoves(char HeroNextMove) {
            // Hero's movement:
            // determine where hero proposes to move to
            size_t newR, newC;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);
            try { // hero attempts to move out-of-bounds in rows
                if (newR < 0 || newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                }
            }
            catch (runtime_error& excpt) { // catch out-of-bounds in rows
                cout << excpt.what() << endl;
                newR = HeroRow;
                cout << "Changing row for Hero position to stay in-bounds" << endl;
                findHero();
            }

            try { // hero attempts to move out-of-bounds in columns
                if (newC < 0 || newC >= numCols) {
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
                }
            }
            catch (runtime_error& excpt) { // catch out-of-bounds in columns
                cout << excpt.what() << endl;
                newC = HeroCol;
                cout << "Changing column for Hero position to stay in-bounds" << endl;
                findHero();
            }

            try { // hero attempts to move into a barrier
                if (board(newR, newC)->isBarrier()) {
                    throw runtime_error("Hero trying to move into a barrier");
                }
            }
            catch(runtime_error& excpt) { // catch moving into barrier
                cout << excpt.what() << endl;
                newC = HeroCol;
                // checks if the hero would move into another barrier,
                // stay at current position
                if (board(newR, HeroCol)->isBarrier()) {
                    newR = HeroRow;
                }
                // checks if the hero would move into the Abyss,
                // thus the player loses
                if (board(newR, HeroCol)->isHole()) {
                    cout << "Uh Oh! Hero has fallen into the Abyss..." << endl;
                    freeCell(HeroRow, HeroCol);
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                    findHero();
                    return false;
                }
                // checks if the hero would move into a Baddie,
                // thus the player loses
                if (board(newR, HeroCol)->isBaddie()) {
                    cout << "Uh Oh! Hero walked into a Baddie..." << endl;
                    freeCell(HeroRow, HeroCol);
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                    findHero();
                    return false;
                }
            }

            try { // hero attempts to move into an Escape Ladder
                if (board(newR, newC)->isExit()) {
                    throw runtime_error("Hero Escaped!");
                }
            }
            catch (runtime_error& excpt) { // catch moving into escape ladder
                // the hero escaped, player wins
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                wonGame = true;
                findHero();
                return false;
            }
            
            try { // hero attempts to move into an Abyss cell
                if (board(newR, newC)->isHole()) {
                    throw runtime_error("Uh Oh! Hero has fallen into the Abyss...");
                }
            }
            catch (runtime_error& excpt) { // catch moving into Abyss cell
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                findHero();
                return false;
            }

            try { // hero attempts to move into a Baddie
                if (board(newR, newC)->isBaddie()) {
                    throw runtime_error("Uh Oh! Hero has walked into a Baddie...");
                }
            }
            catch (runtime_error& excpt) { // catch moving into a Baddie
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                findHero();
                return false;
            }

            try { // hero attempts to move into free space
                if (board(newR, newC)->isSpace()) {
                    throw runtime_error("Hero moved");
                }
            }
            catch (runtime_error& excpt) { // catch moving into free space
                // switch and update the position of the hero with the empty cell
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                freeCell(newR, newC);
                board(newR, newC) = new Hero(newR, newC);
                findHero();
            }

            // Baddie's movement:
            size_t newMR, newMC;
            // reset the moved bool for each Baddie on the board
            //    allowing each Baddie to move this turn
            for (size_t r = 0; r < numRows; r++) {
                for(size_t c = 0; c < numCols; c++) {
                    if (board(r,c)->isBaddie()) {
                        board(r,c)->setMoved(false);
                    }
                }
            }
            // loop through the entire board
            // for every Baddie, 
            // check if they have moved
            // if they haven't, check for valid moves
            for (size_t r = 0; r < numRows; r++) {
                for(size_t c = 0; c < numCols; c++) {
                    if (board(r,c)->isBaddie()) {
                        if (!board(r,c)->getMoved()) {
                            board(r,c)->attemptMoveTo(newMR,newMC,HeroRow,HeroCol);

                            try { // Baddie attempts to move out-of-bounds in rows
                                if (newMR < 0 || newMR >= numRows) { 
                                    throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                                }
                            }
                            catch (runtime_error& excpt) { // catch Baddie moving out-of-bounds in rows
                                newMR = r;
                            }

                            try { // Baddie attempts to move out-of-bounds in columns
                                if (newMC < 0 || newMC >= numCols) {
                                    throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                                }
                            }
                            catch (runtime_error& excpt) { // catch Baddie moving out-of-bounds in columns
                                newMC = c;
                            }

                            try { // Baddie attempts to move into a barrier
                                if (board(newMR, newMC)->isBarrier()) {
                                    throw runtime_error("Baddie trying to move into a barrier");
                                }
                            }
                            catch(runtime_error& excpt) { // catch Baddie moving into a barrier
                                newMC = c;
                                // for diagonal movements:
                                //    if the new row is blocked by another object
                                //      (escape ladder, baddie, or barrier)
                                //      keep the Baddie in its current position
                                if(board(newMR, c)->isExit() || board(newMR, c)->isBaddie() || board(newMR, c)->isBarrier()) {
                                    newMR = r;
                                }
                                //    if the new row is a hole
                                //      the Baddie would fall into the hole
                                if(board(newMR, c)->isHole()) {
                                    cout << "Yay! A Baddie fell into the Abyss!" << endl;
                                    freeCell(r, c);
                                    board(r, c) = new Nothing(r, c);
                                }
                            }

                            try { // Baddie attempts to move into an Escape Ladder
                                if (board(newMR, newMC)->isExit()) {
                                    throw runtime_error("Baddie attempting to move onto Escape Ladder");
                                }
                            }
                            catch (runtime_error& excpt) {
                                newMC = c;
                                // for diagonal movements:
                                //    if the new row is blocked by another object
                                //      (a baddie, or barrier)
                                //      keep the Baddie in its current position
                                if ((board(newMR, c)->isBarrier() || board(newMR, c)->isBaddie())) {
                                    newMR = r;
                                }
                                //    if the new row is a hole
                                //      the Baddie would fall into the hole
                                if (board(newMR, c)->isHole()) {
                                    cout << "Yay! A baddie fell into the Abyss!" << endl;
                                    freeCell(r, c);
                                    board(r, c) = new Nothing(r, c);
                                }
                            }
                            
                            try { // Baddie attempts to move into an Abyss cell
                                if (board(newMR, newMC)->isHole()) {
                                    throw runtime_error("Yay! A baddie fell into the Abyss!");
                                }
                            }
                            catch (runtime_error& excpt) { // catch Baddie attempting to move into an Abyss cell
                                freeCell(r, c);
                                board(r, c) = new Nothing(r, c);
                            }

                            try { // Baddie attempts to move into a Baddie
                                if (board(newMR, newMC)->isBaddie()) {
                                    throw runtime_error("Baddie walked into another Baddie...");
                                }
                            }
                            catch (runtime_error& excpt) { // catch Baddie moving into another Baddie
                                newMC = c;
                                // for diagonal movements:
                                //    if the new row is blocked by another object
                                //      (escape ladder, baddie, or barrier)
                                //      keep the Baddie in its current position
                                if(board(newMR, c)->isBarrier() || board(newMR, c)->isExit() || board(newMR, c)->isBaddie()) {
                                    newMR = r;
                                }
                                //    if the new row is a hole
                                //      the baddie would fall into the hole
                                if(board(newMR, c)->isHole()) {
                                    cout << "Yay! A baddie fell into the Abyss!" << endl;
                                    freeCell(r, c);
                                    board(r, c) = new Nothing(r, c);
                                }
                            }

                            try { // monster attempts to move into hero
                                if (board(newMR, newMC)->isHero()) {
                                    throw runtime_error("Uh Oh! Baddie walked into Hero");
                                }
                            }
                            catch (runtime_error& excpt) { // catch baddie moving into the hero
                                // free the hero and create an empty spot on the hero's position
                                // switch the empty spot with the monster
                                freeCell(newMR, newMC);
                                board(newMR, newMC) = new Nothing(newMR, newMC);
                                BoardCell* temp = board(newMR, newMC);
                                board(newMR, newMC) = board(r, c);
                                board(r, c) = temp;
                                board(newMR, newMC)->setPos(newMR, newMC);
                            }

                            // lets the baddie move
                            // switches an empty spot with the baddie
                            if (board(newMR, newMC)->isSpace()) {
                                BoardCell* temp = board(newMR, newMC);
                                board(r, c)->setPos(newMR, newMC);
                                board(newMR, newMC) = board(r, c);
                                board(r, c) = temp;
                                board(newMR,newMC)->setMoved(true);
                            }
                        }
                    }
                }
            }

            // At the end of the round, look for the hero
            findHero();
            
            // If the hero isn't found
            //      HeroRow and HeroCol = -1
            // Return false
            if(HeroRow == -1 && HeroCol == -1) {
                return false;
            }

            return true;
        }
};

#endif //_GAMEBOARD_H