#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abstract.h"
#ifndef LIBRARY_H
#define LIBRARY_H
/* ── Basic list  modules ── */
int length(sentence *head);// to calculate the length of a sentence list
int frequency(sentence *head, const char *w);// to calculate the frequency of a specific sentence in a list
sentence *concatenation(sentence *a, sentence *b);// to concatenate two sentence lists into a new one
void deleteDuplicates(sentence **head);// to remove duplicate sentences from a list

/* ── Set operations on Paragraph lists ── */
Paragraph *intersectionPara(Paragraph *A, Paragraph *B);// to do the intersection of two paragraph lists
int subSetsPara(Paragraph *A, Paragraph *B);// to check if A is a subset of B for paragraph lists

/* ── Set operations on sentence lists ── */
sentence *unionSen(sentence *A, sentence *B);// to do the union of two sentence lists
sentence *intersectionSen(sentence *A, sentence *B);// to do the intersection of two sentence lists
sentence *differenceSen(sentence *A, sentence *B);// to do the difference of two sentence lists
int subSetsSen(sentence *A, sentence *B);// to check if A is a subset of B for sentence lists
Paragraph *getParagraphByIndex(Paragraph *head, int n);// to get the memory address of the n-th paragraph in the list
int countParagraphs(Paragraph *head);// to count the number of paragraphs in a list

// File loading
Paragraph *loadFile(const char *filename);// to read a file and convert it into a linked list of paragraphs and sentences
void toLowerCase(char *str);// to normalize string (  ex THAT =That =that )
void trimSpaces(char *str);// to clean up leading/trailing spaces from the token
void tokenizeLineIntoSentences(char *line, Paragraph *p);// to split a line into sentences and add them to a paragraph
int searchExpressionInFile(Paragraph *file, const char *expr);// to search for a specific expression in the file (paragraph list)
int compareParagraphs(Paragraph *p1, Paragraph *p2);// to compare two paragraphs for equality based on their sentences
sentence *complementInFile(Paragraph *file, Paragraph *target); // to find the complement of a paragraph in a file (sentences in the file that are not in the target paragraph)q


#endif /* LIBRARY_H */