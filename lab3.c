/**
 * This program reads a text file and write into a new and existing file about all the unique words and their frequencies.
 * This program also prints the number of unique words in the command line. The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Runhao Huang
 * @date Sept. 24, 2022
 * @assignment Lab 3
 * @course CSC 250
 **/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Maximum size of the static character array of the buffer. */  
#define MAX_STRING_SIZE 20

/* a useful struct, if you choose to use it */  
struct WordFreq {
   char* word;  /* the word */
   int count;   /* the word frequency */
};

/* function declarations go here */
int processCharacters(char filename[], struct WordFreq** wordListPtr, int *numWordsPtr);
int search(char* buffer, struct WordFreq* list, int num);
int printList(struct WordFreq** list, int num, char fileName[], char fileName2[]);

/* This is the main function of the program that includes initialization, validation, process, print result, and free dynamic memory.*/
int main(int argc, char* argv[]) {
    /* Initialization */
    struct WordFreq* wordList = NULL;
    int numWords = 0;
    int fileOK = 1;
    int i;

    /* Check whether there are three argument provided in the command line.*/
    if (argc < 3) {
        printf("Usage: %s filename \n", argv[0]);
        return 1;
    }

    /* Call the function to read and process file.*/
    fileOK = processCharacters(argv[1], &wordList, &numWords);
    /* If the input file doesn't exist, it will immediately stop and return. */  
    if (!fileOK) {
        printf("%s could not be opened, and AJ is nice \n", argv[1]);
        return 1;
    }
    
    /* Call function to write the output into the new or existing file. */
    printList(&wordList, numWords, argv[2], argv[1]);

    /* Free the dynamic memory of character arrays representing the words.*/  
    for (i = 0; i < numWords; i++) {
        if ((wordList)[i].word) {
            free(wordList[i].word);
        }
    }
    /* Free the dynamic memory of the wordList.*/  
    if (wordList) {
        free(wordList);
    }
    return 0;
}


/* A funtion that writes the unique words and their frequencies into the new or existing file.*/ 
int printList(struct WordFreq** list, int num, char fileName[], char fileName2[]) {
    int i = 0;
    int j = 0;
    struct WordFreq currWordFreq;
    FILE *fp = fopen(fileName, "we");

    /* Insertion sort that sort the words in the wordList with frequencies from large to small. */
    for (i = 1; i < num; i++) { 
        currWordFreq = (*list)[i];
        j = i - 1; 
        while (j >= 0 && (*list)[j].count < currWordFreq.count) { 
            (*list)[j + 1] = (*list)[j]; 
            j = j - 1; 
        } 
        (*list)[j + 1] = currWordFreq; 
    }

    /* Write the sorted word list into the new or existing file.*/  
    for (i = 0; i < num; i++) {
        fprintf(fp, "%s %d\n", (*list)[i].word, (*list)[i].count);
    }
    printf("%s has %d unique words\n", fileName2, num);
    fclose(fp);
    return 1;
}


/* A funtion that checks the validity of the filename and stores all the unique words, and their frequencies into the dynamic array of struct WordFreq.*/
int processCharacters(char filename[], struct WordFreq** wordListPtr, int *numWordsPtr) {
    char ch;
    FILE *filePtr = fopen(filename, "rie");
    struct WordFreq* tempList = NULL;
    char buffer[MAX_STRING_SIZE];
    int bufferLength = 0;
    int loc = 0;
    int i;
    int numWord = *numWordsPtr;  /* local copy of the number of words */
    buffer[0] = '\0';

    /* If the file doesn't exist*/
    if (filePtr == 0) {
        return 0;
    }

    ch = fgetc(filePtr);
    setbuf(stdout, NULL);
    
    /* If don't reach an end of the file.*/
    while (ch != EOF) {
        /* If current character is a letter, we add the letter into the buffer.*/
        if (isalpha(ch)) {
            ch = tolower(ch);
            buffer[bufferLength] = ch;
            bufferLength++;
            buffer[bufferLength] = '\0';
        }
        /* If current character is not a letter and there are already characters in the buffer, we make the buffer a word and search in the wordlist.*/
        else if (bufferLength > 0) {
            loc = search(buffer, *wordListPtr, numWord);
            /* If the word has been in the wordlist, we just update its count.*/
            if (loc != -1) {
                (*wordListPtr)[loc].count++;
            }
            /* If the word is new to the wordlist, we need to create a new list and copy the old one to the new list. Then add the new word into the new list.*/
            else {
                tempList = (struct WordFreq *) malloc((numWord + 1) * sizeof(struct WordFreq));
                /* Copy the old list into new list.*/
                for (i = 0; i < numWord; i++) {
                    tempList[i] = (*wordListPtr)[i];
                    tempList[i].word = (*wordListPtr)[i].word;
                }
                tempList[numWord].word = (char *) malloc(strlen(buffer) + 1);
                strncpy(tempList[numWord].word, buffer, (strlen(buffer) + 1));
                tempList[numWord].count = 1;

                /* Free dynamic memory of the old list.*/
                if (*wordListPtr) {
                    free(*wordListPtr);
                }
                *wordListPtr = tempList;
                numWord++;

            }
            bufferLength = 0;
            buffer[0] = '\0';
        }
        ch = fgetc(filePtr);
    }

    /* If the file pointer reaches the end and there is letter in the buffer, we need to add the buffer word into the wordlist.*/
    if (strlen(buffer) > 0) {
        loc = search(buffer, *wordListPtr, numWord);
        /* If the word has been in the wordlist, we just update its count.*/
        if (loc != -1) {
            (*wordListPtr)[loc].count++;
        }
        /* If the word is new to the wordlist, we need to create a new list and copy the old one to the new list. Then add the new word into the new list.*/
        else {
            tempList = (struct WordFreq *) malloc((numWord + 1) * sizeof(struct WordFreq));
            /* Copy the old list into new list.*/
            for (i = 0; i < numWord; i++) {
                tempList[i] = (*wordListPtr)[i];
                tempList[i].word = (*wordListPtr)[i].word;
            }
            tempList[numWord].word = (char *) malloc(strlen(buffer) + 1);
            strncpy(tempList[numWord].word, buffer, (strlen(buffer) + 1));
            tempList[numWord].count = 1;

            /* Free dynamic memory of the old list.*/
            if (*wordListPtr) {
                free(*wordListPtr);
            }
            *wordListPtr = tempList;
            numWord++;
        }
    }
    buffer[0] = '\0';

    fclose(filePtr);
    *numWordsPtr = numWord;

    return 1;
}


/* A funtion that loop through the array to find the location of the word. It will return -1 if not found.*/
int search(char* buffer, struct WordFreq* list, int num){
    int loc = -1;
    int i;
    for (i = 0; i < num && loc == -1; i++) {
        /* Check if the word is the same as the buffer word.*/
        if (strcmp((list)[i].word, buffer)==0) {
            loc = i;
        }
    }
    return loc;
}
