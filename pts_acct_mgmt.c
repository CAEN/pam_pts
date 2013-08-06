/* Written by jhorwitz@umich.edu -- yay for me */

/* adbisaro@umich.edu                                */
/* Sun, 15 Dec 2002, 1713 hrs EST (GMT-0500)         */
/* improve logging, fix possible format-strings bug. */

#include <syslog.h>
#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <libintl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>

#include "utils.h"
#include "auth-umich.h"

#define DEFAULT_DENY_FILE "/etc/login.sorry"
#define DEFAULT_DENY_MSG "You are not authorized to use this service.\n"
#define CL_RETURN(X) { closelog() ; return((X)) ; }

void deny_service(
	struct pam_conv *c,
	char *file)
{
	int fd;
	int n;
	int usedefault = 0;
	char buf[PAM_MAX_MSG_SIZE];
	char messages[1][PAM_MAX_MSG_SIZE];

	if ((fd = open(file,O_RDONLY)) == -1) {
		usedefault = 1;
	}
	else {
		n = read(fd,buf,PAM_MAX_MSG_SIZE-1);
		close(fd);
	}

	if (n >= 0 && !usedefault) {
		strncpy(messages[0],buf,n+1);
	}
	else {
		strncpy(messages[0],DEFAULT_DENY_MSG,strlen(DEFAULT_DENY_MSG)+1);
	}

	__display_errmsg(c->conv,1,&messages[0],c->appdata_ptr);
}


int
pam_sm_acct_mgmt(
	pam_handle_t *pamh,
	int	flags,
	int	argc,
	const char **argv)
{
	char	*user;
	char	*pg;
	char	denyfile[MAXPATHLEN];
	int	i;
	int	debug = 1;
	int	nowarn = 0;
	int	usedenyfile = 0;
	int	error = 0;
	struct pam_conv *pam_convp;

        openlog("pam_pts", LOG_PID, LOG_AUTH);

	if (pam_get_item(pamh, PAM_USER, (const void **)&user) != PAM_SUCCESS) {
		syslog(LOG_DEBUG, 
                       "pam service error:  Could not get username.");
		CL_RETURN(PAM_SERVICE_ERR);
	}

	if (pam_get_item(pamh, PAM_SERVICE, (const void **)&pg) != PAM_SUCCESS) {
		syslog(LOG_DEBUG, 
                       "pam service error:  Could not get group.");
		CL_RETURN(PAM_SERVICE_ERR);
	}

	error = pam_get_item(pamh, PAM_CONV, (const void**) &pam_convp);

	if (error != PAM_SUCCESS) {
		syslog(LOG_DEBUG, 
                       "pam service error:  Could not get pam_conv.");
		CL_RETURN(error);
	}

	if (user == 0 ) {
		syslog(LOG_DEBUG, "allowing access for null username ptr");
		CL_RETURN(PAM_SUCCESS);
	}
	if (*user == '\0') {
		syslog(LOG_DEBUG, "allowing access for zero-length username");
		CL_RETURN(PAM_SUCCESS);
	}
        if (strncmp(user, "root", 5) == 0) {
		syslog(LOG_NOTICE, "allowing access for root");
		CL_RETURN(PAM_SUCCESS);
	}

	for (i = 0; i < argc; i++) {
		if (strcasecmp(argv[i], "debug") == 0)
			debug = 1;
		else if (strcasecmp(argv[i], "nowarn") == 0) {
			nowarn = 1;
			flags = flags | PAM_SILENT;
		}
		else if (strncasecmp(argv[i], "denyfile=", 9) == 0) {
			/* don't trust the length of the option! */
			strncpy(denyfile,argv[i]+9,MAXPATHLEN);
			usedenyfile = 1;
		}
		else {
			/* If we can't trust it in the previous case,  */
			/* then we can't trust it here either.         */
			syslog(LOG_DEBUG, "illegal option passed in argv");
                }
	}

	if (allow_service_request(user,pg,NULL)) {
		syslog(LOG_NOTICE, "service granted to %s", user);
		CL_RETURN(PAM_SUCCESS);
	}
	else {
		syslog(LOG_ERR, "ACCESS DENIED for %s", user);
		deny_service(pam_convp,
			usedenyfile ? denyfile : DEFAULT_DENY_FILE);
		CL_RETURN(PAM_PERM_DENIED);
	}
}
