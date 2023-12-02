#include <stdio.h>
#include <strings.h>
#include <cstring>
#include <clang-c/Index.h>
#include <cstdlib>


typedef struct node{
    char src_idx[50];
    char dest_idx[50];
    char edge_type[50];
    bool does_ret;
    struct node *next;
}Node;

typedef struct dict_node{
    char name[50];
    struct dict_node *next;
}D_Node;

Node *head = NULL;
D_Node *dict_head = NULL;

void add_node(const char* x, const char* y, const char* edge_type){
    Node * new_node = NULL;
    new_node = (Node *) malloc(sizeof(Node));
    
    strcpy(new_node->src_idx,x);
    if(strcmp(x, "") != 0){
        char * token;
        token = strtok(new_node->src_idx, "(");
        strcpy(new_node->src_idx,token);
    }
    strcpy(new_node->dest_idx,y);
    strcpy(new_node->edge_type,edge_type);

    if(head == NULL){
        head = new_node;
    } else{
        Node * current = head;
        while (current->next != NULL) {
        current = current->next;
        }
        current->next = new_node;
    }
}

void clean_list(){
    Node * current = head;
    while (current != NULL){
        if (strcmp(current->src_idx, "")  == 0 || strcmp(current->dest_idx, "")  == 0){
            head = current->next;
            free(current);
            current->next = head;
        }
        current = current->next;
    }
}

bool is_external(char * dest){
    D_Node * dict_current = dict_head;
    while(dict_current != NULL){
        if(strcmp(dict_current->name, dest) == 0){
            return false;
        }
        dict_current = dict_current->next;
    }
    return true;
}

void restructure_list(){
    if (head != NULL){
        Node * current = head;
        char cur_src[50] = "";
        while (current != NULL){
            if (strcmp(current->src_idx, "") != 0){
                strcpy(cur_src, current->src_idx);
            }else if (strcmp(current->edge_type, "ReturnStmt") == 0){
                add_node(current->dest_idx, cur_src, current->edge_type);
            }else if(strcmp(current->dest_idx, "") != 0 && is_external(current->dest_idx) == 0){
                add_node(cur_src, current->dest_idx, current->edge_type);
            }
            current = current->next;
        }
        clean_list();
    }
}

void delete_list(){
    char current_func[50] = "";
    if(head != NULL){
        Node * current = head;
        current = current->next;
        head->next = NULL;
        free(head);

        while (current != NULL) {
            head = current;
            current = current->next;
            head->next = NULL;
            free(head);
        }
    }
}

void print_list(){
    Node * current = head;
    while(current != NULL){
        printf("src: %s\tdest: %s\ttype: %s\n",current->src_idx,current->dest_idx, current->edge_type);
        current = current->next;
    }
}

void write_list(){
    FILE *fpt;
    fpt = fopen("edgelist.csv", "w");
    Node * current = head;
    while(current != NULL){
        fprintf(fpt,"%s, %s, %s\n",current->src_idx,current->dest_idx, current->edge_type);
        current = current->next;
    }
    fclose(fpt);
}

void dict_add_node(const char* x){
    D_Node * new_node = NULL;
    new_node = (D_Node *) malloc(sizeof(D_Node));
    
    strcpy(new_node->name,x);
    if(strcmp(x, "") != 0){
        char * token;
        token = strtok(new_node->name, "(");
        strcpy(new_node->name,token);
    }

    if(dict_head == NULL){
        dict_head = new_node;
    } else{
        D_Node * current = dict_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void delete_dict_list(){
    char current_func[50] = "";
    if(dict_head != NULL){
        D_Node * current = dict_head;
        current = current->next;
        dict_head->next = NULL;
        free(dict_head);

        while (current != NULL) {
            dict_head = current;
            current = current->next;
            dict_head->next = NULL;
            free(dict_head);
        }
    }
}

void print_dict_list(){
    D_Node * current = dict_head;
    while(current != NULL){
        printf("Function: %s\n",current->name);
        current = current->next;
    }
}