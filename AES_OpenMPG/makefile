.SUFFIXES: .cpp .o
CXX = g++
CXXFLAGS = -std=c++11 -Ofast -fopenmp -foffload==nvptx-none="-Ofast -lm -latomic -misa=sm_35"
EXEC = AES_OMPG
OBJS = Table.o Helper.o AES.o main.o
${EXEC}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJS}
.cu.o: # Abstract rule
	${CXX} ${CXXFLAGS} -c $<
# Still need to list the dependencies for object files
Table.o: Table.cpp Table.h
Helper.o: Helper.cpp Helper.h AES.h
AES.o: AES.cpp AES.h Helper.h Table.h
main.o: main.cpp AES.h Helper.h
clean:
	rm -f ${EXEC} ${OBJS}
