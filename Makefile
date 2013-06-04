# AFS Protection Group Authorization PAM module
# jhorwitz@umich.edu

MODULE=pam_pts.so

OBJECTS=	pts_acct_mgmt.o \
		utils.o \
		auth-umich.o 
#		comerr_add.o

SRCS= $(OBJECTS:%.o=%.c)

# include library definitions

BUILD.SO=      $(CC) -o $@ --shared $(OBJECTS) $(LDLIBS)

CPPFLAGS += -fpic -I.

# library dependency
# LDLIBS+= -lcmd
#AFSLIBS = -L/usr/lib -L/usr/lib/afs -lkauth -lprot -lubik -lauth.krb -laudit /usr/lib/afs/libsys.a -lrxkad -ldes -lrx /usr/lib/afs/libcom_err.a -lcom_err  -lafsint /usr/lib/afs/util.a -lresolv -lpthread 
# Most of the library needs have gone away with our conversion to
# kerberosV and openafs 1.6.2. Just be sure you have dev libraries
# for krb5, openafs and pam installed, and this should be sufficient.
# development environment installed.
AFSLIBS = -L/usr/lib -L/usr/lib/afs

LDLIBS += -lc -lpam -lnsl  $(AFSLIBS)

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
