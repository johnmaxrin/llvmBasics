#include "./src/pacellvm.h"

int main(int argc, char *argv[]){
    Pacellvm pacellvm;
    pacellvm.run("print 1;");
    return 0;
}