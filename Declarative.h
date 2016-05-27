
//////////////////////////////////////////////////////////////////////////
/*

	TAKEN FROM ANDREI ALEXANDRESCU'S TALK, "Declarative Control Flow" at CPPCON 2015
	FULL TALK CAN BE FOUND AT https://www.youtube.com/watch?v=WjTrfoiB0MQ
	DECLARATIVE CONTROL FLOW

*/
//////////////////////////////////////////////////////////////////////////


//INCLUDE GUARD
#ifndef ___DECLARITIVE_____H
#define ___DECLARITIVE_____H

//Concatenate two preprocessor symbols
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

//Create a variable with a unique name using __COUNTER__ or __LINE__ preprocessor symbols
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) \
	CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
	CONCATENATE(str, __LINE__)
#endif

using namespace std;


namespace declarative {

	//Enumeration of Scope Exit states
	enum class ScopeExitState { Exit, Fail, Success };

	//Scope guard class, the template takes a callable object with no parameters
	template <typename Fun>
	class ScopeGuard {
		Fun f_;
		ScopeExitState ex;
	public:
		//Constructed with a ScopeExitState and function
		ScopeGuard(ScopeExitState ex, Fun fun)
			: f_(fun), ex(ex) {}

		//Scope exiting because the object is being destroyed
		~ScopeGuard() {
			switch (ex) {
			case ScopeExitState::Exit:
				f_();
				break;
			case ScopeExitState::Fail:
				if (std::uncaught_exception()) //If the scope is exiting because of an exception
					f_();
				break;
			case ScopeExitState::Success:
				if (!std::uncaught_exception()) //If the scope is exiting without an exception
					f_();
				break;
			}
		}
	};

	//Defines addition operator of ScopeExitState enum and Fun&& which returns a ScopeGuard to protect the scope
	template<typename Fun>
	ScopeGuard<Fun>
		operator +(ScopeExitState ex, Fun&& fn) {
		return ScopeGuard<Fun>(ex, forward<Fun>(fn));
	}
}

//THE FUN HAPPENS HERE

//Use the addition operator defined in namespace "declarative" to create a convenient macro 
//to generate a scope guard protecting a lambda with a global scope capture.
#define SCOPE_EXIT \
	auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) \
	= ::declarative::ScopeGuardOnExit::Exit + [&]()

#define SCOPE_FAIL \
	auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) \
	= ::declarative::ScopeGuardOnExit::Fail + [&]()

#define SCOPE_SUCCESS \
	auto ANONYMOUS_VARIABLE(SCOPE_SUCCESS_STATE) \
	= ::declarative::ScopeGuardOnExit::Success + [&]()



/*

USAGE example

void example(const char* file) {
	FILE* f = fopen(file, "r");

	SCOPE_EXIT { fclose(f); };
	SCOPE_FAIL { cout << "Scope exiting because of an uncaught exception..." << endl; };
	SCOPE_SUCCESS { cout << "HURRAY!!!!" << endl; };

}

*/

#endif
