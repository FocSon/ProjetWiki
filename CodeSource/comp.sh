proc code=ANSI_C main.pc
gcc -o Wiki main.c -L$ORACLE_HOME/lib -lclntsh -I$ORACLE_HOME/sdk/include
