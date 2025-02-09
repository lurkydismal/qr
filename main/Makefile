SRCDIR                = .
SUBDIRS               =
DLLS                  =
LIBS                  =
EXES                  = libmain.a



### Common settings

CEXTRA                = $(C_FLAGS)
CXXEXTRA              =
RCEXTRA               =
DEFINES               = $(DEFINES)
INCLUDE_PATH          = -I callbacks/include \
			-I ../stdfunc/include \
			-I ../player/include \
			-I ../logic/include
DLL_PATH              =
DLL_IMPORTS           =
LIBRARY_PATH          =
LIBRARIES             =


### libmain.a sources and settings

libmain_a_MODULE       = libmain.a
libmain_a_C_SRCS       = src/main.c \
			 callbacks/src/callbacks.c
libmain_a_CXX_SRCS     =
libmain_a_RC_SRCS      =
libmain_a_LDFLAGS      =
libmain_a_ARFLAGS      = rc
libmain_a_DLL_PATH     =
libmain_a_DLLS         =
libmain_a_LIBRARY_PATH =
libmain_a_LIBRARIES    =

libmain_a_OBJS         = $(libmain_a_C_SRCS:.c=.o) \
			$(libmain_a_CXX_SRCS:.cpp=.o) \
			$(libmain_a_RC_SRCS:.rc=.res)



### Global source lists

C_SRCS                = $(libmain_a_C_SRCS)
CXX_SRCS              = $(libmain_a_CXX_SRCS)
RC_SRCS               = $(libmain_a_RC_SRCS)


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

$(libmain_a_MODULE): $(libmain_a_OBJS)
	$(AR) $(libmain_a_ARFLAGS) $@ $(libmain_a_OBJS)


