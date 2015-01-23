# AFS Protection Group Authorization PAM module
# jhorwitz@umich.edu

MODULE=pam_pts.so

OBJECTS=	pts_acct_mgmt.o \
		utils.o \
		auth-umich.o 

SRCS= $(OBJECTS:%.o=%.c)

# include library definitions

BUILD.SO=      $(CC) -Wall -o $@ --shared $(OBJECTS) $(LDLIBS)

CPPFLAGS += -Wall -fpic -I.

# library dependency
# LDLIBS+= -lcmd
#AFSLIBS = -L/usr/lib -L/usr/lib/afs -lkauth -lprot -lubik -lauth.krb -laudit /usr/lib/afs/libsys.a -lrxkad -ldes -lrx /usr/lib/afs/libcom_err.a -lcom_err  -lafsint /usr/lib/afs/util.a -lresolv -lpthread 
# Most of the library needs have gone away with our conversion to
# kerberosV and openafs 1.6.2. Just be sure you have dev libraries
# for krb5, openafs and pam installed, and this should be sufficient.
# development environment installed.
ifndef _ARCH
ARCH := $(shell uname -m)
export _ARCH
endif

ifndef AFSLIBS
ifeq ($(ARCH),x86_64)
AFSLIBS = -L/usr/lib64 -L/usr/lib64/afs /usr/lib64/afs/libprot.a /usr/lib64/libafsauthent_pic.a -lafsrpc 
else
AFSLIBS = -L/usr/lib -L/usr/lib/afs /usr/lib/afs/libprot.a /usr/lib/libafsauthent_pic.a -lafsrpc
endif
endif # AFSLIBS

LDLIBS += -lc -lpam -lnsl -lresolv -lkrb5  $(AFSLIBS)

# resolve with local variables in shared library
DYNFLAGS = -o $(MODULE)

LINTFLAGS=

OWNER = root
GROUP = sys


$(MODULE): $(OBJECTS)
	$(BUILD.SO)
.KEEP_STATE:

all: $(MODULE) 

install: all

clean:
	rm -f $(OBJECTS) $(MODULE)
