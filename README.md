# DeclarativeControlFlowCPP
Header file for Declarative control flow in c++ adapted from Andrei Alexandrescu's talk at CppCon 2015 named "Declarative Control Flow". Check out the talk here https://www.youtube.com/watch?v=WjTrfoiB0MQ it's straight fire and Andrei is a god amongst mortal men

The beauty of this is that you don't need 3*10^343 nested try catch blocks to handle control flow which depends on whether an exception is thrown or not
For an example of where this may be useful check the example in Andrei's talk

USAGE example

  void example(const char* file) {
      
    	FILE* f = fopen(file, "r");
    	char* buffer = new char[2048];
    	
    	//do something ...
    
    	SCOPE_EXIT { //Will be run regardless of the exit context (even on an exception being thrown, so use this to deallocate resources
    		  cout << "Scope is exiting, deallocating resources..." << endl;
    		  fclose(f); 
    		  delete[] buffer;
    	}; 
    	SCOPE_FAIL { //Will only run when an exception causes the program to leave the scope
    		  cout << "Scope exiting because of an uncaught exception..." << endl; 
    	};
    	SCOPE_SUCCESS { //Will only run when the scope exits without an exception
    	  	cout << "HURRAY!!!!" << endl; 
      };
  }
