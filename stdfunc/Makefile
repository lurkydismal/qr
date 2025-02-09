SRCDIR                = .
SUBDIRS               =
DLLS                  =
LIBS                  =
EXES                  = libstdfunc.a



### Common settings

CEXTRA                = $(C_FLAGS)
CXXEXTRA              =
RCEXTRA               =
DEFINES               = $(DEFINES)
INCLUDE_PATH          = -I include
DLL_PATH              =
DLL_IMPORTS           =
LIBRARY_PATH          =
LIBRARIES             =


### libstdfunc.a sources and settings

libstdfunc_a_MODULE       = libstdfunc.a
libstdfunc_a_C_SRCS       = src/stdfunc.c
libstdfunc_a_CXX_SRCS     =
libstdfunc_a_RC_SRCS      =
libstdfunc_a_LDFLAGS      =
libstdfunc_a_ARFLAGS      = rc
libstdfunc_a_DLL_PATH     =
libstdfunc_a_DLLS         =
libstdfunc_a_LIBRARY_PATH =
libstdfunc_a_LIBRARIES    =

libstdfunc_a_OBJS         = $(libstdfunc_a_C_SRCS:.c=.o) \
			$(libstdfunc_a_CXX_SRCS:.cpp=.o) \
			$(libstdfunc_a_RC_SRCS:.rc=.res)



### Global source lists

C_SRCS                = $(libstdfunc_a_C_SRCS)
CXX_SRCS              = $(libstdfunc_a_CXX_SRCS)
RC_SRCS               = $(libstdfunc_a_RC_SRCS)


### Tools

CC = ccache gcc
CXX = ccache g++
RC = rcc
AR = gcc-ar


### Generic targets

all: $(SUBDIRS) $(DLLS:%=%.so) $(LIBS) $(EXES)

### Build rules

.PHONY: all clean dummy

$(SUBDIRS): dummy
	@cd $@ && $(MAKE)

# Implicit rules

.SUFFIXES: .cpp .cxx .rc .res
DEFINCL = $(INCLUDE_PATH) $(DEFINES) $(OPTIONS)

.c.o:
	$(CC) -c $(CFLAGS) $(CEXTRA) $(DEFINCL) -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.rc.res:
	$(RC) $(RCFLAGS) $(RCEXTRA) $(DEFINCL) -fo$@ $<

# Rules for cleaning

CLEAN_FILES     = y.tab.c y.tab.h lex.yy.c core *.orig *.rej \
                  \\\#*\\\# *~ *% .\\\#*

clean:: $(SUBDIRS:%=%/__clean__) $(EXTRASUBDIRS:%=%/__clean__)
	$(RM) $(CLEAN_FILES) $(RC_SRCS:.rc=.res) $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)
	$(RM) $(DLLS:%=%.so) $(LIBS) $(EXES) $(EXES:%=%.so)

$(SUBDIRS:%=%/__clean__): dummy
	cd `dirname $@` && $(MAKE) clean

$(EXTRASUBDIRS:%=%/__clean__): dummy
	-cd `dirname $@` && $(RM) $(CLEAN_FILES)

### Target specific build rules
DEFLIB = $(LIBRARY_PATH) $(LIBRARIES) $(DLL_PATH) $(DLL_IMPORTS:%=-l%)

$(libstdfunc_a_MODULE): $(libstdfunc_a_OBJS)
	$(AR) $(libstdfunc_a_ARFLAGS) $@ $(libstdfunc_a_OBJS)


