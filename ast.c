#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum Type {
    NUMBER,
    OPERATOR,
    LPAREN,
    RPAREN
};

typedef struct REQ {
    enum Type type;
    char text[100];
    char op;
} REQ;

typedef struct Node {
    char op;
    char *value;
    struct Node *left;
    struct Node *right;
} Node;

Node *createValue(const char *v) {
    Node *newNode = calloc(1, sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->value = (char *)malloc(strlen(v) + 1);
    if (newNode->value == NULL) {
        free(newNode);
        return NULL;
    }
    strcpy(newNode->value, v);
    return newNode;
}

Node *createOp(char op, Node *left, Node *right) {
    Node *newNode = calloc(1, sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->op = op;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

int is_op(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int order(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    if (op == '~') {
        return 3;
    }
    return 0;
}

int analyzer(const char *s, REQ *arr, int *n) {
    int k = 0;
    for (int i = 0; s[i] != '\0' && s[i] != '\n';) {
        if (isspace(s[i])) {
            i++;
            continue;
        }
        if (isalnum(s[i])) {
            int start = i;
            i++;
            while (isalnum(s[i])) {
                i++;
            }
            int length = i - start;
            memcpy(arr[k].text, s + start, length);
            arr[k].text[length] = '\0';
            arr[k].type = NUMBER;
            k++;
            continue;
        }
        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
            arr[k].type = OPERATOR;
            arr[k].op = s[i];
            arr[k].text[0] = s[i];
            arr[k].text[1] = '\0';
            k++;
            i++;
            continue;
        }
        if (s[i] == '(') {
            arr[k].type = LPAREN;
            k++;
            i++;
            continue;
        }
        if (s[i] == ')') {
            arr[k].type = RPAREN;
            k++;
            i++;
            continue;
        }
        printf("Неправильный ввод\n");
        return 0;
    }
    *n = k;
    return 1;
}

int to_postfix(const REQ *in, int in_n, REQ *out, int *out_n) {
    REQ stack[1000];
    int top = -1;
    int j = 0;
    enum Type prev = OPERATOR;
    for (int i = 0; i < in_n; i++) {
        REQ cur = in[i];
        if (cur.type == NUMBER) {
            out[j++] = cur;
            prev = NUMBER;
            continue;
        }
        if (cur.type == LPAREN) {
            stack[++top] = cur;
            prev = LPAREN;
            continue;
        }
        if (cur.type == RPAREN) {
            while (top >= 0 && stack[top].type != LPAREN) {
                out[j++] = stack[top--];
            }
            if (top < 0)
                return 0;
            top--;
            prev = RPAREN;
            continue;
        }
        if (cur.type == OPERATOR) {
            if (cur.op == '-' && (i == 0 || prev == OPERATOR || prev == LPAREN)) {
                cur.op = '~';
            }
            while (top >= 0 && stack[top].type == OPERATOR) {
                int p1 = order(stack[top].op);  // приоритет того, что в стеке
                int p2 = order(cur.op);          // приоритет нового оператора
                if ((cur.op == '~' && p2 < p1) || (cur.op != '~' && p2 <= p1)) {
                    out[j++] = stack[top--];
                } else {
                    break;
                }
            }
            stack[++top] = cur;
            prev = OPERATOR;
        }
    }
    while (top >= 0) {
        if (stack[top].type != OPERATOR)
            return 0;
        out[j++] = stack[top--];
    }
    *out_n = j;
    return 1;
}

Node *buildTree(const REQ *pf, int n) {
    Node *stack[1000];
    int top = -1;
    for (int i = 0; i < n; i++) {
        if (pf[i].type == NUMBER) {
            Node *leaf = createValue(pf[i].text);
            stack[++top] = leaf;
            continue;
        }
        if (pf[i].type == OPERATOR && pf[i].op == '~') {
            Node *right = stack[top--];
            Node *node = createOp('~', NULL, right);
            stack[++top] = node;
            continue;
        }
        if (pf[i].type == OPERATOR) {
            Node *right = stack[top--];
            Node *left  = stack[top--];
            Node *node = createOp(pf[i].op, left, right);
            stack[++top] = node;
        }
    }
    return stack[0];
}

void transform(Node *node) {
    if (node == NULL)
        return;
    transform(node->left);
    transform(node->right);
    if (node->op == '-') {
        Node *neg = createOp('~', NULL, node->right);
        if (neg != NULL) {
            node->op = '+';
            node->right = neg;
        }
    }
}

void printInfix(const Node *node) {
    if (node == NULL) {
        return;
    }
    if (node->value != NULL) {
        printf("%s", node->value);
        return;
    }
    if (node->op == '~') {
        printf("(-");
        printInfix(node->right);
        printf(")");
        return;
    }
    printf("(");
    printInfix(node->left);
    printf(" %c ", node->op);
    printInfix(node->right);
    printf(")");
}

void printTree(const Node *node, int d) {
    if (node == NULL) {
        return;
    }
    printTree(node->right, d + 1);
    for (int i = 0; i < d; i++) {
        printf("    ");
    }
    if (node->value != NULL) {
        printf("%s\n", node->value);
    } else if (node->op == '~') {
        printf("~\n");
    } else {
        printf("%c\n", node->op);
    }
    printTree(node->left, d + 1);
}

void freeTree(Node *node) {
    if (node == NULL) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    free(node->value);
    free(node);
}

int main() {
    system("chcp 65001");
    char input[1000];
    REQ arr[1000], postfix[1000];
    int tn = 0, pn = 0;
    printf("Пример: a - b * с -> a + (-(b * с))\n");
    printf("Введите выражение: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Неправильный ввод\n");
        return 1;
    }
    if (!analyzer(input, arr, &tn) || !to_postfix(arr, tn, postfix, &pn)) {
        return 1;
    }
    Node *root = buildTree(postfix, pn);
    if (root == NULL) {
        return 1;
    }
    printf("\nДо:\n");
    printInfix(root);
    printf("\n\nДерево до:\n");
    printTree(root, 0);
    transform(root);
    printf("\nПосле:\n");
    printInfix(root);
    printf("\n\nДерево после:\n");
    printTree(root, 0);
    freeTree(root);
    return 0;
}
