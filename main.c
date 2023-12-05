#include "linked-list.h"

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {

    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0) {
        return CXChildVisit_Continue;
    }
    
    CXCursorKind cursor_kind = clang_getCursorKind(cursor);
    CXString cursor_display_name = clang_getCursorDisplayName(cursor);

    if (cursor_kind == CXCursor_FunctionDecl) {  // FUNCTION DECLERATION
        //printf("Function Decl: %s\tReturn Type: %s\n",
        //    clang_getCString(cursor_display_name),
        //    clang_getCString(clang_getTypeSpelling(clang_getResultType(clang_getCursorType(cursor)))));

        add_node(clang_getCString(cursor_display_name), "", clang_getCString(clang_getCursorKindSpelling(cursor_kind)));
        dict_add_node(clang_getCString(cursor_display_name), clang_getCString(clang_getTypeSpelling(clang_getResultType(clang_getCursorType(cursor)))));
    } else if(cursor_kind == CXCursor_CallExpr) {  // FUNCTION CALL
        //printf("Function call: %s\n", clang_getCString(cursor_display_name));
        
        add_node("", clang_getCString(cursor_display_name), clang_getCString(clang_getCursorKindSpelling(cursor_kind)));
    }
    //printf("Name: %s\t Kind: %s\treturn Type: %s\n",
    //   clang_getCString(cursor_display_name),
    //   clang_getCString(clang_getCursorKindSpelling(cursor_kind)),
    //   clang_getCString(clang_getTypeSpelling(clang_getResultType(clang_getCursorType(cursor)))));

    clang_disposeString(cursor_display_name);

    return CXChildVisit_Recurse;
    //return CXChildVisit_Continue;
}

void visit_children(CXCursor parent, CXTranslationUnit tu) {
    clang_visitChildren(parent, visitor, NULL);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please provide a C source code file.\n");
        return 1;
    }
    find_dependencies(argv[1]);
    print_dict_list();

    CXIndex index;
    CXTranslationUnit tu;
    CXCursor cursor;
    D_Node *cur = dict_head;
    while(cur != NULL){
        index = clang_createIndex(0, 0);
        tu = clang_parseTranslationUnit(index, cur->name, nullptr, 0, nullptr, 0, CXTranslationUnit_None);
        cursor = clang_getTranslationUnitCursor(tu);
        visit_children(cursor, tu);
        cur = cur->next;
    }

    printf("End of header file analysis\n");
    index = clang_createIndex(0, 0);
    tu = clang_parseTranslationUnit(index, argv[1], nullptr, 0, nullptr, 0, CXTranslationUnit_None);
    if (tu == nullptr) {
        printf("Unable to parse translation unit. Quitting.\n");
        return 1;
    }
    cursor = clang_getTranslationUnitCursor(tu);
    visit_children(cursor, tu);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    
    restructure_list();
    print_dict_list();
    print_list();
    write_list();
    delete_list();

    return 0;
}
