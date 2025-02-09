SRCDIR                = .
SUBDIRS               =
DLLS                  =
LIBS                  =
EXES                  = liblogic.a



### Common settings

CEXTRA                = $(C_FLAGS)
CXXEXTRA              =
RCEXTRA               =
DEFINES               = $(DEFINES)
INCLUDE_PATH          = -I include \
			-I ../stdfunc/include \
			-I ../player/include
DLL_PATH              =
DLL_IMPORTS           =
LIBRARY_PATH          =
LIBRARIES             =


### liblogic.a sources and settings

liblogic_a_MODULE       = liblogic.a
liblogic_a_C_SRCS       = src/map.c \
			  src/player.c
liblogic_a_CXX_SRCS     =
liblogic_a_RC_SRCS      =
liblogic_a_LDFLAGS      =
liblogic_a_ARFLAGS      = rc
liblogic_a_DLL_PATH     =
liblogic_a_DLLS         =
liblogic_a_LIBRARY_PATH =
liblogic_a_LIBRARIES    =

liblogic_a_OBJS         = $(liblogic_a_C_SRCS:.c=.o) \
			$(liblogic_a_CXX_SRCS:.cpp=.o) \
			$(liblogic_a_RC_SRCS:.rc=.res)



### Global source lists

C_SRCS                = $(liblogic_a_C_SRCS)
CXX_SRCS              = $(liblogic_a_CXX_SRCS)
RC_SRCS               = $(liblogic_a_RC_SRCS)


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

$(liblogic_a_MODULE): $(liblogic_a_OBJS)
	$(AR) $(liblogic_a_ARFLAGS) $@ $(liblogic_a_OBJS)


