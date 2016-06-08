
#include "Declarative.h"

using namespace std;

int main(int argc, char* argv[]) {
    
    char* buffer = new char[1024];

    SCOPE_EXIT { delete[] buffer; };

}
