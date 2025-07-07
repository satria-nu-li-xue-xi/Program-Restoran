#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==============================
// ADT Menu
// ==============================
typedef struct Menu {
    int id;
    char nama[50];
    float harga;
} Menu;

// ==============================
// AVL Tree untuk daftar menu
// ==============================
typedef struct AVLNode {
    Menu data;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

int max(int a, int b) { return (a > b) ? a : b; }
int height(AVLNode *N) { return N ? N->height : 0; }

AVLNode* createNode(Menu data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data; node->left = node->right = NULL; node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode *y) {
    AVLNode *x = y->left, *T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode *x) {
    AVLNode *y = x->right, *T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(AVLNode *N) { return N ? height(N->left) - height(N->right) : 0; }

AVLNode* insertAVL(AVLNode* node, Menu data) {
    if (!node) return createNode(data);
    if (strcmp(data.nama, node->data.nama) < 0) node->left = insertAVL(node->left, data);
    else if (strcmp(data.nama, node->data.nama) > 0) node->right = insertAVL(node->right, data);
    else return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(data.nama, node->left->data.nama) < 0) return rightRotate(node);
    if (balance < -1 && strcmp(data.nama, node->right->data.nama) > 0) return leftRotate(node);
    if (balance > 1 && strcmp(data.nama, node->left->data.nama) > 0) {
        node->left = leftRotate(node->left); return rightRotate(node); }
    if (balance < -1 && strcmp(data.nama, node->right->data.nama) < 0) {
        node->right = rightRotate(node->right); return leftRotate(node); }
    return node;
}

void inorderTraversal(AVLNode *root) {
    if (root) {
        inorderTraversal(root->left);
        printf("%d. %s - Rp %.2f\n", root->data.id, root->data.nama, root->data.harga);
        inorderTraversal(root->right);
    }
}

AVLNode* searchMenuByName(AVLNode* root, char* keyword) {
    if (!root) return NULL;
    int cmp = strcmp(keyword, root->data.nama);
    if (cmp == 0) return root;
    else if (cmp < 0) return searchMenuByName(root->left, keyword);
    else return searchMenuByName(root->right, keyword);
}

// ==============================
// Linked List untuk pesanan per pelanggan
// ==============================
typedef struct OrderNode {
    Menu menu;
    int quantity;
    struct OrderNode *next;
} OrderNode;

OrderNode* addOrder(OrderNode *head, Menu menu, int qty) {
    OrderNode *newNode = (OrderNode*)malloc(sizeof(OrderNode));
    newNode->menu = menu; newNode->quantity = qty; newNode->next = head;
    return newNode;
}

void printOrders(OrderNode *head) {
    OrderNode *current = head; int i = 1;
    while (current) {
        printf("%d. %s x%d - Rp %.2f\n", i++, current->menu.nama,
               current->quantity, current->menu.harga * current->quantity);
        current = current->next;
    }
}

// ==============================
// Stack untuk histori Undo/Redo per pelanggan
// ==============================
#define MAX 100
OrderNode* stack[MAX];
int top = -1;

void push(OrderNode* orders) {
    if (top < MAX - 1) stack[++top] = orders;
}

OrderNode* pop() {
    return (top >= 0) ? stack[top--] : NULL;
}

// ==============================
// Queue untuk antrean pelanggan
// ==============================
typedef struct QueueNode {
    OrderNode *orders; // satu pelanggan = linked list pesanan
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL; return q;
}

void enqueue(Queue* q, OrderNode* orders) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->orders = orders; temp->next = NULL;
    if (!q->rear) { q->front = q->rear = temp; return; }
    q->rear->next = temp; q->rear = temp;
}

QueueNode* dequeue(Queue* q) {
    if (!q->front) return NULL;
    QueueNode* temp = q->front;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    return temp;
}

void printQueue(Queue* q) {
    QueueNode* temp = q->front; int i = 1;
    while (temp) {
        printf("Pelanggan %d:\n", i++);
        printOrders(temp->orders);
        temp = temp->next;
    }
}

// ==============================
// MAIN Program
// ==============================
int main() {
    AVLNode *menuTree = NULL;
    Queue *customerQueue = createQueue();

    // Seed menu
    Menu m1 = {1, "Ayam Goreng", 20000};
    Menu m2 = {2, "Es Teh", 5000};
    Menu m3 = {3, "Nasi Goreng", 25000};
    Menu m4 = {4, "Soto Ayam", 22000};
    menuTree = insertAVL(menuTree, m1);
    menuTree = insertAVL(menuTree, m2);
    menuTree = insertAVL(menuTree, m3);
    menuTree = insertAVL(menuTree, m4);

    int choice;
    do {
        printf("\n===== Menu Restoran =====\n");
        printf("1. Tampilkan Menu\n");
        printf("2. Cari Menu\n");
        printf("3. Tambah Pesanan (multi-menu per pelanggan)\n");
        printf("4. Lihat Antrean Pelanggan\n");
        printf("5. Batalkan Pesanan Pelanggan Terakhir (Undo)\n");
        printf("6. Kembalikan Pesanan Pelanggan Terakhir (Redo)\n");
        printf("7. Layani Pelanggan Berikutnya\n");
        printf("0. Keluar\nPilih: ");
        scanf("%d", &choice);

        if (choice == 1) inorderTraversal(menuTree);
        else if (choice == 2) {
            char keyword[50];
            printf("Nama menu: "); scanf(" %[^\n]", keyword);
            AVLNode* f = searchMenuByName(menuTree, keyword);
            f ? printf("Ditemukan: %s - Rp %.2f\n", f->data.nama, f->data.harga)
              : printf("Menu tidak ditemukan.\n");
        }
        else if (choice == 3) {
            OrderNode *tempOrderList = NULL;
            char keyword[50];
            int count = 0;
            while (1) {
                printf("Nama menu (\"selesai\" untuk berhenti): ");
                scanf(" %[^\n]", keyword);
                if (!strcmp(keyword, "selesai")) break;
                AVLNode* f = searchMenuByName(menuTree, keyword);
                if (f) {
                    int qty; printf("Jumlah untuk %s: ", f->data.nama); scanf("%d", &qty);
                    tempOrderList = addOrder(tempOrderList, f->data, qty);
                    printf("'%s' x%d ditambahkan.\n", f->data.nama, qty);
                    count++;
                } else printf("Menu tidak ditemukan.\n");
            }
            if (count > 0) {
                enqueue(customerQueue, tempOrderList);
                printf("Pesanan pelanggan dimasukkan ke antrean.\n");
            }
        }
        else if (choice == 4) customerQueue->front ? printQueue(customerQueue) : printf("Antrean kosong.\n");
        else if (choice == 5) {
            if (customerQueue->front) {
                QueueNode* removed = dequeue(customerQueue);
                push(removed->orders);
                printf("Pesanan pelanggan dibatalkan & disimpan ke histori.\n");
                free(removed);
            } else printf("Tidak ada pelanggan untuk dibatalkan.\n");
        }
        else if (choice == 6) {
            OrderNode* redo = pop();
            if (redo) {
                enqueue(customerQueue, redo);
                printf("Pesanan pelanggan dikembalikan ke antrean.\n");
            } else printf("Tidak ada histori pembatalan.\n");
        }
        else if (choice == 7) {
            QueueNode* served = dequeue(customerQueue);
            if (!served) printf("Antrean kosong.\n");
            else {
                printf("Melayani pelanggan:\n");
                printOrders(served->orders);
                free(served);
            }
        }
    } while (choice != 0);

    printf("Terima kasih!\n");
    return 0;
}
