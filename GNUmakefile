ifndef (CXX)
CXX=g++
endif




#############
#### log ####
#############
LOG_TARGET=libslugger.a
LOG_SRC= \
	slugger/log.cpp \
	slugger/destination.cpp \
	slugger/logstream.cpp \
	slugger/stream/file.cpp \
	slugger/stream/null.cpp \
	slugger/stream/std_err.cpp \
	slugger/stream/std_out.cpp \
	slugger/stream/sys_log.cpp \
	slugger/stream/tee.cpp
LOG_HEADERS= \
	slugger/slugger.h \
	slugger/types.h \
	$(LOG_SRC:.cpp=.h)
	
LOG_OBJ=$(LOG_SRC:.cpp=.o)


##############
#### test ####
##############
TEST_TARGETS=tests/log
TESTS_SRC=tests/log.cpp
TESTS_OBJ=$(TESTS_SRC:.cpp=.o)
LDLIBS_TEST=-lslugger 


################
#### global ####
################
ALL_SRC=$(LOG_SRC) $(TESTS_SRC)
ALL_TARGETS=$(LOG_TARGET)
INSTALL_HEADERS=$(LOG_HEADERS)


# lists of files
SRCDIR=slugger
ALL_OBJ=$(ALL_SRC:.cpp=.o)
ALL_DEPS:=$(shell echo $(ALL_SRC) | sed 's?/\([a-z_]*\).cpp *?/.\1.d ?g')

# building options
CFLAGS_INC= -I. 
ifneq ($(CXX),CC)
CFLAGS_WARN= -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wold-style-cast -Werror 
CFLAGS_DEBUG= -g
CFLAGS_ARCH= -m$(ADDRESS_MODEL) -fPIC
LDFLAGS_ARCH= -fPIC
LDLIBS= -lpthread
else
CFLAGS_WARN= 
CFLAGS_DEBUG= -g
CFLAGS_ARCH= -m$(ADDRESS_MODEL) -Kpic
CFLAGS_STL=-library=stlport4
LDFLAGS_ARCH=-mt -staticlib=%all -Kpic
LDLIBS= 
endif

include arch.mk

CFLAGS=$(CFLAGS_ARCH) $(CFLAGS_WARN) $(CFLAGS_DEBUG) $(CFLAGS_INC) $(CFLAGS_STL)

# linking options
LDFLAGS=-L. $(LDFLAGS_ARCH)

# main dep
all: $(ALL_DEPS) $(ALL_TARGETS) $(TEST_TARGETS)

##############################################################################
# AUTOMATIC DEPENDENCIES
##############################################################################

.%.d: %.cpp
	@echo "Creating dependency: $@"
	@set -e; rm -f $@; \
	 g++ -MM $(CFLAGS_INC) $< | sed -e 's|.*:|$*.o:|' | \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

NODEPS=clean reallyclean
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
        -include $(ALL_DEPS)
endif

##############################################################################
# MAIN TARGET
##############################################################################

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@


install: $(ALL_TARGETS)
	@if [ -z "$(DESTDIR)" ] ; then \
	  echo "make install must specify DESTDIR."; \
	  echo "e.g. gmake DESTDIR=stage install"; \
	  exit 1; \
	fi;
	@mkdir -p $(DESTDIR)/lib
	@mkdir -p $(DESTDIR)/include
	@if tar --help >/dev/null 2>&1 ; then \
	   echo Using GNU tar; \
	   tar cf - $(ALL_TARGETS) | (cd $(DESTDIR)/lib; tar xvf -); \
	   tar cf - $(INSTALL_HEADERS) | (cd $(DESTDIR)/include; tar xvf -); \
	 else \
	   echo Using SUN tar; \
	   tar cpBEf - $(ALL_TARGETS) | (cd $(DESTDIR)/lib; tar xvBEf -); \
	   tar cpBEf - $(INSTALL_HEADERS) | (cd $(DESTDIR)/include; tar xvBEf -); \
	 fi


##############################################################################
# LOG
##############################################################################
.PHONY: log
log: $(LOG_TARGET)

$(LOG_TARGET): $(LOG_OBJ)
	ar -ruv $@ $(LOG_OBJ)


##############################################################################
# TESTS
##############################################################################

$(TEST_TARGETS): $(ALL_TARGETS) $(TESTS_OBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $@.o $(LDLIBS_TEST) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(ALL_OBJ) $(ALL_TARGETS) $(TEST_TARGETS)

.PHONY: reallyclean
reallyclean:
	rm -f $(ALL_DEPS)
	find . -name .*.d* | xargs rm -f

