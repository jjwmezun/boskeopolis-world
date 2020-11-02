COMPILER = gcc
LINKER = gcc
WIN_COMPILER = x86_64-w64-mingw32-gcc
WIN_LINKER = x86_64-w64-mingw32-gcc
EXT = c
CFLAGS = -g3 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wfloat-equal -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wmain -Wfatal-errors -Wextra -Wall -Wno-switch -Wno-unused-parameter -Wno-float-equal

LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lm
ABS_INC = -Iinc/
LOCAL_INC = -Iinc/ $(patsubst %,-I%,$(filter %/,$(wildcard $(SRC_DIR)*/)))
INC =  $(ABS_INC) $(LOCAL_INC)

SRC_DIR = src/
OBJ_DIR = obj/

SOURCES = $(wildcard $(SRC_DIR)*.$(EXT)) $(wildcard $(SRC_DIR)**/*.$(EXT))
OBJ = $(subst $(SRC_DIR),$(OBJ_DIR),$(subst .$(EXT),.o,$(SOURCES)))

EXE_DIR = bin/
EXE = $(EXE_DIR)Boskeopolis\ World

OBJ_FOLDERS = $(EXE_DIR) $(OBJ_DIR) $(subst -I$(SRC_DIR),$(OBJ_DIR),$(LOCAL_INC))

#################################################

all: before out

win: before winout

before:
	$(foreach f,$(OBJ_FOLDERS),$(mkdir -p f))

out: $(OBJ)
	$(LINKER) -o $(EXE) $(OBJ) $(LDFLAGS)

$(OBJ): $(OBJ_DIR)%.o : $(SRC_DIR)%.$(EXT)
	$(COMPILER) $(CFLAGS) $(INC) -c $< -o $@

debug:
	echo $(OBJ_FOLDERS)

.PHONY: clean
.SILENT: *.o out before

clean:
	rm -f $(OBJ_DIR)*.o $(OBJ_DIR)**/*.o $(EXE)
