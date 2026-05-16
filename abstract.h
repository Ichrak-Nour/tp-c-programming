
#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Sentence node ── each paragraph is a linked list of sentences/expressions */
typedef struct sentence
{
    char exp[500];
    struct sentence *next;
} sentence;

/* ── Paragraph node ── each file is a linked list of paragraphs */
typedef struct Paragraph
{
    char label[50];      // the title of each paragraph
    sentence *sentences; /* linked list of sentences inside this paragraph */
    struct Paragraph *next;
} Paragraph;

/*  Abstract machine – sentence  */
sentence *allocateSentence(void);
sentence *nextSentence(sentence *p);
void ass_sentence(sentence *p, const char *exp);
void ass_adr_Sentence(sentence *p, sentence *adr);
sentence *createSentence(const char *e);
int searchSentence(sentence *head, const char *e);
void insertSentence(sentence **head, const char *e);
void deleteSentence(sentence **head, const char *e);
void displaySentences(sentence *head);
void freeSentences(sentence *head);
char *wfield(sentence *p);

/* ══ Abstract machine – paragraph ══ */
Paragraph *allocateParagraph(void);
Paragraph *nextParagraph(Paragraph *p);
void ass_adr_Paragraph(Paragraph *p, Paragraph *adr);
void ass_ptr(Paragraph *p, sentence *a);
Paragraph *createParagraph(const char *label);
void insertParagraph(Paragraph **head, Paragraph *newP);
void displayParagraphs(Paragraph *head);
void addSentenceToParagraph(Paragraph *p, const char *sent);
void freeParagraphs(Paragraph *head);
sentence *ptrfield(Paragraph *p);

#endif /* ABSTRACT_H */