.SUFFIXES: .cpp .o
CXX = g++
CXXFLAGS = -std=c++11 -O3
EXEC = AES_SEQ
OBJS = Helper.o Mode.o AES.o main.o
${EXEC}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJS}
.cpp.o: # Abstract rule
	${CXX} ${CXXFLAGS} -c $<
# Still need to list the dependencies for object files
Helper.o: Helper.cpp Helper.hpp AES.hpp
Mode.o: Mode.cpp Helper.hpp Mode.hpp AES.hpp
AES.o: AES.cpp AES.hpp Helper.hpp Table.hpp
main.o: main.cpp AES.hpp Helper.hpp Mode.hpp
clean:
	rm -f ${EXEC} ${OBJS}