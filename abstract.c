#include "abstract.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*       sentence          */
sentence *allocateSentence(void)
{
    sentence *p = (sentence *)malloc(sizeof(sentence));
    if (!p)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    p->exp[0] = '\0';
    p->next = NULL;
    return p;
}

sentence *nextSentence(sentence *p)
{
    return p ? p->next : NULL;
}

void ass_sentence(sentence *p, const char *exp)
{
    if (p)
        strncpy(p->exp, exp, sizeof(p->exp) - 1);
}

void ass_adr_Sentence(sentence *p, sentence *adr)
{
    if (p)
        p->next = adr;
}

/* Create a new sentence node with given expression */
sentence *createSentence(const char *e)
{
    sentence *p = allocateSentence();
    ass_sentence(p, e);
    return p;
}

/* Returns 1 if expression e exists in the list, 0 otherwise */
int searchSentence(sentence *head, const char *e)
{
    while (head)
    {
        if (strcmp(head->exp, e) == 0)
            return 1;
        head = head->next;
    }
    return 0;
}

/* Append sentence at the end of the list (no duplicates) */
void insertSentence(sentence **head, const char *e)
{
    if (searchSentence(*head, e))
        return; /* set: no duplicates */
    sentence *newS = createSentence(e);
    if (!*head)
    {
        *head = newS;
        return;
    }
    sentence *cur = *head;
    while (cur->next)
        cur = cur->next;
    cur->next = newS;
}

/* Remove a sentence by expression value */
void deleteSentence(sentence **head, const char *e)
{
    if (!*head)
        return;
    if (strcmp((*head)->exp, e) == 0)
    {
        sentence *tmp = *head;
        *head = (*head)->next;
        free(tmp);
        return;
    }
    sentence *cur = *head;
    while (cur->next && strcmp(cur->next->exp, e) != 0)
        cur = cur->next;
    if (cur->next)
    {
        sentence *tmp = cur->next;
        cur->next = tmp->next;
        free(tmp);
    }
}

void displaySentences(sentence *head)
{
    if (!head)
    {
        printf("  (empty)\n");
        return;
    }
    while (head)
    {
        printf("   %s\n", head->exp);
        head = head->next;
    }
}

void freeSentences(sentence *head)
{
    while (head)
    {
        sentence *tmp = head->next;
        free(head);
        head = tmp;
    }
}

char *wfield(sentence *p)
{
    return p ? p->exp : NULL;
}

/* ════════paragraph ════════*/

Paragraph *allocateParagraph(void)
{
    Paragraph *p = (Paragraph *)malloc(sizeof(Paragraph));
    if (!p)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
        p->label[0] = '\0';
    p->sentences = NULL;
    p->next = NULL;
    return p;
}

Paragraph *nextParagraph(Paragraph *p)
{
    return p ? p->next : NULL;
}

void ass_adr_Paragraph(Paragraph *p, Paragraph *adr)
{
    if (p)
        p->next = adr;
}

void ass_ptr(Paragraph *p, sentence *a)
{
    if (p)
        p->sentences = a;
}

Paragraph *createParagraph(const char *label)
{
    Paragraph *p = allocateParagraph();
    strncpy(p->label, label, sizeof(p->label) - 1);
    return p;
}

/* Append paragraph at the end of the list */
void insertParagraph(Paragraph **head, Paragraph *newP)
{
    if (!*head)
    {
        *head = newP;
        return;
    }
    Paragraph *cur = *head;
    while (cur->next)
        cur = cur->next;
    cur->next = newP;
}

void displayParagraphs(Paragraph *head)
{
    int i = 1;
    while (head)
    {
        printf("  --- %s ---\n", head->label);
        displaySentences(head->sentences);
        head = head->next;
        i++;
    }
}

void addSentenceToParagraph(Paragraph *p, const char *sent)
{
    if (p)
        insertSentence(&p->sentences, sent);
}

void freeParagraphs(Paragraph *head)
{
    while (head)
    {
        Paragraph *tmp = head->next;
        freeSentences(head->sentences);
        free(head);
        head = tmp;
    }
}

sentence *ptrfield(Paragraph *p)
{
    return p ? p->sentences : NULL;
}