#include <stdio.h>
#include <strings.h>
#include <cstring>
#include <clang-c/Index.h>
#include <cstdlib>


typedef struct node{
    char src_idx[50];
    char dest_idx[50];
    char edge_type[50];
    struct node *next;
}Node;

typedef struct dict_node{
    char name[50];
    char return_type[50];
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

bool is_void(char* dest){
    D_Node * tmp = dict_head;
    while(tmp != NULL){
        if(strcmp(tmp->name, dest) == 0){
            if(strcmp(tmp->return_type, "void") == 0){
                return true;
            } else{
                return false;
            }
        }
        tmp = tmp->next;
    }
    return true;
}

void print_dict_list(){
    D_Node * current = dict_head;
    if(current == NULL){
        printf("Dict List is empty\n");
    }else{
        printf("\t\tPrinting Dictionary:\n");
        while(current != NULL){
            printf("Function: %s\tReturn Type: %s\n",current->name, current->return_type);
            current = current->next;
        }
    }
}

void restructure_list(){
    if (head != NULL){
        Node * current = head;
        char cur_src[50] = "";
        while (current != NULL){
            if (strcmp(current->src_idx, "") != 0){ // If the source value is not empty copy the src
                strcpy(cur_src, current->src_idx);
            }else if(strcmp(current->dest_idx, "") != 0 && is_external(current->dest_idx) == 0){
                add_node(cur_src, current->dest_idx, current->edge_type);
                if(is_void(current->dest_idx) == 0){
                    D_Node * dict_current = dict_head;
                    while(dict_current != NULL){
                        if(strcmp(dict_current->name, current->dest_idx) == 0){
                            
                            add_node(current->dest_idx,cur_src,dict_current->return_type);
                            break;
                        }
                        dict_current = dict_current->next;
                    }
                    
                }
            }
            current = current->next;
        }
        clean_list();
    }
}

void delete_list(){
    Node* x = head;
    while(x != NULL){
        head = head->next;
        free(x);
        x = head;
    }
    head = NULL;
}

void print_list(){
    Node * current = head;
    if(current == NULL){
        printf("List is empty\n");
    }else{
        while(current != NULL){
            printf("src: %s\tdest: %s\ttype: %s\n",current->src_idx,current->dest_idx, current->edge_type);
            current = current->next;
        }
    }
}

void write_list(){
    FILE *fpt;
    fpt = fopen("edgelist.csv", "w");
    Node * current = head;
    fprintf(fpt,"source, dest, label\n");
    while(current != NULL){
        fprintf(fpt,"%s,%s,%s\n",current->src_idx,current->dest_idx, current->edge_type);
        current = current->next;
    }
    fclose(fpt);
    printf("Successfully Wrote to edgelist.csv!\n");
}

void dict_add_node(const char* x, const char* ret_type){
    D_Node * new_node = NULL;
    new_node = (D_Node *) malloc(sizeof(D_Node));
    
    strcpy(new_node->name,x);
    if(strcmp(x, "") != 0){
        char * token;
        token = strtok(new_node->name, "(");
        strcpy(new_node->name,token);
    }
    strcpy(new_node->return_type,ret_type);

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

void find_dependencies(char* filename){
    FILE *fpt;
    fpt = fopen(filename, "r");
    char file_line[50];
    char str[50];
    char *token;
    D_Node * current = dict_head;
    while (fgets(file_line, 50, fpt) != NULL) {
        strcpy(str,file_line);
        if(strstr(str, "#include") != NULL){
            token = strtok(str, "\"");
            if(strcmp(token,file_line) != 0){
                token = strtok(NULL, "\"");
                dict_add_node(token, "file");
            }
        }
    }

    fclose(fpt);
}

void delete_dict_list(){
    D_Node* x = dict_head;
    while(x != NULL){
        dict_head = dict_head->next;
        free(x);
        x = dict_head;
    }
    dict_head = NULL;
}

