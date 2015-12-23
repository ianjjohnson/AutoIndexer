#include "arraylist2d.h"
#include <iostream>

using namespace std;

/*
Default constructor for an ArrayList2D
*/
ArrayList2D::ArrayList2D()
{

    //Allocate memory for pointers
    arrPointer = new int*[RESIZE_FACTOR];
    numElementsArr = new int[RESIZE_FACTOR];

    //Allocate the main sublists and empty the numElementsArr to overwrite existing garbage
    for(int i =0; i < RESIZE_FACTOR; i++){
        arrPointer[i] = new int[RESIZE_FACTOR];
        numElementsArr[i] = 0;
    }

    //Initalize to default values
    length = 0;
    true_length = RESIZE_FACTOR;
}

/*
Resizes the main array to allow for more sublists.
Turns an array of int[N][M] -> int[N+RESIZE_FACTOR][M].
Keep in mind that this is not a rectangular 2D array so M != constant
*/
void ArrayList2D::resize(){

    //Update true length to account for increased size and allocate memory for new array
    true_length += RESIZE_FACTOR;
    int** temp = new int*[true_length];
    int* numElementsArrTemp = new int[true_length];

    //Allocate space for each subarray and copy existing elements into new arrays
    for(int i = 0; i < length; i++){
        //Alloc sublist for index i of new array
        temp[i] = new int[(numElementsArr[i]%RESIZE_FACTOR) + RESIZE_FACTOR];

        //Copy over existing elements to new subarrays
        for(int j = 0; j < numElementsArr[i]; j++)
            temp[i][j] = arrPointer[i][j];

        //Save number of elements in each old subarray
        numElementsArrTemp[i] = numElementsArr[i];
    }

    //Allocate the new subarrays, and set their size to zero.
    //WARNING: These new subarrays contain garbage
    for(int i = length; i < true_length; i++){
        temp[i] = new int[RESIZE_FACTOR];
        numElementsArrTemp[i] = 0;
    }

    //Delete all existing subarrays
    for(int i = 0; i < length; i++){
        delete[] arrPointer[i];
    }
    //Delete main arrays
    delete[] arrPointer;
    delete[] numElementsArr;

    //Assign pointers to newly allocated and copied arrays
    arrPointer = temp;
    numElementsArr = numElementsArrTemp;
}

/*
Resizes the subarray at the specified index such that, in the main array,
int[column] goes from length N to N + RESIZE_FACTOR
@param column - the index of the subarray to resize
*/
void ArrayList2D::resize(int column){

    //Allocate space for the new array and copy over existing elements
    int* temp = new int[numElementsArr[column]+RESIZE_FACTOR];
    for(int i =0; i < numElementsArr[column]; i++)
        temp[i] = arrPointer[column][i];

    //Delete the existing array and adjust the pointer to the newly allocated array
    delete[] arrPointer[column];
    arrPointer[column] = temp;
}


/*
Checks if a sublist already contains an element.
@param sublistIndex - the index of the sublist (column #) to be searched
@param element - the element to search for in the sublist
@return - true if 'element' appears in the sublist at arrPointer[sublistIndex]
*/
bool ArrayList2D::sublistContainsElement(int sublistIndex, int element){

    //Parse through the sublist, checking for equality with each element
    for(int i = 0; i < numElementsArr[sublistIndex]; i++)
        if(arrPointer[sublistIndex][i] == element)
            return true;

    //Return false if no match is found
    return false;
}

/*
Adds a new item to the specified sublist, resizing if necessary
@param newItem - the item to add to the sublist
@param sublistIndex - the index in the main array of the sublist to which the new element should be added
*/
void ArrayList2D::addItemToSublist(int newItem, int sublistIndex){

    //If the sublist already contains this element, return. No action necessary
    if(sublistContainsElement(sublistIndex, newItem)) return;

    //Resize if needed
    if(numElementsArr[sublistIndex]%RESIZE_FACTOR == 0) resize(sublistIndex);

    /*
    The following code is used for --SORTING ON INSERTION--
    Move the tracking variable 'index' forward until an element
    in the array is found whose value is greater than newItem
    */
    int index = 0;
    while(newItem > arrPointer[sublistIndex][index] && index < numElementsArr[sublistIndex]) index++;

    //Push forward every item infront of the index where the new item will reside
    for(int i = numElementsArr[sublistIndex]; i > index; i--)
        arrPointer[sublistIndex][i] = arrPointer[sublistIndex][i-1];

    //Place the new item in it's found position
    arrPointer[sublistIndex][index] = newItem;

    //Register the increase in number of items in the sublist
    numElementsArr[sublistIndex]++;
}

/*
Creates a new sublist with one item -- newItem -- resizing if needed
@param newItem - the item to be placed at index 0 of the new sublist
*/
void ArrayList2D::addSublistWithNewItem(int newItem){

    //Resize if needed
    if(length%RESIZE_FACTOR == 0 && length!=0) resize();

    //Place the new item in the array, register it's size, and register the addition of the new sublist
    arrPointer[length][0] = newItem;
    numElementsArr[length] = 1;
    length++;
}

/*
Getter method for the number of initialized sublists in the ArrayList2D
@return - the number of non-empty, initialized sublists in the ArrayList2D
*/
int ArrayList2D::getNumberOfSublists(){
    return length;
}

/*
Getter method for the number of initialized entries in a specified sublist
@param index - the index of the sublist whose length is requested
@return - the number of initialized elements in the sublist arrPointer[index]
*/
int ArrayList2D::getSizeOfSublist(int index){
    return numElementsArr[index];
}

/*
Prints all of the numbers contained in the list, used for debugging
*/
void ArrayList2D::print(){

    //Parse through every initialized sublist
    for(int i = 0; i < length; i++){
        //Parse through every initalized element in the sublist and print it
        for(int j = 0; j < numElementsArr[i]; j++)
            cout << arrPointer[i][j] << " ";
        cout << endl;
    }
}

/*
Getter method for the value of the item at index x, y in the array
@param x - the sublist to be searched
@param y - the index in the sublist to be grabbed
@return - the item stored in arrPointer at x,y
*/
int ArrayList2D::get(int x, int y){
    return arrPointer[x][y];
}

/*
Destructor for ArrayList2D, deletes all sublists and all their elements
*/
ArrayList2D::~ArrayList2D(){

    //Delete the array of sublist sizes
    delete[] numElementsArr;

    //Delete every subarray
    for(int i = 0; i < true_length; i++)
        delete[] arrPointer[i];

    //Delete the main array
    delete[] arrPointer;
}

