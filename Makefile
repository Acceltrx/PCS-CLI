CC = gcc
RC = windres
CFLAGS = -Iinclude
SRC = src/*.c
RES_SRC = assets/resources.rc
RES_OBJ = assets/resources.res
OUT = build/Project_PCS

all: $(RES_OBJ)
	$(CC) $(SRC) $(RES_OBJ) $(CFLAGS) -o $(OUT)

$(RES_OBJ): $(RES_SRC)
	$(RC) -I include $(RES_SRC) -O coff -o $(RES_OBJ)

run: all
	./$(OUT)

clean:
	del $(OUT).exe $(RES_OBJ)