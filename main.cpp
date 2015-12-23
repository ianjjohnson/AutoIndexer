/*
 * NOTES ABOUT DESIGN CHOICE:
 * - The ArrayList class was originally templated to hold either a char** or an int**.
 *      This was, in theory, a more succinct solution. However, because the cstring array
 *      had to be mutable in one dimension while the 2D int array has to be mutable in both dimensions, the
 *		original template class was quite messy and not very elegant.
 *      For this reason, two distinct classes were created which, although quite similar, are different
 *      enough to warrant making them isolate from one another.
 * - A more elegant solution (and my original solution) was to have only one arraylist of IndexObjects,
 *      where an IndexObject held both a cstring and a list of ints representing page numbers where the
 *      word was found. I moved away from this solution because, although functional and perhaps a bit
 *      easier on the eyes, it required that every time a tenth word was added, all of the saved page numbers,
 *      along with the cstrings themselves, had to be copied. My current solution is markedly more efficient
 *      in that respect.
 * - In addition to the efficiency boost of using the parallel arraylist approach instead of the nested arraylist
 *      approach, the parallel arraylists dramatically simplify the memory management of the program, which helped
 *      avoid memory leaks
 * - The 'indeces' array is used as an transformation of the enumeration of the numbers ArrayList in the doOutput
 *      method, despite looking a little hacky, is wildly more memory efficient than the obvious solution, which
 *      would be to sort the actual array of integers. I chose to use the 'indeces' strategy because even though
 *      it's a bit more complicated, it's much faster, especially for large data sets
*/

#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>

#include "ArrayList.h"
#include "arraylist2d.h"

using namespace std;

int strCompare(char*, char*); //Compare the alphabetical order of two cstrings
void sort(char**, int, int*); //Sort an array of cstrings, generating an indeces matrix used for enumeration transformation
int numDigits(int); //Calculates the number of digits in an integer
void doInput(char*); //Performs the input from the file
void doOutput(char*); //Writes the output to a file

ArrayList words; //The list of words being stored
ArrayList2D numbers; //The list of lists of page numbers for each word being stored

int main(int argc, char* argv[]){

    //I don't enjoy the compiler warnings that I'm getting so I put this here.
    argc++;

    //Does the input... as one might expect
    doInput(argv[1]);

    //Does the output... as one might expect
    doOutput(argv[2]);

    return 0;
}

/*
Helper method -- calculates the number of digits in a given integer
Used to limit file output lines to 50 characters each
@param number - the number whose digits are to be counter
@return - the number of digits in 'number'
*/
int numDigits(int number){

    //Divide number by 10 using integer division until 0 is reached, saving the number of iterations
    int numDigits = 0;
    do {
        numDigits++;
        number /= 10;
    } while(number != 0);

    //Return the number of iterations over which the number was divided by 10, and therefore the number of digits in 'number'
    return numDigits;
}

/*
Helper method - compares two cstrings to one another based on alphabetical order
@param str1, str2 - the cstrings to compare
@return - negative if str1 appears first alphabetically, 0 if equal, positive if str1 appears second alphabetically
*/
int strCompare(char* str1, char* str2){

    //Move through the cstrings until non matching characters are found, or the end of the strings is found
    int index = 0;
    while(str1[index] != '\0' && str2[index] != '\0' &&str1[index] == str2[index]) index++;

    //Return the difference in the first non-matching chars, or zero if they're both null terminators
    return str1[index] - str2[index];
}

/*
Helper method - performs selection sort on the cstring array, saving an indexMap of the final positions of each element for later lookupt
@param list - the list of cstrings to be sorted
@param sizeOfList - the number of cstrings in list
@param indexMap - an int array used to track the final positions of the cstrings in list
PRECONDITION: indexMap is an int* of size [sizeOfList] (equal to size of cstring list) whose elements are indexMap[i] = i for i in 0..sizeOfList
POSTCONDITION: for every index i in indexMap, indexMap[i] = j, where j is the final index of the string originally saved at index i.
THE INDEXMAP IS USED TO AVOID INEFFICIENT COPYING OF ALL OF THE INT*s ASSOCIATED WITH EACH CSTRING
*/
void sort(char** list, int sizeOfList, int* indexMap){

    //Declaring helper variables
    int indexOfMin;
    char* tempStr;
    int indexMapTemp;

    //Parse through every index i of the list, and find the mininum value in the sublist i..endOfList
    for(int i = 0; i < sizeOfList-1; i++){

        //Set default index of min value to i so that if no lesser value is found, no swap occurs
        indexOfMin = i;

        //Find the mininum value in the sublist i..endOfList
        for(int j = i+1; j < sizeOfList; j++)
            if(strCompare(list[j], list[indexOfMin]) < 0)
                indexOfMin = j;

        //If some minimum was found after the current index i, swap elements at i and indexOfMin
        if(indexOfMin != i) {

            //Save items at i to temp variables
            tempStr = list[i];
            indexMapTemp = indexMap[i];

            //Overwrite values at i with values at indexOfMin
            list[i] = list[indexOfMin];
            indexMap[i]  = indexMap[indexOfMin];

            //Put temp values from i into indexOfMin
            list[indexOfMin] = tempStr;
            indexMap[indexOfMin] = indexMapTemp;
        }
    }
}

/*
Outputs the information to a file whose name is specified outputFileName
@param outputFileName - the name of the file to which the output will be written
*/
void doOutput(char* outputFileName){

    //Allocate and initalize indeces matrix such that indeces[i] = i for every in in 0...words.size()
    int* indeces = new int[words.size()];
    for(int i = 0; i < words.size(); i++)
        indeces[i] = i;

    //Sort the words, modifying the indeces array to save the final locations of every cstring in words
    sort(words.getPointer(), words.size(), indeces);

    //Declare and open the ofstream
    ofstream outputFileStream;
    outputFileStream.open(outputFileName);

    //For every word in the words list
    for(int i = 0; i < words.size(); i++){

       //Save the char currently being output (for the [N] line in output file) and the length of the current output line
       char currentFirstChar = words.get(i)[0];
       int lineLength = 0;

       //Print the current char being output
       outputFileStream << '[' << (char)(toupper(currentFirstChar)) << ']' << endl;

       //Parse through the sublist i..words.size() and print every other cstring for which stringName[0] == currentFirstChar
       for(int j = i; j < words.size(); j++){

           //If this word starts with currentFirstChar
           if(words.get(j)[0] == currentFirstChar){

               //Print the word followed by a colon and save the length of the line being written
               outputFileStream << words.get(j) << ": ";
               lineLength = 2 + strlen(words.get(j));

               //For every page on which this word appeared, print the page number followed by a comma (excluding comma for last word)
               for(int k = 0; k < numbers.getSizeOfSublist(indeces[j]); k++){

                   //If writing this number would exceed the 50 character limit for the line
                   if(numDigits(numbers.get(indeces[j], k)) >= 50 - lineLength - 1){

                       //Move onto a new line
                       outputFileStream << "\n    ";
                       lineLength = 4;
                   }

                   //Output the number to the file, ternary operator used to add a comma only if the number is not the last, and save the increase in line size
                   outputFileStream << numbers.get(indeces[j], k) << ((k == numbers.getSizeOfSublist(indeces[j])-1)? " ": ", ");
                   lineLength += numDigits(numbers.get(indeces[j], k)) + 2;
               }

               //Skip to a new line
               outputFileStream << endl;

               //If a word has been written to the file other than the first word which started with currentFirstChar, increment i to avoid duplicate word outputs
               if(j != i) i++;
           }
       }
    }

    //Close the ofstream and delete the indeces array
    outputFileStream.close();
    delete[] indeces;

}

/*
Inputs data from a file named inputFileName, inputting data into the words and numbers data structures
@param inputFileName - the name of the file from which input will be read
*/
void doInput(char* inputFileName){

    //Open the file stream and allocate space for the char* used to tokenize input
    ifstream file(inputFileName);
    char* inputToken = new char[43];
    int currentPageNumber = 0;

    //Make sure the file is open
    while(file.is_open()){

        //Input the next 'token' in the file (delimiter is a space) into the inputToken char*
        file >> inputToken;

        //If the input token starts with a bracket
        if(inputToken[0] == '['){

            //Copy all of the items in the token back by one to delete the opening bracket
            for(int i = 0; i < 42; i++){
                inputToken[i] = inputToken[i+1];
            }

            //Loop forward until manual break when end bracket is found
            while(true){

                //'found' is used to recognize when the end bracket has been found in the multi-word phrase
                bool found = false;

                //Saves the index of the end of the inputToken
                int endIndex = 0;

                //Loop through the input token looking for the null terminator
                for(int i = 0; i <43; i++){

                    //When the null terminator is found, save the index where it was found.
                    //This index will be where writing to the char* continues later
                    if(inputToken[i] == '\0'){
                        endIndex = i;
                        break;
                    }
                }

                //addendum is the next token in the file, which will be part of the multi-word phrase
                char* addendum = new char[43];
                file >> addendum;

                //replace the null terminator at the end of the input token with a space
                inputToken[endIndex] = ' ';

                //Copy the addendum token onto the end of the input token
                for(int i = 0; i < 43-endIndex; i++){
                    inputToken[i+endIndex + 1] = addendum[i];
                }

                //Delete the addendum token from the heap
                delete [] addendum;

                //Look through the input token for an end bracket
                for(int i = 0; i < 43; i++){

                    //If we found a null character, there will be no forthcoming end bracket, so break
                    if(inputToken[i] == '\0') break;
                    //If the end bracket is found, replace it with a null terminator and break from the for loop
                    if(inputToken[i] == ']'){
                        inputToken[i] = '\0';
                        found = true;
                        break;
                    }
                }

                //If an end bracket was found, break from the multi-word phrase onto the next token
                if(found) break;

                //If no end bracket was found, the loop will repeat until one is found and the multi-word phrase is complete
            }
        }

        //If the next token is a page number identified by following the form <n>
        if(inputToken[0] == '<'){

            //If the number specified starts with '-' (is a negative number, indicating end of file)
            if(inputToken[1] == '-'){
                //Breaks from the while file.is_open() loop, ending the file input process
                break;
            } else {

                //Find the index of the token where the end of the number is
                int endOfNumberIndex = 2;
                for(int i = 2; i < 41; i++){

                    //If the end '>' is found, save where it was found
                    if(inputToken[i] == '>'){
                        endOfNumberIndex = i;
                        break;
                    }
                }

                //Char array used to save the number found wrapped in the form <number>
                char* pageNum = new char[endOfNumberIndex];

                //Copy the number into the pageNum char* and end pageNum with a null terminator
                for(int i = 1; i < endOfNumberIndex; i++){
                    pageNum[i-1] = inputToken[i];
                }
                pageNum[endOfNumberIndex-1] = '\0';

                //Save the page number to an integer
                currentPageNumber = atoi(pageNum);

                //Delete the pageNum char* and move on to the next token
                delete [] pageNum;
                continue;
            }
        }

        //Move the input token cstring to lower case
        for(int i = 0; i < 43; i++){
            inputToken[i] = tolower(inputToken[i]);
        }

        //Save the index of the input token in the words arrayList (will be -1 if not found in list)
        int indexOfInputToken = words.indexOf(inputToken);

        //If the inputToken is not in the words ArrayList
        if(indexOfInputToken == -1){

            //Add the inputToken to the arraylist, and make a new int array to track the pages on which the token was found
            words.add(inputToken);
            numbers.addSublistWithNewItem(currentPageNumber);

        //If the inputToken is already in the list
        } else {
            //Register the word as having been found on the current page
            numbers.addItemToSublist(currentPageNumber, indexOfInputToken);
        }
    }

    //Close the file and delete the inputToken from the heap
    file.close();
    delete [] inputToken;
}



