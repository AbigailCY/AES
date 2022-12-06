.SUFFIXES: .cu .o
CXX = nvcc
ARCH  = sm_61
CXXFLAGS = -std=c++11 -arch=$(ARCH) -O3
EXEC = AES_CUDA
OBJS = Table.o Helper.o AES.o main.o
${EXEC}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJS}
.cu.o: # Abstract rule
	${CXX} ${CXXFLAGS} -c $<
# Still need to list the dependencies for object files
Table.o: Table.cu Table.h
Helper.o: Helper.cu Helper.h AES.h
AES.o: AES.cu AES.h Helper.h Table.h
main.o: main.cu AES.h Helper.h timer.h
clean:
	rm -f ${EXEC} ${OBJS}
