#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
  struct ROW {
    T*  Cols;     // array of column elements
    size_t NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // array of ROWs
  size_t  NumRows;  // total # of rows (0..NumRows-1)
    
public:
    /*
        Grid()
            default constructor:
                Initializes a 4x4 Grid
                    4 rows and 4 columns is the default number
                    of rows and columns
                Each element on the grid holds the default value of T
    */
    Grid() {
        Rows = new ROW[4];  // 4 rows
        NumRows = 4;

        // initialize each row to have 4 columns:
        for (size_t r = 0; r < NumRows; ++r) {
        Rows[r].Cols = new T[4];
        Rows[r].NumCols = 4;

        // initialize the elements to their default value:
        for (size_t c = 0; c < Rows[r].NumCols; ++c) {
            Rows[r].Cols[c] = T();  // default value for type T:
        }
        }
    }

    /*
        Grid(size_t R, size_t C)
            parameterized constructor:
                Initializes a Grid with R rows and C columns in each row
                    R and C are passed in as parameters
                Each element on the grid holds the default value of T
    */
    Grid(size_t R, size_t C) {
        Rows = new ROW[R]; // R rows
        NumRows = R;

        for(size_t r = 0; r < NumRows; r++) {
            Rows[r].Cols = new T[C]; // each row has C columns
            Rows[r].NumCols = C;

            for(size_t c = 0; c < Rows[r].NumCols; c++) {
                Rows[r].Cols[c] = T(); // default value of T
            }
        }
    }

    /*
        virtual ~Grid()
            destructor:
                Called automatically to free memory associated
                with a Grid object when it goes out of scope

                Iterates through each row of the Grid and deletes 
                each column
                Then it deletes the Row object associated with the Grid
    */
    virtual ~Grid() {
        for(size_t r = 0; r < NumRows; r++) {
            delete[] Rows[r].Cols;
        }
        delete[] Rows;
    }

    /*
        Grid(const Grid<T>& other)
            copy constructor:
                Called automatically to construct a Grid that contains
                a copy of an existing Grid
                    Occurs when passing Grid as a parameter by value
                
                copies the members of other 
                    the number of rows
                    the number of columns
                    each element of the grid
    */
    Grid(const Grid<T>& other) {
        this->Rows = new ROW[other.NumRows];
        this->NumRows = other.NumRows;

        for(size_t r = 0; r < other.NumRows; r++) {
            size_t other_numcols = other.Rows[r].NumCols; // holds the number of columns in a row in other

            this->Rows[r].Cols = new T[other_numcols]; // initialize a new column T array for every row
            this->Rows[r].NumCols = other_numcols;

            for(size_t c = 0; c < other_numcols; c++) {
                this->Rows[r].Cols[c] = other.Rows[r].Cols[c]; // sets each element to its matching pair on other
            }
        }
    }
        
    /*
        Grid& operator=(const Grid& other)
            copy operator:
                Called when one Grid is assigned into another

                this = other;
    */
    Grid& operator=(const Grid& other) {
        // if this is equal to other, return a pointer to this
        if(this == &other) {
            return *this;
        }
        
        // deleting all elements inside of this
        for(size_t r = 0; r < NumRows; r++) {
            delete[] Rows[r].Cols;
        }
        delete[] Rows;

        // coping all elements inside of other to this
        this->Rows = new ROW[other.NumRows];
        this->NumRows = other.NumRows;

        for(size_t r = 0; r < other.NumRows; r++) {
            size_t other_numcols = other.Rows[r].NumCols;

            this->Rows[r].Cols = new T[other_numcols];
            this->Rows[r].NumCols = other_numcols;

            for(size_t c = 0; c < other_numcols; c++) {
                this->Rows[r].Cols[c] = other.Rows[r].Cols[c];
            }
        }
        return *this;
    }

    /*
        size_t numrows() const
            Returns the number of rows in the
            Grid
    */
    size_t numrows() const {
        return NumRows;
    }

    /*
        size_t numcols(size_t r) const
            Returns the number of columns that Row r has
    */
    size_t numcols(size_t r) const {
        return Rows[r].NumCols; 
    }


    /*
        size_t size() const
            Returns the total number of elements in the Grid:
                Looks at each row and sums the number of columns
                in each row
    */
    size_t size() const {
        int total = 0;
        for(size_t r = 0; r < NumRows; r++) {
            total += Rows[r].NumCols;
        }
        
        return total;
    }

    /*
        T& operator()(size_t r, size_t c)
            Returns a reference to the element at location (r, c)
            Allows you to access or assign the element at (r, c)
    */
    T& operator()(size_t r, size_t c) {
        if(r >= NumRows || c >= Rows[r].NumCols) {
            throw out_of_range("Invalid row or column index");
        }
        return Rows[r].Cols[c];
    }

    /*
        void _output()
            Outouts the contents of the grid
    */
    void _output() {
        for(size_t r = 0; r < NumRows; r++) {
            cout << "| ";
            for(size_t c = 0; c < Rows[r].NumCols; c++) {
                cout << Rows[NumRows - r - 1].Cols[c] << " |";
            }
            cout << endl;
        }
    }
};