#ifndef _BOARDCELL_H
#define _BOARDCELL_H

using namespace std;

// First, the BoardCell abstract base class 

class BoardCell {
	
    public:  
		BoardCell() {} // default contstructor (do nothing)
        virtual ~BoardCell() {} // destructor (do nothing)
        
        virtual char display( ) = 0; // pure virtual function; this is an abstract base class

        /*
            virtual function, basic template of the attemptMoveTo function for
            the Hero and Baddies
        */
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            newR = getRow();
            newC = getCol();
        }
        
        virtual bool isHero( ) {return false;}
    	virtual bool isBaddie( ) {return false;}
    	virtual bool isSpace( ) {return false;}
        virtual bool isBarrier( ) {return false;}
        virtual bool isHole( ) {return false;}
    	virtual bool isExit( ) {return false;}
    	virtual bool isStatic( ) {return true;}

        virtual void setPower(int num) {}
        virtual int getPower() {return 1;}
        virtual void setNextMove(char inChar) {}
        
        void setMoved(bool m) {moved = m;}
        bool getMoved() {return moved;}
        void setRow(size_t r) {myRow = r;}
        size_t getRow() {return myRow;}
        void setCol(size_t c) {myCol = c;}
        size_t getCol() {return myCol;}
        void setPos(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }

    private:
        size_t myRow; // current row for this board cell in a 2D grid
        size_t myCol; // current column for this board cell in a 2D grid
        bool moved;   // true = this board cell already moved in the current round

}; // BoardCell (abstract base class)


// Then, all the derived classes

class Hero: public BoardCell {
	
    public:
    	Hero(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        
        virtual bool isHero( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return 'H';}
        
        virtual void setNextMove(char inChar) {
            nextMove = inChar;
        }

    	/*
            attemptMoveTo() for Hero:
                The Hero's attempted move is determined by the nextMove data member
                Checks what the inputted attempted move is and then moves accordingly
                    'q' = up and left       'w' = up        'e' = up and right
                    'a' = left              's' = stay      'd' = right
                    'z' = down and left     'x' = down      'c' = down and right
                        interpret ANY other input as 's' = stay
        */
        virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            if(nextMove == 'q') {
                newR = getRow() - 1;
                newC = getCol() - 1;
            }
            else if(nextMove == 'w') {
                newR = getRow() - 1;
                newC = getCol();
            }
            else if(nextMove == 'e') {
                newR = getRow() - 1;
                newC = getCol() + 1;
            }
            else if(nextMove == 'a') {
                newR = getRow();
                newC = getCol() - 1;
            }
            else if(nextMove == 'd') {
                newR = getRow();
                newC = getCol() + 1;
            }
            else if(nextMove == 'z') {
                newR = getRow() + 1;
                newC = getCol() - 1;
            }
            else if(nextMove == 'x') {
                newR = getRow() + 1;
                newC = getCol();
            }
            else if(nextMove == 'c') {
                newR = getRow() + 1;
                newC = getCol() + 1;
            }
            else {
                newR = getRow();
                newC = getCol();
            }
        }

    private:
        char nextMove;

}; // Hero


class Monster: public BoardCell {
    
    public:
		Monster(size_t r, size_t c) {
            setRow(r);
            setCol(c);
            power = 1;
        }
        
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        
        virtual char display( ) {
            if (power == 2) {
                return 'M'; // Super Monster
            } else {
                return 'm'; // Monster
            } 
        }

        virtual void setPower(int num) {
            power = num;
        }

        virtual int getPower() {
            return power;
        }
        
        /*
            attemptToMove() for Monsters:
                The Monster's attempted move is determined by its position related to the Hero
                Determine if the monster is Super Monster or a Regular Monster
                    if it is a Super Monster, it takes 2 steps
                    else it just takes 1 step
                Regular monsters attempt to move:
                    1 step vertically closer to hero
                    (unless already in same column)
                                and
                    1 step horizontally closer to hero
                       (unless already in same row)
                    Super monsters attempt to move:
                        2 step vertically closer to hero
                        (unless already in same column)
                                    and
                        2 step horizontally closer to hero
                        (unless already in same row)
                    
        */
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            int rDiff = hRow - getRow();
            int cDiff = hCol - getCol();

            int step = 1;
            newR = getRow();
            newC = getCol();

            if(power == 2) {
                step = 2;
            }

            // if the difference is > 0, the monster is on the left/above of the hero
            //    add steps to the row and/or column
            // 
            // if the difference is < 0, the monster is on the right/below of the hero
            //    minus steps to the row and/or column
            if(rDiff != 0) {
                if(rDiff > 0) {
                    newR = getRow() + step;
                }
                else if(rDiff < 0) {
                    newR = getRow() - step;
                }
            }
            if(cDiff != 0) {
                if(cDiff > 0) {
                    newC = getCol() + step;
                }
                else if(cDiff < 0) {
                    newC = getCol() - step;
                }
            }
        }
    
    private:
        int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
    public:
		Bat(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return '~';}

        /*
            attemptMoveTo() for Bats:
                Bats always move horizontally to the Hero's column
                    however they cannot move vertically
        */
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            newR = getRow();
            newC = hCol;
        }

}; // Bat


class Abyss: public BoardCell {
	public:
    	Abyss(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '#';}
        virtual bool isHole( ) {return true;}
}; // Abyss


class Wall: public BoardCell {
	public:
    	Wall(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '+';}
    	virtual bool isBarrier( ) {return true;}
}; // Wall


class Nothing: public BoardCell {
	public:
    	Nothing(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return ' ';}
    	virtual bool isSpace( ) {return true;}
}; // Nothing


class EscapeLadder: public BoardCell {
	public:
    	EscapeLadder(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '*';}
    	virtual bool isExit( ) {return true;}
}; // EscapeLadder


#endif //_BOARDCELL_H
