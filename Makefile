# AFS Protection Group Authorization PAM module
# jhorwitz@umich.edu

MODULE=pam_pts.so.1

OBJECTS=	pts_acct_mgmt.o \
		utils.o \
		auth-umich.o

SRCS= $(OBJECTS:%.o=%.c)

# include library definitions

BUILD.SO=	$(CC) -o $@ -M mapfile -G $(DYNFLAGS) $(OBJECTS) $(LDLIBS)

CPPFLAGS += -K pic -I.

# library dependency
# LDLIBS+= -lcmd
AFSLIBS = -L/usr/lib -L/usr/lib/afs -lkauth -lprot -lubik -lauth.krb -laudit /usr/lib/afs/libsys.a -lrxkad -ldes -lrx -llwp -lcom_err -lafsint /usr/lib/afs/util.a /usr/ucblib/libucb.a
LDLIBS += -lc -lpam -lnsl -lsocket $(AFSLIBS)

# resolve with local variables in shared library
DYNFLAGS = -h pam_pts.so.1 -z defs -Bsymbolic

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
