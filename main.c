#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

char GetNextNonspace(void);

int main(int argc, char** argv) {
    Matrix m = GetMatrixFromUser();

    // errno

    PrintMatrix(head);

    FreeNodes(head);
    return 0;
}

Matrix GetMatrixFromUser(void) {
    int c = GetNextNonspace();
    if(c != '[') {
        // strcpy(packet->error, "invalid syntax: matrix must start with a [\n");
        // errno
        return NULL;
    }

    int num = 0;
    while((c = GetNextNonspace())) {
        if(isdigit(c)) {
            num *= 10;
            num += (c - '0');
        } else if(c == ',') {
            AppendToMatrix(&matrixEnd, num);
            if(head == NULL) {
                head = matrixEnd;
            }

            num = 0;
        } else if(c == ']'){
            AppendToMatrix(&matrixEnd, num);
            if(head == NULL) {
                head = matrixEnd;
            }

            break;
        } else {
            // strcpy(packet->error, "invalid syntax: matrix must contain only digits, commas, and spaces. matrix must end with a ].\n");
            return NULL;
        }
    }

    return head;
}

void AppendToMatrix(Node** matrixEnd, int num) {
    if((*matrixEnd) == NULL) {
        (*matrixEnd) = CreateNode();
        (*matrixEnd)->data = num;
        return;
    }

    Node* node = CreateNodeAfter((*matrixEnd));

    node->prev = (*matrixEnd);
    (*matrixEnd)->next = node;

    node->data = num;

    // change where matrix end points to
    (*matrixEnd) = node;
}

void PrintMatrix(Node* head) {
    printf("[");

    if(head == NULL) {
        printf("]");
        return;
    }

    for(head; head->next != NULL; head=head->next) {
        printf("%d,", head->data);
    }

    printf("%d]\n", head->data);
}

Node* CreateNode(void) {
    Node* head = (Node*)malloc(sizeof(Node));
    head->prev = NULL;
    head->next = NULL;
    return head;
}

Node* CreateNodeAfter(Node* cur) {
    Node* node = (Node*)malloc(sizeof(Node));
    cur->next = node;
    node->prev = cur;
    node->next = NULL;
    return node;
}

void FreeNodes(Node* head) {
    Node* ptr = head;
    Node* tmp;
    while(ptr != NULL) {
        tmp = ptr;
        ptr = ptr->next;

        // free the node
        free(tmp);
    }
}

char GetNextNonspace(void) {
    char c;
    while(isspace(c = getchar()));
    return c;
}
