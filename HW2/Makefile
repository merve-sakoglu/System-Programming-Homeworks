# C Compiler
CC=gcc

# main file
MAIN=grepFromDirectory.c

#executable name
EXECUTABLE_NAME=grepFromDirectory

# library source codes
LIBRARY_SOURCES=

# library object files
LIBRARY_OBJECTS=

# cc parameters for release mod (compatible with gcc at least)
RELEASE_PARAMS=-ansi -O2

# cc parameters for debug mode (compatible with gcc at least)
DEBUG_PARAMS=-ansi -DDEBUG

# cc paramters for general use
GENERAL_PARAMS=-Wall

#Some other variables (compatible with Linux at least)
CLEAR=clear
REMOVE=rm
REMOVE_PARAMS=-rf

all:
#	@echo [CC] ${LIBRARY_SOURCES}
#	@${CC} -c ${LIBRARY_SOURCES}
	@echo [CC] ${MAIN}
	@${CC} ${GENERAL_PARAMS} ${MAIN} ${LIBRARY_OBJECTS} -o ${EXECUTABLE_NAME}

clean:
	@echo Removing Object Files...
	@${REMOVE} ${REMOVE_PARAMS} *.o
	@echo Removing Executable Files...
	@${REMOVE} ${REMOVE_PARAMS} ${EXECUTABLE_NAME}
	@echo Removing some other extras...
	@${REMOVE} ${REMOVE_PARAMS} *~

debug:
#	@echo [CC] ${LIBRARY_SOURCES}
#	@${CC} -c ${DEBUG_PARAMS} ${LIBRARY_SOURCES}
	@echo [CC] ${MAIN}
	@${CC} ${DEBUG_PARAMS} ${GENERAL_PARAMS} ${MAIN} ${LIBRARY_OBJECTS} -o ${EXECUTABLE_NAME}
	
release:
#	@echo [CC] ${LIBRARY_SOURCES}
#	@${CC} ${RELEASE_PARAMS} -c ${LIBRARY_SOURCES}
	@echo [CC] ${MAIN}
	@${CC} ${RELEASE_PARAMS} ${GENERAL_PARAMS} ${MAIN} ${LIBRARY_OBJECTS} -o ${EXECUTABLE_NAME}

