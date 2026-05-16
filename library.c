#include "library.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include "abstract.h"
/********** Basic modules for list manipulation **********/

/* --- Uses a counter and pointer traversal to calculate node count --- */
int length(sentence *head)
{
    int c = 0;
    while (head)
    {
        c++;               // increment counter for each node
        head = head->next; // move to next memory address
    }
    return c;
}

/* --- L linear search with string comparison to count specific matches --- */
int frequency(sentence *head, const char *w)
{
    int c = 0;
    while (head)
    {
        if (strcmp(head->exp, w) == 0)
            c++; // check equality of strings
        head = head->next;
    }
    return c;
}

/* --- builds a new list by copying A then B --- */
sentence *concatenation(sentence *a, sentence *b)
{
    sentence *result = NULL;
    while (a != NULL)
    {
        insertSentence(&result, a->exp); // copy element from A
        a = a->next;
    }

    while (b != NULL)
    {
        insertSentence(&result, b->exp); // append element from B
        b = b->next;
    }
    return result;
}

/* ---  compare current node with all subsequent nodes --- */
void deleteDuplicates(sentence **head)
{
    sentence *cur = *head;
    while (cur)
    {
        sentence *runner = cur;
        while (runner->next)
        {
            if (strcmp(runner->next->exp, cur->exp) == 0)
            {
                sentence *dup = runner->next;
                runner->next = dup->next; // bypass the duplicate node
                free(dup);                // release memory
            }
            else
            {
                runner = runner->next;
            }
        }
        cur = cur->next;
    }
}

/* ═══════════════════════════════════════════════
   SET OPERATIONS
   ═══════════════════════════════════════════════ */
// set operations on paragraph lists (files are sets of paragraphs)
Paragraph *intersectionPara(Paragraph *A, Paragraph *B)
{
    Paragraph *result = NULL;
    while (A)
    {
        if (compareParagraphs(A, B)) // logical AND (exists in both)
            insertParagraph(&result, createParagraph(A->label));
        A = A->next;
    }
    return result;
}
int subSetsPara(Paragraph *A, Paragraph *B)
{
    while (A)
    {
        if (!compareParagraphs(A, B))
            return 0; // violation of subset rule
        A = A->next;
    }
    return 1; // all elements passed the comparison check
}
// set operations on sentence lists (paragraphs are sets of sentences)
sentence *unionSen(sentence *A, sentence *B)
{
    sentence *result = NULL;
    while (A)
    {
        insertSentence(&result, A->exp);
        A = A->next;
    }
    while (B)
    {
        insertSentence(&result, B->exp);
        B = B->next;
    }
    return result;
}

/*  Iterates through A and checks existence in B using searchSentence */
sentence *intersectionSen(sentence *A, sentence *B)
{
    sentence *result = NULL;
    while (A)
    {
        if (searchSentence(B, A->exp)) // logical AND (exists in both)
            insertSentence(&result, A->exp);
        A = A->next;
    }
    return result;
}

/*  Iterates through A and keeps only those not found in B */
sentence *differenceSen(sentence *A, sentence *B)
{
    sentence *result = NULL;
    while (A)
    {
        if (!searchSentence(B, A->exp)) // logical NOT (exists in A but not B)
            insertSentence(&result, A->exp);
        A = A->next;
    }
    return result;
}

/*  Returns 0 immediately if any element of A is missing from B */
int subSetsSen(sentence *A, sentence *B)
{
    while (A)
    {
        if (!searchSentence(B, A->exp))
            return 0; // violation of subset rule
        A = A->next;
    }
    return 1; // all elements passed the search check
}

/* ═══════════════════════════════════════════════*/

/* Logic: Traverses the list i times to reach the n-th paragraph node */
Paragraph *getParagraphByIndex(Paragraph *head, int n)
{
    int i = 0;
    while (head)
    {
        if (i == n)
            return head; // target index reached
        i++;
        head = head->next;
    }
    return NULL;
}

/* Pointer traversal counter for the paragraph linked list */
int countParagraphs(Paragraph *head)
{
    int c = 0;
    while (head)
    {
        c++;
        head = head->next;
    }
    return c;
}

/* ══════════════════════════════════════════════*/

void toLowerCase(char *str)
// to normalize string (  ex THAT =That =that )
{
    for (int i = 0; str[i]; i++)
        str[i] = (char)tolower((unsigned char)str[i]);
}

void trimSpaces(char *str)
{
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t')
        start++;
    if (start > 0)
        memmove(str, str + start, strlen(str) - start + 1);

    int end = (int)strlen(str) - 1;
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' ||
                        str[end] == '\r' || str[end] == '\n'))
        str[end--] = '\0';
}

/* uses strtok to split based on punctuation delimiters */
// strtok:It breaks a long string into smaller parts (tokens)
// based on specific characters like dots or comma
void tokenizeLineIntoSentences(char *line, Paragraph *p)
{
    char copy[1024];
    strncpy(copy, line, sizeof(copy) - 1);

    char *token = strtok(copy, ".!?,"); // split by sentence endings
    while (token)
    {
        trimSpaces(token);// clean up leading/trailing spaces from the token
        if (strlen(token) >= 1)
            addSentenceToParagraph(p, token);
        token = strtok(NULL, ".!?,");
    }
}

//  reads line by line; empty lines trigger a new paragraph node creation */
Paragraph *loadFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("  [!] Cannot open file: %s\n", filename);
        return NULL;
    }

    Paragraph *fileHead = NULL;
    char line[1024];
    int paraIndex = 1;
    int inParagraph = 0;
    Paragraph *curPara = NULL;
    char label[50];

    while (fgets(line, sizeof(line), fp))// read line by line
    {
        trimSpaces(line);

        if (strlen(line) == 0)
        {
            inParagraph = 0; // blank line detected
            curPara = NULL;
        }
        else
        {
            if (!inParagraph)
            {
                snprintf(label, sizeof(label), "P%d", paraIndex++);
                curPara = createParagraph(label); // instantiate new paragraph
                insertParagraph(&fileHead, curPara);
                inParagraph = 1;
            }
            tokenizeLineIntoSentences(line, curPara);
        }
    }
    fclose(fp);
    return fileHead;
}

/* ═════════════════════════*/

/*  loops paragraphs then loops sentences within each */
int searchExpressionInFile(Paragraph *file, const char *expr)
{
    while (file)
    {
        if (searchSentence(file->sentences, expr))// if found in any paragraph, return true
            return 1;
        file = file->next;
    }
    return 0;
}

/*  bidirectional subset check; if A subset of B and B subset of A, they are equal */
int compareParagraphs(Paragraph *p1, Paragraph *p2)
{
    if (!p1 || !p2)
        return 0;
    return subSetsSen(p1->sentences, p2->sentences) &&
           subSetsSen(p2->sentences, p1->sentences);
}

/* ════════════════════COMPLEMENT═══════════════════════════*/
sentence *complementInFile(Paragraph *file, Paragraph *target)// to find the complement of a paragraph in a file
{

    sentence *universe = NULL;
    Paragraph *cur = file;
    /* Accumulate every sentence in the file into one universe  set  ; without dup*/
    while (cur)
    {
        sentence *s = cur->sentences;
        while (s)
        {
            insertSentence(&universe, s->exp);
            s = s->next;
        }
        cur = cur->next;
    }

    /*  Result = Universe - Target  */
    sentence *comp = differenceSen(universe, target->sentences);
    freeSentences(universe);
    return comp;
}