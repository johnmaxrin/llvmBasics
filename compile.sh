# Compile Lex and Yacc
flex include/parser/pacelang.l
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile lexer"
    exit 1
fi

bison -d include/parser/pacelang.y
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile parser"
    exit 1
fi

clang++ -c `llvm-config --cxxflags --ldflags --system-libs --libs core` lex.yy.c pacelang.tab.c -I ./ast
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile generated C++ files"
    exit 1
fi

echo "Done Compiling Lex and Yacc"

# Compile the main program
clang++ -c `llvm-config --cxxflags --ldflags --system-libs --libs core` pacelang.cpp  
if [ $? -ne 0 ]; then
    echo "Error: Failed to compile main program"
    exit 1
fi

echo "Done Compiling Main"

# Link and create the executable
clang++ -o pacelang `llvm-config --cxxflags --ldflags --system-libs --libs core` lex.yy.o pacelang.tab.o pacelang.o 
if [ $? -ne 0 ]; then
    echo "Error: Failed to link and create executable"
    exit 1
fi

# Execute the program
./pacelang 4+5
if [ $? -ne 0 ]; then
    echo "Error: Failed to execute pacelang"
    exit 1
fi

# Execute generated IR with lli
lli ./pacelang.ll
if [ $? -ne 0 ]; then
    echo "Error: Failed to execute generated IR"
    exit 1
fi

# Print results
result=$?
echo "Execution result: $result"
printf "\n"
