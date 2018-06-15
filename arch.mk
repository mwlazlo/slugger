PLATFORM:=$(shell uname -s)
ADDRESS_MODEL:=$(shell file /bin/ls | awk '{print $$3}' | sed 's/-bit//')

ifeq ($(PLATFORM),SunOS)
ADDRESS_MODEL=64
endif

ifeq ($(PLATFORM),SunOS)
LDLIBS += -lnsl -lsocket -lresolv -lrt
DEP_CFLAGS=-pthreads

ifneq ($(CXX),CC)
# gcc on solaris
CPPFLAGS+=-pthreads 

# GNU ld
LD_ALTEXEC=/usr/sfw/bin/gld
endif

else
# compiling on linux
LDLIBS += -pthread
endif
