## Instructions counter.
## Year: 2020
## Author: Manel Lurbe Sempere <malursem@gap.upv.es>

TOPDIR  := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)/..

include $(TOPDIR)/config.mk
include $(TOPDIR)/rules.mk

DIRS=

ifeq ($(ARCH),ia64)
#DIRS +=ia64
endif

ifeq ($(ARCH),x86_64)
DIRS += x86
endif

ifeq ($(ARCH),i386)
DIRS += x86
endif

CFLAGS+= -I. -D_GNU_SOURCE -pthread

PERF_EVENT_HDR=$(TOPDIR)/include/perfmon/pfmlib_perf_event.h

LPC_UTILS=perf_util.o
LPC_UTILS_HDR=perf_util.h

TARGETS=Instructions_counter

EXAMPLESDIR=$(DESTDIR)$(DOCDIR)/perf_examples

all: $(TARGETS)
	@set -e ; for d in $(DIRS) ; do $(MAKE) -C $$d $@ ; done
#
# Many systems don't have ncurses-devel installed
#
rtop: rtop.o $(PFMLIB)
	-$(CC) $(CFLAGS) $(LDFLAGS) -D_GNU_SOURCE -o $@ $^ $(LIBS) -lpthread -lncurses -lm

$(TARGETS): %:%.o $(LPC_UTILS) $(PFMLIB) $(PERF_EVENT_HDR)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $< $(LPC_UTILS) $(PFMLIB) $(LIBS)

$(LPC_UTILS): $(LPC_UTILS_HDR)

clean:
	@set -e ; for d in $(DIRS) ; do $(MAKE) -C $$d $@ ; done
	$(RM) -f *.o $(TARGETS) *~

distclean: clean

install_examples: $(TARGETS)
	@echo installing: $(TARGETS)
	-mkdir -p $(EXAMPLESDIR)
	$(INSTALL) -m 755 $(TARGETS) $(EXAMPLESDIR)
	@set -e ; for d in $(DIRS) ; do $(MAKE) -C $$d $@ ; done
#
# examples are installed as part of the RPM install, typically in /usr/share/doc/libpfm-X.Y/
#
.PHONY: install depend install_examples
