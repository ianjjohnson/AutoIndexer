#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

//The number of indeces by which the array will expand on resize
#define RESIZE_FACTOR 10

/*
 * The ArrayList class stores a mutable array of cstrings of length 41 characters.
 */
class ArrayList{
	private:
        char** arrPointer; //The Array of cstring objects in the arraylist
        int resizeCounter; //Counts down from RESIZE_Factor to zero (resize required at zero). While not required, it makes code a bit more readable
        int numElements; //Total number of initialized non-empty elements in the array
        void resize(); //Resize method called when numElements > size of array

	public:
        void add(char*); //Add a new generic element to the array
		ArrayList(); //Constructor
		//ArrayList operator= (const ArrayList<T>&); //Assignment operator
        ArrayList(const ArrayList&); //Copy constructor
		~ArrayList(); //Destructor
        int indexOf(char*);//Returns the index of this char* in the arraylist
		void print(); //Prints all items in arrayList
        void set(int, char*); //Set the item at index
		int size() const; //Getter for the number of elements in the arrayList
        char* get(int) const; //Gets the item at the index passed as a parameter
        char** getPointer(); //Gets the pointer to the array of words being stored
};

#endif
