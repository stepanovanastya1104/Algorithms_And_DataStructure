#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>


enum Colors {
    RED,    //0
    ORANGE, //1
    YELLOW, //2
    GREEN,  //3
    BLUE,   //4
    INDIGO, //5
    VIOLET, //6
    PINK,   //7
    BROWN,  //8
    WHITE,  //9
    BLACK   //10
};

typedef struct Node {
    enum Colors data;
    int cnt;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(enum Colors value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
        return NULL;
    newNode->data = value;
    newNode->cnt = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* addNode(Node* root, int val) {
    enum Colors value = (enum Colors)val;
    if (root == NULL)
        return createNode(value);
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    else {
        root->cnt++;
    }
    return root;
}

Node* findMinDepth(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

int findVal(Node* root, int val) {
    enum Colors value = (enum Colors)val;
    if (root == NULL)
        return 0;
    if (root->data == value)
        return 1;
    if (value < root->data)
        return findVal(root->left, value);
    return findVal(root->right, value);
}

Node* deleteNode(Node* root, int val) {
    enum Colors value = (enum Colors)val;
    if (root == NULL)
        return root;
    if (value < root->data) {
        root->left = deleteNode(root->left, value);
    } else if (value > root->data) {
        root->right = deleteNode(root->right, value);
    } else {
        if (root->cnt > 1) {
            root->cnt--;
            return root;
        }
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = findMinDepth(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

void printTree(Node* root, int level) {
    char *color_names[] = {
        "RED",
        "ORANGE",
        "YELLOW",
        "GREEN",
        "BLUE",
        "INDIGO",
        "VIOLET",
        "PINK",
        "BROWN",
        "WHITE",
        "BLACK"
    };
    if (root == NULL)
        return;
    printTree(root->right, level + 1);
    for (int i = 0; i < level; i++) printf("----");
    printf("%s(%d)\n", color_names[root->data], root->cnt);
    printTree(root->left, level + 1);
}

int getMinDepth(Node* root) {
    if (root == NULL)
        return -1;
    int depth = 0;
    Node* current = root;
    while (current->left != NULL) {
        current = current->left;
        depth++;
    }
    return depth;
}

void freeTree(Node* root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    Node* root = NULL;
    int choice, val;
    while (1) {
        printf("\nМЕНЮ\n");
        printf("1 - Добавить число\n");
        printf("2 - Показать дерево\n");
        printf("3 - Удалить число\n");
        printf("4 - Глубина минимальной вершины\n");
        printf("5 - Завершить\n");
        printf("Выполнить функцию:\n");
        if (scanf("%d", &choice) != 1) {
            printf("Дерево пусто!\n");
            break;
        }
        switch (choice) {
            case 1:
                printf("Введите новую вершину (от 0 до 10): ");
                scanf("%d", &val);
                if (val < 0 || val > 10) {
                    printf("Ошибка: число должно быть от 0 до 10\n");
                    break;
                }
                root = addNode(root, val);
                break;
            case 2:
                val = getMinDepth(root);
                if (val == -1) {
                    printf("Дерево пусто!\n");
                    break;
                }
                printTree(root, 0);
                break;
            case 3:
                val = getMinDepth(root);
                if (val == -1) {
                    printf("Дерево пусто, удалять нечего!\n");
                    break;
                }
                printf("Какое число удалить (от 0 до 10):\n");
                scanf("%d", &val);
                if (val < 0 || val > 10) {
                    printf("Ошибка: число должно быть от 0 до 10\n");
                    break;
                }
                if (!findVal(root, val)) {
                    printf("Такого значения в дереве нет!\n");
                } else {
                    if (val == root->data && root->cnt == 1) {
                        printf("Удаление корня! Дерево удалено. Завершение программы. \n");
                        freeTree(root);
                        return 0;
                    }
                    root = deleteNode(root, val);
                    printf("Цвет удален!\n");
                }
                break;
            case 4:
                val = getMinDepth(root);
                if (val == -1) {
                    printf("Дерево пусто!\n");
                } else {
                    printf("Глубина минимальной вершины: %d\n", val);
                }
                break;
            case 5:
                freeTree(root);
                return 0;
            default:
                printf("Ошибка! Введите число от 1 до 5\n");
        }
    }
    return 0;
}
