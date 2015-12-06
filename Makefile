CC			= gcc
Executable	= project3
Libraries	= -L/usr/local/lib/ -L/usr/local/mysql/ -lgmp -lmysqlclient -lz -lm
Flags		= -c -Wall -I/usr/local/mysql/include/ -I/usr/include/mysql/
Sources		= main.c functions.c paillier.c
Objects		= $(Sources:.c=.o)

$(Executable): $(Objects)
	$(CC) $(Objects) $(Libraries) -o $@

.c.o:
	$(CC) $(Flags) $<

SUM_HE:
	$(CC) $(Flags) -fPIC SUM_HE.c
	$(CC) SUM_HE.o $(Libraries) -shared -o SUM_HE.so

run: $(Executable)
	./$(Executable)

clean: 
	rm -f SUM_HE.o SUM_HE.so $(Objects) $(Executable)