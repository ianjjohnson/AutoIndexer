#include "ArrayList.h"
#include <iostream>

using namespace std;

/*
Default constructor for the arrayList
*/
ArrayList::ArrayList(){

    //Default value assignment
    resizeCounter = RESIZE_FACTOR;
    numElements = 0;

    //constructing empty array of 10 cstrings, each of which can hold a 40 character phrase
    arrPointer = new char*[RESIZE_FACTOR];
    for(int i = 0; i < RESIZE_FACTOR; i++)
        arrPointer[i] = new char[41];
}

/*
Copy constructor (unused)
@param other - the ArrayList to copy from
*/
ArrayList::ArrayList(const ArrayList& other){

    //construct cstring array and copy over elements from other ArrayList passed as a parameter
    arrPointer = new char*[other.size() + RESIZE_FACTOR - other.size()%RESIZE_FACTOR];
	for(int i = 0; i < other.size(); i++){
		arrPointer[i] = other.get(i);	
	}

    //lookup and save values for resizeCounter and numElements
    resizeCounter = other.size() + RESIZE_FACTOR - other.size()%RESIZE_FACTOR;
	numElements = other.size();
}


/*
Resizes the underlying cstring array in the list to allow for more elements.
Increases the size of the ArrayList by RESIZE_FACTOR
*/
void ArrayList::resize(){

    //Make new, larger array in the heap and initialize the cstrings
    char** temp = new char*[numElements+RESIZE_FACTOR];
    for(int i = 0; i < numElements+RESIZE_FACTOR; i++)
        temp[i] = new char[41];

	//Copy the elements in the former array to the new one
	for(int i = 0; i < numElements; i++){
        for(int j = 0; j < 41; j++)
            temp[i][j] = arrPointer[i][j];
	}

	//Delete the original array from the heap
    for(int i = 0; i < numElements; i++)
        delete[] arrPointer[i];
	delete [] arrPointer;

	//Set arrPointer to point to new array in heap
	arrPointer = temp;

	//Reset the resize counter
	resizeCounter = RESIZE_FACTOR;
}

/*
Adds a new cstring to an ArrayList
@param newElement - the new cstring to put in the arrayList
*/
void ArrayList::add(char* newElement){

    //Resize the arraylist if necessary
	if(resizeCounter == 0) resize();

    //copy the new object into the next open space in the array
    for(int i = 0; i < 41; i++)
        arrPointer[numElements][i] = newElement[i];

    //modify tracking variables
	resizeCounter--;
	numElements++;
}
/*
Destructor for the class, deletes the array from the heap
*/
ArrayList::~ArrayList(){

    //Delete each cstring, and then the array of pointers to the strings
    for(int i = 0; i < numElements + resizeCounter; i++){
        delete[] arrPointer[i];
    }
    delete [] arrPointer;
}

/*
Calculates and returns the index of 'item' in the arraylist, or -1 if 'item' doesn't appear in the list
@param item - the cstring to lookup in the arraylist
@return - the index of item in the arraylist, or -1 if the item doesn't appear in the list
*/
int ArrayList::indexOf(char* item){

    //Move through the entire list of elements one by one
    for(int i = 0; i < numElements; i++){

        /*
         * These booleans and their logic are used to avoid an issue with the ifstreaming library where it
         * leaves stray characters in a string after a null character or an escape character / null terminator
        */
        bool reachedEndOfNewItem = false;
        bool reachedEndOfExistingItem = false;

        //for every char in the cstrings
        for(int j = 0; j < 41; j++){

            /*
             * Check if a null or null terminator character has been hit in the input string or the saved string.
             * If one has, it can be assumed that the string is over so we ignore any junk left behind it in the array
             */
            if(item[j] == 0 || item[j] == '\0') reachedEndOfNewItem = true;
            if(arrPointer[i][j] == 0 || arrPointer[i][j] == '\0') reachedEndOfExistingItem = true;

            //If the end of each string has been reached, equality has been found. Return the index of the found match
            if(reachedEndOfNewItem && reachedEndOfExistingItem) return i;

            //If two characters don't agree, these cstrings (numElements[i] and item) don't match. Skip to the next one.
            if(item[j] != arrPointer[i][j]){
                break;
            }
        }
    }

    //Return -1 if no match is found
    return -1;
}

/*
Sets the value of the item at index 'index' to 'value'. Unused
@param index - the index in the array to change
@param value - the new value to place at index
 */
void ArrayList::set(int index, char* value){

    //Copy value into the cstring at arrPointer[index]
    for(int i = 0; i < 41; i++)
        arrPointer[index][i] = value[i];
}

/*
Prints every element in the array, separated by commas. Used for debugging
*/
void ArrayList::print(){

    //Cycle through the list, printing each cstring
	for(int i = 0; i < numElements; i++){
        cout << arrPointer[i] << ", ";
	}
}


/*
Getter which returns the size of the arrayList.
@return - the number of initialized elements in the array
*/
int ArrayList::size() const{
	return numElements;
}

/*
Gets the element at index i in the array
@param i - the index in the array from which to return the element
@return - the object at index i in the array
*/
char* ArrayList::get(int index) const{
	return arrPointer[index];
}

/*
Gets the 2D char array pointer used as the under-the-hood data structure
@return - the list of cstrings held in this arraylist as a char**
 */
char** ArrayList::getPointer(){
    return arrPointer;
}
