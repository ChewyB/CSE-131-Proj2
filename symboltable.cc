#include "symboltable.h"
#include "errors.h"


ScopeIter Scope::ScopeLookUp(Data &d){
	//printf("I'm in ScopeLookUp \n");
	ScopeIter it = ScopeObjs.find(d.name);

	if (it != ScopeObjs.end())
		return it; //This means it got found and we need to delete this

	return ScopeObjs.end(); //This means it didn't get found
}

void Scope::Insert(Data &d) {
	ScopeIter prev = ScopeLookUp(d); 

	if( prev != ScopeObjs.end()/*(*prev).first == d.name*/) {
		ReportError::DeclConflict(d.decl, (prev->second).decl );
		ScopeObjs.erase(prev);
	}
	
	
	ScopeObjs.insert(make_pair(d.name, d));
	d.valid = true;

	//cout << "\n Inserting my dick into: " << d.name << "\n";
	
}

void Scope::Size() {
	//printf("The number of objects in this Scope is %d \n", (int)ScopeObjs.size());
}

/*********************  SYMBOLTABLE **********************************/
// Data SymbolTable::TableLookUp(Data &d){
// 	 SymTableIter startIter = tableStack.rbegin();
// 	 while (startIter != tableStack.rend()) {
// 	 	printf("Looping\n");
// 	 	//Scope* currScope = (Scope*)startIter;
// 	 	ScopeIter dataInScope = (*startIter)->ScopeLookUp(d);
	 	

// 	 	if ((*dataInScope).first == (d.name)) { //This will check if we returned the "new" data we passed in which should have valid set to false
// 	 														//OR if tmpData.valid is true, then that means this returned the data inside of the Scope and we  
// 	 														//return that data "tmpData"
// 	 		printf("The key was found in the scope, can't insert\n");
// 	 		return (*dataInScope).second;
// 	 	}
// 	 	startIter++;
// 	 printf("The key was not found in the ScopeTable\n");
// 	}
// 	return d;
// }


Data SymbolTable::TableLookUp(Data &d){
	 SymTableIter startIter = tableStack.rbegin();
	 while (startIter != tableStack.rend()) {
	 	//printf("Looping\n");
	 	//Scope* currScope = (Scope*)startIter;
	 	ScopeIter dataInScope = (*startIter)->ScopeLookUp(d);
	 	
	 	if (dataInScope != ((*startIter)->ScopeObjs).end()) { 
	 		//printf("The key was found in the scope, can't insert\n");
	 		return (*dataInScope).second; //Also means that the identifier was declared
	 	}
	 	startIter++;
	 //printf("The key was not found in the ScopeTable\n");
	}

	//ReportError::IdentifierNotDeclared((d.decl)->getIdentifier(), LookingForVariable);
	return d;
}


void SymbolTable::Push(Scope *s){
	
	tableStack.push_back(s);
}

void SymbolTable::Pop() {

	tableStack.pop_back();	
}

void SymbolTable::Size() {
	//printf("The size of the Symbol Table is %d \n", (int)(tableStack.size()));
}