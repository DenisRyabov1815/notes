
PROJECT_NAME = notes

C_COMPILER = g++
C_COMPILER_FLAGS = -Wall -Wextra
C_FILE_EXTENSION = cpp

SRC_PATH = ./src
OBJ_PATH = ./obj
BIN_PATH = ./bin

SRC_C = $(wildcard $(SRC_PATH)/*.$(C_FILE_EXTENSION))

OBJ_C = $(SRC_C:$(SRC_PATH)/%.$(C_FILE_EXTENSION)=$(OBJ_PATH)/%.o)

TARGET = $(BIN_PATH)/$(PROJECT_NAME)

all : $(TARGET)

$(TARGET) : $(OBJ_C) $(OBJ_ASM)
	$(C_COMPILER) $(C_COMPILER_FLAGS) $(OBJ_C) $(OBJ_ASM) -o $(TARGET)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.$(C_FILE_EXTENSION)
	$(C_COMPILER) $(C_COMPILER_FLAGS) -c $< -o $@

run :
	@$(TARGET)

clean :
	rm -vrf $(OBJ_PATH)/* $(TARGET)

install : all
	sudo mv $(TARGET) /bin/

uninstall :
	sudo rm -f /bin/$(PROJECT_NAME)
