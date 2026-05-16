#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abstract.h"
#include "library.h"

#define MAX_FILES 10

// needed to load n files : we use it cause we dont have the number of files before



// Color Definitions--------------------------------------------------
#define SCREEN_WIDTH 50
#ifdef _WIN32
#define CLEAR_SCREEN "cls" // Windows clear command
#endif
#define DELAY 500000         // FOR THE CAR SPEED
#define DELAY2 50000         // THE CAR IS FASTER
#define COLOR_RED "\x1b[31m" // defining colours of HMI
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"
#define MAIN_TITLE "\x1b[38;5;51m"    // Bright Cyan
#define MENU_HEADER "\x1b[38;5;45m"   // Soft Blue
#define MENU_OPTIONS "\x1b[38;5;255m" // Bright White
#define MENU_NUM "\x1b[38;5;226m"     // Yellow for option numbers
#define INPUT_PROMPT "\x1b[38;5;228m" // Light Yellow
#define USER_INPUT "\x1b[38;5;214m"   // Orange for user input
#define SUCCESS "\x1b[38;5;46m"       // Bright Green
#define WARNING "\x1b[38;5;226m"     // Yellow
#define ERROR "\x1b[38;5;196m"        // Bright Red
#define DATA_DISPLAY "\x1b[38;5;231m" // Pure White
#define BORDER "\x1b[38;5;33m"        // Medium Blue
#define RESET "\x1b[0m"
#define SUBTITLE "\x1b[38;5;117m"     // Lighter blue
#define HIGHLIGHT "\x1b[38;5;226m"    // Bright yellow
#define DIM_TEXT "\x1b[38;5;245m"     // Gray for less important info
#define FAMILY_COLOR "\x1b[38;5;51m"  // Cyan for family names
#define PERSON_COLOR "\x1b[38;5;156m" // Light green for people
#define CAR_COLOR "\x1b[38;5;215m"    // Peach for cars
#define PLATE_COLOR "\x1b[38;5;228m"  // Light yellow for plates
#define SEX_COLOR "\x1b[38;5;207m"    // Pink for gender
//---------------------------------------------------------------------

typedef struct
{
    Paragraph *ptr; // pointer of first paragraph in the file
    char name[256]; // name of the file
} LoadedFile;

LoadedFile loadedFiles[MAX_FILES]; // where to load files

int currentFileCount = 0; // number of loaded files
// we opt to use this instead independently cause it is not a part of choices

void printHeader(void)
{
    printf(MAIN_TITLE "\n =================================================\n");
    printf("           SET THEORY 'LINGUISTIC DATA PROCESSONG '     \n");
    printf("   Nourelichrak MEHSAS  &  Rihab HAMMADOUCHE      \n");
    printf(" =================================================\n" RESET);
}

// listing loaded paragraphs
void listLoadedFiles(void)
{
    if (currentFileCount == 0)
    {
        printf(WARNING "(no files loaded)\n" RESET);
        return;
    }
    for (int i = 0; i < currentFileCount; i++)
    {
        // FIXED: Added %s and newline for clarity
        printf(MENU_NUM "  [%d] " DATA_DISPLAY "%s\n" RESET, i, loadedFiles[i].name);
    }
}
// lsiting paragraphs of a file
void listParagraphsofFile(int fileIdx)
{
    Paragraph *p = loadedFiles[fileIdx].ptr;
    int i = 0;
    while (p != NULL)
    {
        // FIXED: Matched format specifiers to arguments
        printf(MENU_NUM "  [%d] " DATA_DISPLAY "%s" DIM_TEXT " (Sentences: %d)\n" RESET, i, p->label, length(p->sentences));
        p = p->next;
        i++;
    }
}

// ask the user to pick a file ,then the paragraph to operate on it
// giving the number of the desired file , the pointer tto the pargaraph among the file

int selectSource(int *fileIdx, int *paraIdx)
{
    printf(MENU_HEADER "\n  Select file:\n" RESET);
    listLoadedFiles(); // it is module used to display the loaded "available files "
    printf(INPUT_PROMPT "  File index: " RESET);
    if (scanf("%d", fileIdx) != 1 // scanf returns 1 for correct input ( 0 if Ab for example )
        ||
        *fileIdx < 0 // if a negative number of file !!
        ||
        *fileIdx >= currentFileCount) // enters a number out of the range
    {
        printf(ERROR "  [!] Invalid file index.\n" RESET);
        return 0;
    }
    if (loadedFiles[*fileIdx].ptr == NULL)
    {
        printf(ERROR "  [!] Selected file is empty.\n" RESET);
        return 0;
    }

    printf(MENU_HEADER "  Paragraphs in '" DATA_DISPLAY "%s" MENU_HEADER "':\n" RESET, loadedFiles[*fileIdx].name); // prints the name of the file indexed by the entered indx
    // acceder au champ name du maillon pointé par fileIdx enterd


    listParagraphsofFile(*fileIdx); // we list the paragraph of the file

    int total = countParagraphs(loadedFiles[*fileIdx].ptr); // we count the paragraphs pf that file

    printf(INPUT_PROMPT "  Paragraph index: " RESET); // same logic as used with the file to choise paragraph
    if (scanf("%d", paraIdx) != 1 ||
        *paraIdx < 0 ||// if a negative number of paragraph !!
        *paraIdx >= total)// if enters a number out of the range
    {
        printf(ERROR "  [!] Invalid paragraph index.\n" RESET);
        return 0;
    }

    return 1;
}

/* ═══════════════════════════════════════════════
              OPERATION SUBMENUES
   ═══════════════════════════════════════════════ */

void handleSetOperations(void) // submenu for operations
{
    if (currentFileCount == 0) // there is no file !!!
    {
        printf(ERROR "  [!] No files loaded. Load files first.\n" RESET);
        return;
    }

    printf(MENU_HEADER "\n  *** SET OPERATIONS ***\n");
    printf(MENU_NUM "  1. " MENU_OPTIONS "Union        (A U B)\n" RESET);
    printf(MENU_NUM "  2. " MENU_OPTIONS "Intersection (A INTER  B)\n" RESET);
    printf(MENU_NUM "  3. " MENU_OPTIONS "Difference   (EL BELONGS TO A NOT TO B)\n" RESET);
    printf(MENU_NUM "  4. " MENU_OPTIONS "Subset check (Is A  INCLUDED IN B?)\n" RESET);
    printf(MENU_NUM "  0. " MENU_OPTIONS "Back\n" RESET);
    printf(INPUT_PROMPT "  Choice: " RESET);
    int op;
    if (scanf("%d", &op) != 1 || op == 0) // if invalid choice or quite choice , return
        return;

    int f1, p1, f2, p2;
    if (!selectSource(&f1, &p1))
        return;
    if (!selectSource(&f2, &p2))
        return;
    // if we can not select the paragraph from the adequate file we return

    Paragraph *paraA = getParagraphByIndex(loadedFiles[f1].ptr, p1); // to extract the adequate file
    Paragraph *paraB = getParagraphByIndex(loadedFiles[f2].ptr, p2); // to e
    sentence *resultsentence = NULL;
    Paragraph *result = NULL;

    switch (op)
    {
    case 1:
        
        resultsentence = unionSen(paraA->sentences, paraB->sentences); // do  the union,free!!!!
        printf(MENU_HEADER "\n  [Union of SENTENCES]: all sentences that belong to A or B\n" RESET);
        printf(DIM_TEXT "  Note: Union on paragraphs is not performed because\n"
                        "  the result would be identical to the sentence-level union\n" RESET);
        printf(MENU_HEADER "\n  --- Union (%s of '%s') UNION  (%s of '%s') ---\n" RESET,
               paraA->label, loadedFiles[f1].name,
               paraB->label, loadedFiles[f2].name);
        displaySentences(resultsentence);
        printf(INPUT_PROMPT "  Cardinality: %d\n" RESET, length(resultsentence));
        freeSentences(resultsentence);
        break;

    case 2:
     //=================== The intersection between the two paragraphs ============================
        result = intersectionPara(paraA, paraB); // do the intersection between the two paragraphs, free!!!!
        printf(MENU_HEADER "\n  --- Intersection on the paragraphs ---\n" RESET);
        if (result)
        {
            displayParagraphs(result);
            printf( DIM_TEXT "  Cardinality: %d\n" RESET, countParagraphs(result));
        }
        else
        {
            printf(WARNING "  (Empty set - the paragraphs are not equal)\n" RESET);
        }



       //===========The intersection between the sentences of the two paragraphs==========
       
       

        resultsentence = intersectionSen(paraA->sentences, paraB->sentences); // do the intersection
        printf( MENU_HEADER "\n  --- Intersection on the sentences ---\n" RESET);
        if (resultsentence)
        {
            displaySentences(resultsentence);
            printf( DIM_TEXT "  Cardinality: %d\n" RESET, length(resultsentence));
        }
        else
        {
            printf(WARNING "   (Empty set - no common sentences)\n" RESET);
        }
        freeSentences(resultsentence);
        break;

    case 3:
        printf(MENU_HEADER "\n  [Difference of SENTENCES]: sentences that belong to A but NOT to B\n" RESET);
        printf(DIM_TEXT "  Note: Difference on paragraphs is not performed because\n"
                        "  a paragraph is defined by its sentences, so the meaningful\n"
                        "  difference can only be computed at the sentence level\n" RESET);
        resultsentence = differenceSen(paraA->sentences, paraB->sentences);
        printf( MENU_HEADER "\n  --- Difference (A - B) on the sentences ---\n" RESET);
        if (resultsentence)
        {
            displaySentences(resultsentence);
            printf(DIM_TEXT "  Cardinality: %d\n" RESET, length(resultsentence) );
        }
        else
        {
            printf(WARNING "  (Empty - A is a subset of B)\n" RESET);
        }
        freeSentences(resultsentence);
        break;

    case 4:
        if (subSetsPara(paraA, paraB))
            printf( SUCCESS "\n  :TRUE :  %s SUBSET  %s\n" RESET , paraA->label, paraB->label); // we need abstraction for the field label
        else 
            printf( ERROR "\n  :FALSE: the paragraph %s is NOT a subset of the paragraph   %s\n" RESET, paraA->label, paraB->label); 
        if (subSetsSen(paraA->sentences, paraB->sentences))
            printf( SUCCESS "\n  :TRUE :  %s SUBSET  %s\n" RESET, paraA->label, paraB->label); // we need abstraction for the field label
        else
            printf( ERROR "\n  :FALSE: %s is NOT a subset of %s\n" RESET, paraA->label, paraB->label);
        break;
    default:
        printf(COLOR_RED "  Invalid choice.\n" RESET);
    }
}

void handleComplement(void)
{
    // Check if we actually have files to work with
    if (currentFileCount == 0)
    {
        printf(ERROR "  No files loaded.\n" RESET);
        return;
    }

    int f, p;
    //  Ask user which file/paragraph they want to pick
    if (!selectSource(&f, &p))
        return;

    // Get the memory address of the chosen paragraph
    Paragraph *target = getParagraphByIndex(loadedFiles[f].ptr, p);
    if (!target)
    {
        printf(ERROR "  [!] Invalid paragraph.\n" RESET);
        return;
    }

    //  Create a new list
    sentence *comp = complementInFile(loadedFiles[f].ptr, target);

    printf( MENU_HEADER "\n  --- Complement of %s in '%s' ---\n" RESET, target->label, loadedFiles[f].name);

    // 5. If the complement list isn't empty, show it!!!!!
    if (comp != NULL)
    {
        displaySentences(comp);
        printf(DIM_TEXT "  Cardinality: %d\n" RESET, length(comp));
    }
    else
    {
        printf( SUCCESS " (Paragraph contains every sentence in the file)\n" RESET);
    }

    // 6. Clean up the temporary list to save RAM
    freeSentences(comp);
}

// >>>function to search for an expression inside a selected file
void handleSearch(void)
{
    // check if there are no files loaded in memory
    if (currentFileCount == 0)
    {
        printf(ERROR "  [!] No files loaded.\n" RESET);
        return; // stop the function if no files exist
    }

    // show all loaded files
    listLoadedFiles();

    // ask the user to choose a file index
    printf( INPUT_PROMPT "  File index to search in: " RESET);

    int f;

    // read file index and validate it
    // scanf returns 1 if input is a valid integer
    if (scanf("%d", &f) != 1 || f < 0 || f >= currentFileCount)
    {
        printf(ERROR "  [!] Invalid index.\n" RESET);
        return;
    }
    if (loadedFiles[f].ptr == NULL)// if the file is empty
    {
        printf(ERROR "  [!] Selected file '%s' is empty.\n" RESET, loadedFiles[f].name);
        return;
    }

    // remove leftover newline from input buffer
    // this prevents fgets from reading an empty line
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // array to store the expression entered by the user
    char expr[500];

    // ask user to enter the expression to search
    printf(INPUT_PROMPT "  Expression to search: " RESET);

    // read full line including spaces
    fgets(expr, sizeof(expr), stdin);

    // remove extra spaces and newline from the input
    trimSpaces(expr);

    // search the expression inside the selected file
    if (searchExpressionInFile(loadedFiles[f].ptr, expr))
        printf( SUCCESS "  >> FOUND in '%s'\n" RESET, loadedFiles[f].name); // since we work with expression with space , we need fgets istead of simple scanf
    // because scanf can not read space
    else
        // if not found, print this message
        printf( ERROR "  >> NOT found in '%s'\n" RESET, loadedFiles[f].name);
}
/* ═════════════════════MAIN MENU══════════════════════════*/

int main(void)
{
    printHeader();
    int choice;

    while (1)
    {
        printf(MAIN_TITLE "\n============================================\n");
        printf("     ***    LINGUISTIC DATA PROCESSING  ***\n");
        printf("============================================\n" RESET);
        printf( DIM_TEXT "  Please load files first:\n" RESET);
        printf( MENU_NUM"  1. Load file(s)\n" RESET);
        printf(MENU_NUM "  2. Display loaded files\n" RESET);
        printf( MENU_NUM "  3. Set operations  (union / intersection / difference / subset)\n" RESET);
        printf( MENU_NUM "  4. Complement operation\n" RESET);
        printf( MENU_NUM "  5. Search expression in file\n" RESET);
        printf( MENU_NUM "  0. Exit\n" RESET);
        printf(INPUT_PROMPT "Choice: " RESET);

        if (scanf("%d", &choice) != 1)
        {
            /* clear bad input */
            while (getchar() != '\n') // etchar reads one character at a time , fgets to read a sentence
                ;                     // ensure correct reading
            continue;
        }

        switch (choice)
        {
        case 0:
            // free first if we had one file loaded at least
            for (int i = 0; i < currentFileCount; i++)
                freeParagraphs(loadedFiles[i].ptr);
            printf(SUCCESS "  Goodbye!\n" RESET);
            return 0;

        case 1:
        {
            int n;
            printf(INPUT_PROMPT "  How many files you like to chose ? " RESET);
            if (scanf("%d", &n) != 1 || n <= 0)// if the user enters a non-integer or a non-positive number
            {
                printf(ERROR"  [!] Invalid number of files.\n" RESET);
                return 0;
            }
             if (n > MAX_FILES - currentFileCount) // if the user tries to load more files than the remaining capacity
            {
                printf(WARNING "  [!] Can only load %d more file(s).\n" RESET, MAX_FILES - currentFileCount);
                return 0;
            }
            for (int i = 0; i < n && currentFileCount < MAX_FILES; i++)// loop to load n files
            {
                printf(INPUT_PROMPT "  Filename %d: " RESET, i + 1);
                scanf("%255s", loadedFiles[currentFileCount].name);// read filename with spaces, limit to 255 chars
                loadedFiles[currentFileCount].ptr = loadFile(loadedFiles[currentFileCount].name);

                if (loadedFiles[currentFileCount].ptr != NULL) // check if we loaded
                {
                    printf( SUCCESS "  yesssssss!!  Loaded '%s'  : %d paragraph(s)\n" RESET, loadedFiles[currentFileCount].name, countParagraphs(loadedFiles[currentFileCount].ptr));
                    currentFileCount++;
                }
            }
            break;
        }

        case 2:
            for (int i = 0; i < currentFileCount; i++) // traverse
            {
                printf( MENU_HEADER "\n ::: File [%d]: %s :::\n" RESET, i, loadedFiles[i].name);
                displayParagraphs(loadedFiles[i].ptr);
            }
            break;

        case 3:
            handleSetOperations(); // call  submenu of operations
            break;

        case 4:
            handleComplement(); // call procedure of complement
            break;

        case 5:
            handleSearch(); // call the function of search done above
            break;

        default: // else
            printf(ERROR "  [!] Invalid choice.\n" RESET);
        }
    }
}