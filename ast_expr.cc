/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */

#include <string.h>
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "errors.h"



IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}
void IntConstant::PrintChildren(int indentLevel) { 
    printf("%d", value);
}

FloatConstant::FloatConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}
void FloatConstant::PrintChildren(int indentLevel) { 
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}
void BoolConstant::PrintChildren(int indentLevel) { 
    printf("%s", value ? "true" : "false");
}

VarExpr::VarExpr(yyltype loc, Identifier *ident) : Expr(loc) {
    Assert(ident != NULL);
    this->id = ident;
}

void VarExpr::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
    printf("%s",tokenString);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o) 
  : Expr(Join(l->GetLocation(), o->GetLocation())) {
    Assert(l != NULL && o != NULL);
    (left=l)->SetParent(this);
    (op=o)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) {
   if (left) left->Print(indentLevel+1);
   op->Print(indentLevel+1);
   if (right) right->Print(indentLevel+1);
}
   

Type* ArithmeticExpr::checkType(){
    Type *leftType = left->checkType();
    Type *rightType = right->checkTypeRight();

    if((leftType->IsNumeric()) && !(rightType->IsNumeric())) {
        ReportError::IncompatibleOperands(op, leftType, rightType);
        return Type::errorType;
    }

    else if(!(leftType->IsNumeric()) && (rightType->IsNumeric())) {
      ReportError::IncompatibleOperands(op, leftType, rightType);
        return Type::errorType;
    }

    else if(!(leftType->IsEquivalentTo(rightType))) {
      ReportError::IncompatibleOperands(op, leftType, rightType);
      return Type::errorType;
    }

    else if (leftType == NULL && (rightType->IsNumeric()))
    {
      return rightType;
    }

    else if (leftType == NULL && !(rightType->IsNumeric())){
      ReportError::IncompatibleOperand(op, rightType);
      return Type::errorType;
    }
    
    return leftType;
} 

void ArithmeticExpr::Check(){
  checkType();
} 


  
Type* RelationalExpr::checkType(){
  Type *leftType = left->checkType();
  Type *rightType = right->checkType();

  if((leftType->IsNumeric())
      && !(rightType->IsNumeric())) {
        ReportError::IncompatibleOperands(op, leftType, rightType);
        return Type::errorType;
    }

    else if(!(leftType->IsNumeric())
            && (rightType->IsNumeric())) {
      ReportError::IncompatibleOperands(op, leftType, rightType);
        return Type::errorType;
    }

    else if(!(leftType->IsEquivalentTo(rightType))) {
      ReportError::IncompatibleOperands(op, leftType, rightType);
      return Type::errorType;
    }

  return leftType;
}

void RelationalExpr::Check(){
  checkType();
}

Type* EqualityExpr::checkType(){
  Type *leftType = left->checkType();
  Type *rightType = right->checkType();

  if((leftType->IsNumeric())
      && !(rightType->IsNumeric())) {

      ReportError::IncompatibleOperands(op, leftType, rightType);
      return Type::errorType;
  }

  else if(!(leftType->IsNumeric()) 
          && (rightType->IsNumeric())) {
    
    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;
  }

  else if(leftType->isBool() && !(rightType->isBool())){

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;  
  }

  else if(!leftType->isBool() && (rightType->isBool())){

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;  
  }

  else if(!(leftType->IsEquivalentTo(rightType))) {

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;
  }

return leftType;
}

void EqualityExpr::Check(){
  checkType();
}


Type* AssignExpr::checkType(){

  // Data *leftHS = new Data();
  // Identifier *leftID = (dynamic_cast<VarExpr*>(left))->getIdentifier();

  // leftHS->name = leftID->name;
  // //leftHS->decl = (dynamic_cast<DeclStmt*>(left))->decl;

  // Data tmp = symbolTable->TableLookUp(*leftHS);

  // if(!tmp.valid){
  //   ReportError::IdentifierNotDeclared((tmp.decl)->getIdentifier(), LookingForVariable);
  //   return Type::errorType;
  // }

  Type *leftType = left->checkType();
  Type *rightType = right->checkType();

  if((leftType->IsNumeric()) && !(rightType->IsNumeric())) {

      ReportError::IncompatibleOperands(op, leftType, rightType);
      return Type::errorType;
  }

  else if(!(leftType->IsNumeric()) && (rightType->IsNumeric())) {
    
    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;
  }

  else if((leftType->isBool()) && !(rightType->isBool())){

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;  
  }

  else if(!(leftType->isBool()) && (rightType->isBool())){

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;  
  }

  else if(!(leftType->IsEquivalentTo(rightType))) {

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;
  }

  else if((leftType->IsEquivalentTo(rightType)) ){
   // cout << "\n\n\n\n\n we are the same \n\n\n\n\n";
    return leftType;
  }

  return leftType;

}

Type* AssignExpr::checkTypeRight(){

  Type *leftType = left->checkType(); 
  Type *rightType = right->checkTypeRight();

  if((leftType->IsNumeric()) && !(rightType->IsNumeric())) {

      ReportError::IncompatibleOperands(op, leftType, rightType);
      return Type::errorType;
  }

  else if(!(leftType->IsNumeric()) && (rightType->IsNumeric())) {
    
    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;
  }

  else if((leftType->isBool()) && !(rightType->isBool())){

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;  
  }

  else if(!(leftType->isBool()) && (rightType->isBool())){

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;  
  }

 else if(!(leftType->IsEquivalentTo(rightType))) {

    ReportError::IncompatibleOperands(op, leftType, rightType);
    return Type::errorType;
  }
/*
  else if((leftType->IsEquivalentTo(rightType)) ){
    cout << "\n\n\n\n\n we are the same \n\n\n\n\n";
    return rightType;
  }*/

  return rightType;
}


void AssignExpr::Check(){
  // printf("\n\n\n\nThis is an assignExpr\n\n\n\n"); 


  Data *leftHS = new Data();
  Identifier *leftID = (dynamic_cast<VarExpr*>(left))->getIdentifier();

  leftHS->name = leftID->name;
  //leftHS->decl = (dynamic_cast<DeclStmt*>(left))->decl;

  Data tmp = symbolTable->TableLookUp(*leftHS);

  if(tmp.valid) {
    // printf("We made it!\n");
    //Here is where we get the type
    Decl *leftType = tmp.decl;
    string leftTypeName = (dynamic_cast<VarDecl*>(leftType))->getTypeName();
    string rightTypeName = "";

    if (dynamic_cast<IntConstant*>(right) == right) {
      Type *rightType = ((dynamic_cast<IntConstant*>(right))->getType());
      rightTypeName = rightType->getTypeName();
    }
    else if (dynamic_cast<FloatConstant*>(right) == right) {
      Type *rightType = ((dynamic_cast<FloatConstant*>(right))->getType());
      rightTypeName = rightType->getTypeName();
    }
    else if (dynamic_cast<BoolConstant*>(right) == right) {
      Type *rightType = ((dynamic_cast<BoolConstant*>(right))->getType());
      rightTypeName = rightType->getTypeName();
    }

    if(leftTypeName == rightTypeName) {
      // printf("\n\n\n\n\nFUCK YES MOTHERFUCKER\n\n\n\n\n\n");
    }

  }

  //cout <<"The identifier is " << leftID->name << endl;
  //checkType();
  //checkTypeRight();


  
}

ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

void ArrayAccess::PrintChildren(int indentLevel) {
    base->Print(indentLevel+1);
    subscript->Print(indentLevel+1, "(subscript) ");
  }
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}


  void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
  }

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

 void Call::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    if (field) field->Print(indentLevel+1);
    if (actuals) actuals->PrintAll(indentLevel+1, "(actuals) ");
  }
 

Type* IntConstant::checkType(){
    return Type::intType;
}

Type* FloatConstant::checkType(){
    return Type::floatType;
} 

Type* BoolConstant::checkType(){
    return Type::boolType;
}

Identifier* VarExpr::getIdentifier(){
  return id;
}

 Type* IntConstant::getType() {
    return Type::intType; 
 }

  Type* FloatConstant::getType() {
    return Type::floatType; 
 }

  Type* BoolConstant::getType() {
    return Type::boolType; 
 }

