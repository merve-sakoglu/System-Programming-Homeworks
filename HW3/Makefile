CC=gcc


MAIN=grepFromDirParalel.c


EXECUTABLE_NAME=grepFromDirParalel


LIBRARY_SOURCES=


LIBRARY_OBJECTS=


RELEASE_PARAMS=-ansi -O2


MATH = lm

THREAD = lpthread

CLEAR=clear
REMOVE=rm
REMOVE_PARAMS=-rf

all:

	@echo [CC] ${MAIN}
	@${CC} ${GENERAL_PARAMS} ${MAIN} ${LIBRARY_OBJECTS} -o ${EXECUTABLE_NAME} -${MATH} -${THREAD}

clean:
	@echo Removing Object Files...
	@${REMOVE} ${REMOVE_PARAMS} *.o
	@echo Removing Executable Files...
	@${REMOVE} ${REMOVE_PARAMS} ${EXECUTABLE_NAME}
	@echo Removing some other extras...
	@${REMOVE} ${REMOVE_PARAMS} *~


release:

	@echo [CC] ${MAIN}
	@${CC} ${RELEASE_PARAMS} ${GENERAL_PARAMS} ${MAIN} ${LIBRARY_OBJECTS} -o ${EXECUTABLE_NAME}

