OBJ:=$(patsubst %.c, %.o, $(wildcard *.c))

calc.exe : $(OBJ)
	gcc -o calc.exe $(OBJ)

clean:
	rm *.exe *.o
