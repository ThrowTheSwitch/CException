#Tool and Lib Locations
C_COMPILER=gcc
C_LIBS=C:/MinGW/lib
UNITY_DIR=vendor/unity/src

#Test File To Be Created
OUT_FILE=test_cexceptions
ifeq ($(OS),Windows_NT)
OUT_EXTENSION=.exe
else
OUT_EXTENSION=.out
endif

#Options
SRC_FILES=lib/CException.c test/TestException.c test/TestException_Runner.c $(UNITY_DIR)/unity.c
INC_DIRS=-Ilib -Itest -I$(UNITY_DIR)
LIB_DIRS=-L$(C_LIBS) 
SYMBOLS=-DTEST -DCEXCEPTION_USE_CONFIG_FILE

#Default Task: Compile And Run Tests
default:
	$(C_COMPILER) $(INC_DIRS) $(LIB_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(OUT_FILE)$(OUT_EXTENSION)
	$(OUT_FILE)$(OUT_EXTENSION)
	