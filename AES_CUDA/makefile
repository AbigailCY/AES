# #===============================================================================
# # User Options
# #===============================================================================

# # Compiler can be set below, or via environment variable
# CC        = nvcc
# OPTIMIZE  = yes
# ARCH      = sm_61

# #===============================================================================
# # Program name & source code list
# #===============================================================================

# program = main

# source = main.cu

# obj = $(source:.cu=.o)

# #===============================================================================
# # Sets Flags
# #===============================================================================

# # Standard Flags
# CFLAGS := $(EXTRA_CFLAGS) -std=c++11 -Xcompiler -Wall -arch=$(ARCH)

# # Linker Flags
# LDFLAGS = 


# # Optimization Flags
# ifeq ($(OPTIMIZE),yes)
#   CFLAGS += -O3
# endif

# #===============================================================================
# # Targets to Build
# #===============================================================================

# $(program): $(obj)
# 	$(CC) $(CFLAGS) $(obj) -o $@ $(LDFLAGS)

# $(obj): main.cu AES.cu AES.h Helper.cu Helper.h Table.cu Table.h timer.h
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -rf $(program) $(obj)



.SUFFIXES: .cu .o
CXX = nvcc
ARCH  = sm_61
CXXFLAGS = -std=c++11 -Xcompiler -Wall -arch=$(ARCH) -O3
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
