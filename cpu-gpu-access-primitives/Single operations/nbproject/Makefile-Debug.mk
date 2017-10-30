#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Column_Store_Import_methods.o \
	${OBJECTDIR}/OpenCLExecutionColumnStore.o \
	${OBJECTDIR}/OpenCLExecutionRowStore.o \
	${OBJECTDIR}/QueryDataGenerator.o \
	${OBJECTDIR}/Row_Store_Data.o \
	${OBJECTDIR}/Row_Store_Import_Methods.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 -I/usr/local/cuda/include -I/usr/local/cuda/bin/nvcc -L/usr/local/cuda/lib64/ -L/usr/lib/x86_64-linux-gnu/ -lcudart -lcuda
CXXFLAGS=-std=c++11 -I/usr/local/cuda/include -I/usr/local/cuda/bin/nvcc -L/usr/local/cuda/lib64/ -L/usr/lib/x86_64-linux-gnu/ -lcudart -lcuda

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lOpenCL

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/single_operations

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/single_operations: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/single_operations ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Column_Store_Import_methods.o: Column_Store_Import_methods.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Column_Store_Import_methods.o Column_Store_Import_methods.cpp

${OBJECTDIR}/OpenCLExecutionColumnStore.o: OpenCLExecutionColumnStore.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OpenCLExecutionColumnStore.o OpenCLExecutionColumnStore.cpp

${OBJECTDIR}/OpenCLExecutionRowStore.o: OpenCLExecutionRowStore.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OpenCLExecutionRowStore.o OpenCLExecutionRowStore.cpp

${OBJECTDIR}/QueryDataGenerator.o: QueryDataGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/QueryDataGenerator.o QueryDataGenerator.cpp

${OBJECTDIR}/Row_Store_Data.o: Row_Store_Data.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Row_Store_Data.o Row_Store_Data.cpp

${OBJECTDIR}/Row_Store_Import_Methods.o: Row_Store_Import_Methods.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Row_Store_Import_Methods.o Row_Store_Import_Methods.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/single_operations

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
