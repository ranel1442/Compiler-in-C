CC = gcc
CFLAGS = -g -ansi -Wall -pedantic  # הוספתי את הדגל -g כאן

# רשימת קבצי המקור והקבצים האובייקטיביים
OBJS = main.o utilities.o memory.o pre_first_pass.o instruction_line.o preProsesor.o pass_one.o instruction_line_second_pass.o pass_two.o

# יצירת הקובץ הבינארי
mycalc: $(OBJS)
	$(CC) $(CFLAGS) -o mycalc $(OBJS) -lm

# כללים לקימפול כל אחד מקבצי המקור
main.o: main.c pre_first_pass.h instruction_line.h pass_two.h
	$(CC) $(CFLAGS) -c main.c

utilities.o: utilities.c utilities.h
	$(CC) $(CFLAGS) -c utilities.c

memory.o: memory.c memory.h
	$(CC) $(CFLAGS) -c memory.c

pre_first_pass.o: pre_first_pass.c pre_first_pass.h
	$(CC) $(CFLAGS) -c pre_first_pass.c

instruction_line.o: instruction_line.c instruction_line.h
	$(CC) $(CFLAGS) -c instruction_line.c

preProsesor.o: preProsesor.c preProsesor.h
	$(CC) $(CFLAGS) -c preProsesor.c

pass_one.o: pass_one.c pass_one.h
	$(CC) $(CFLAGS) -c pass_one.c

instruction_line_second_pass.o: instruction_line_second_pass.c instruction_line_second_pass.h
	$(CC) $(CFLAGS) -c instruction_line_second_pass.c

pass_two.o: pass_two.c pass_two.h
	$(CC) $(CFLAGS) -c pass_two.c

# ניקוי קבצים בינאריים וקבצים אובייקטיביים
clean:
	rm -f mycalc $(OBJS)

