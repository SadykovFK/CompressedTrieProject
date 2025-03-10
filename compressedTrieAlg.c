#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 256

typedef struct Node
{
    struct Node** children;
    char* edge;
    int edge_len;
} Node;

Node* create(void)
{
    Node* node = malloc(sizeof(Node));
    node->children = calloc(ALPHABET, sizeof(Node*));
    node->edge = NULL;
    node->edge_len = 0;
    return node;
}

void insert(Node* root, char* line, int start, int len)
{
    Node* cur = root;
    char* substring = line + start;
    int i = 0; 

    while (i < len) {
        int index = substring[i] - 'a';
        if (index < 0 || index >= ALPHABET) {
        return;
        }

        if (cur->children[index] == NULL) {
            Node* new_node = create();
            new_node->edge = strndup(substring + i, len - i);
            new_node->edge_len = len - i;
            cur->children[index] = new_node;
            return;
        }

        Node* child = cur->children[index];
        int j = 0;
        while ((j < child->edge_len) && (i < len) && (child->edge[j] == substring[i])) {
            i++;
            j++;
        }

        if (j == child->edge_len) {
            cur = child;
        } else {
            Node* split = create();
            split->edge = strndup(child->edge, j);
            split->edge_len = j;
            cur->children[index] = split;

            char* rem_edge = strndup(child->edge + j, child->edge_len - j);
            free(child->edge);
            child->edge = rem_edge;
            child->edge_len -= j;

            split->children[child->edge[0] - 'a'] = child;
            Node* new_node = create();
            new_node->edge = strndup(substring + i, len - i);
            new_node->edge_len = len - i;
            if ((new_node->edge[0] >= 'a') && (new_node->edge[0] <= 'z')) {
                split->children[new_node->edge[0] - 'a'] = new_node;
            } else {
                free(new_node->edge);
                free(new_node);
            }
            return;
        }
    }
}

int subs_cnt(Node* node)
{
    if (node == NULL) return 0;

    int cnt = 0;

    if (node->edge) {
        cnt += node->edge_len;
    }

    for (int i = 0; i < ALPHABET; i++) {
        cnt += subs_cnt(node->children[i]);
    }

    return cnt;
}

void free_trie(Node* node)
{   
    if (node == NULL) return;

    for (int i = 0; i < ALPHABET; i++) {
        free_trie(node->children[i]);
    }

    free(node->children);

    if (node->edge) free(node->edge);

    free(node);
}

int unique_cnt(char* line)
{
    int len = strlen(line);
    Node* root = create();

    for (int i = 0; i < len; i++) {
        insert(root, line, i, len - i);
    }

    int answ = subs_cnt(root);
    free_trie(root);

    return answ + 1;
}

int main(void)
{
    char* line = malloc(sizeof(char) * 2002);
    fgets(line, 2002, stdin);

    int length = strlen(line);
    line[length - 1] = '\0';
    

    printf("%d\n", unique_cnt(line));

    free(line);
    return 0;
}