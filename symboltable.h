#ifndef _H_symboltable
#define _H_symboltable

#include <iostream>
#include <vector>
#include <string.h>
#include <map>
#include <iterator>
#include <stdlib.h>   // for NULL
#include <stdio.h>


#include "ast_decl.h"
#include "ast_stmt.h"
#include "list.h"
#include "error.h"

class VarDecl;
class Decl;

using namespace std;

    enum typeDecl{
       varDecls,
       funcDecls
    };

    struct Data{
        string name;
        Decl *decl;
        typeDecl types;
        int extraInfo;
        bool valid;

        Data(): name(""), decl(NULL), types(varDecls), extraInfo(0), valid(false){}
    };


    typedef map<string, Data>::iterator ScopeIter;
             

    class Scope {
        
        protected:
            
         

        public:
            map <string, Data> ScopeObjs;
            Scope(){}
            ~Scope(){}

            void Insert(Data &d);
            ScopeIter ScopeLookUp(Data &d);
            void Delete(Data &d);
            void Size();
        
    };

    typedef vector<Scope*>::reverse_iterator SymTableIter;  

    class SymbolTable{

        private:
             

        public: 
            vector< Scope* > tableStack;
            SymbolTable(){}
            ~SymbolTable(){}
            void Push(Scope* s);
            void Pop();
            Data TableLookUp(Data &d);
            void Size();
    };

     
#endif