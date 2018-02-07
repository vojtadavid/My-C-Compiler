# My-C-Compiler
Muj pokus o velmi jednoduchy C kompilator. Pro lexikalni analyzu pouziva FLEX. Pro syntaktickou analyzu se pouziva metoda rekurzivního sestupu. Potom se vygeneruje assembler x86_64.
```bash
mkdir build && cd build
cmake ..
make
./mcc ../C_code_examples/sort.c 
./a.out
```
