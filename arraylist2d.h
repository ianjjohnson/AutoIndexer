#ifndef ARRAYLIST2D_H
#define ARRAYLIST2D_H

#define RESIZE_FACTOR 10

/*
 * The ArrayList2D class contains a mutable 2d array of integers
 */
class ArrayList2D
{
private:
    int** arrPointer; //Main data array. 2D int array
    int* numElementsArr;//Array of the lengths of the sublists. numElementsArr[n] represents the number of used slots in arrPointer[n]
    int length; //Number of initialized, non empty sublists
    int true_length; //Number of pointers in arrPointer array
    void resize(); //Resize the main list, allowing for additional sublists to be added
    void resize(int); //Resize the sublist at the index int

public:
    ArrayList2D(); //Default constructor
    void addItemToSublist(int, int); //Adds a new item to a specified sublist
    void addSublistWithNewItem(int); //Creates a new sublist and adds a new item to that list
    bool sublistContainsElement(int, int); //Checks if a sublist contains an element
    ~ArrayList2D(); //Destructor
    int getNumberOfSublists(); //Getter for the width of the 2D array
    int getSizeOfSublist(int); //Getter for the height of one column of the array
    void print(); //Prints the contents of the array for debugging
    int get(int, int); //Gets one specific int from coordinates in the 2D array
};

#endif
