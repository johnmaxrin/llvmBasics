# Compile
clang++ -o pacelang `llvm-config --cxxflags --ldflags --system-libs --libs core` pacelang.cpp 

# Execute
./pacelang

# Execute generated IR
lli ./pacelang.ll

# Print Results
echo $?
printf "\n"

