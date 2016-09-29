/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "errors.h"
#include <map>
#include <string>
#include "symboltable.h"
// #include "ast.h"

/********************* GLOBAL VARIABLES *****************************/

 // map<char*, Type*> mapList;

/********************* GLOBAL VARIABLES *****************************/

 Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    printf("\n");
}

void Program::Check() {
    /* pp3: here is where the semantic analyzer is kicked off.
     *      The general idea is perform a tree traversal of the
     *      entire program, examining all constructs for compliance
     *      with the semantic rules.  Each node can have its own way of
     *      checking itself, which makes for a great use of inheritance
     *      and polymorphism in the node classes.
     */
        symbolTable = new SymbolTable();
        Scope *s = new Scope();
        Data *data = new Data();

        if(this->decls->NumElements() > 0){
       // symbolTable->Push();
        for (int i = 0; i < this->decls->NumElements(); i++) {

           Decl *current = decls->Nth(i);
           data->name = (current->getIdentifier())->name;
           data->decl = current;
           s->Insert(*data);
           
           current->Check();
        }

      }
        symbolTable->Push(s);
        symbolTable->Size();
        s->Size();
  }


void Stmt::Check(){
 // printf("im a statement now..\n");


}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

void StmtBlock::Check(List<VarDecl*> *dec){
  Scope *s2 = new Scope();
  symbolTable->Push(s2);

  if (dec != NULL){
    Data *d = new Data();

    ////cout << "\nFormals count :: " << dec->NumElements() << "\n";
    for(int i = 0; i < dec->NumElements(); i++){
      Decl *curr = dec->Nth(i);
      d->name = (curr->getIdentifier())->name;
      d->decl = curr;
      s2->Insert(*d);
     // printf("did i insert while in the stmtblock?");
    }
  }

  Data *stmtDecl = new Data();
  for(int j = 0; j < stmts->NumElements(); j++){

      if(Expr *e = dynamic_cast<Expr*> (stmts->Nth(j))) {
        e->Check();
     } else {
      
      Decl *singleDecl = (dynamic_cast<DeclStmt*>(stmts->Nth(j)))->decl;

      stmtDecl->name = (singleDecl->getIdentifier())->name;
      stmtDecl->decl = singleDecl;

      s2->Insert(*stmtDecl);

      stmts->Nth(j)->Check();
    }
  }


  s2->Size();
  symbolTable->Pop();
}

DeclStmt::DeclStmt(Decl *d) {
    Assert(d != NULL);
    (decl=d)->SetParent(this);
}

void DeclStmt::PrintChildren(int indentLevel) {
    decl->Print(indentLevel+1);
}

void DeclStmt::Check(){
  //cout << "\n I'm a DeclStmt" << "\n";
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

void ConditionalStmt::Check(){
  //cout << "\n I'm a ConditionalStmt" << "\n";
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    Assert(i != NULL && t != NULL && b != NULL);
    (init=i)->SetParent(this);
    step = s;
    if ( s )
      (step=s)->SetParent(this);
}

void ForStmt::PrintChildren(int indentLevel) {
    init->Print(indentLevel+1, "(init) ");
    test->Print(indentLevel+1, "(test) ");
    if ( step )
      step->Print(indentLevel+1, "(step) ");
  body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::Check(){
  //cout << "\n I'm a WhileStmt" << "\n";
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::PrintChildren(int indentLevel) {
    if (test) test->Print(indentLevel+1, "(test) ");
    if (body) body->Print(indentLevel+1, "(then) ");
    if (elseBody) elseBody->Print(indentLevel+1, "(else) ");
}


ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    expr = e;
    if (e != NULL) expr->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
    if ( expr ) 
      expr->Print(indentLevel+1);
}

SwitchLabel::SwitchLabel(Expr *l, Stmt *s) {
    Assert(l != NULL && s != NULL);
    (label=l)->SetParent(this);
    (stmt=s)->SetParent(this);
}

SwitchLabel::SwitchLabel(Stmt *s) {
    Assert(s != NULL);
    label = NULL;
    (stmt=s)->SetParent(this);
}

void SwitchLabel::PrintChildren(int indentLevel) {
    if (label) label->Print(indentLevel+1);
    if (stmt)  stmt->Print(indentLevel+1);
}

SwitchStmt::SwitchStmt(Expr *e, List<Stmt *> *c, Default *d) {
    Assert(e != NULL && c != NULL && c->NumElements() != 0 );
    (expr=e)->SetParent(this);
    (cases=c)->SetParentAll(this);
    def = d;
    if (def) def->SetParent(this);
}

void SwitchStmt::PrintChildren(int indentLevel) {
    if (expr) expr->Print(indentLevel+1);
    if (cases) cases->PrintAll(indentLevel+1);
    if (def) def->Print(indentLevel+1);
}

