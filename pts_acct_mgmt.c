/* Written by jhorwitz@umich.edu -- yay for me */

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

#include "utils.h"

#define DEFAULT_DENY_FILE "/etc/login.sorry"
#define DEFAULT_DENY_MSG "You are not authorized to use this service.\n"

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
	int	debug = 0;
	int	nowarn = 0;
	int	usedenyfile = 0;
	int	error = 0;
	struct pam_conv *pam_convp;

	if (pam_get_item(pamh, PAM_USER, (void **)&user) != PAM_SUCCESS)
		return (PAM_SERVICE_ERR);

	if (pam_get_item(pamh, PAM_SERVICE, (void **)&pg) != PAM_SUCCESS)
		return (PAM_SERVICE_ERR);

	error = pam_get_item(pamh, PAM_CONV, (void**) &pam_convp);
	if (error != PAM_SUCCESS)
		return (error);

	if (user == 0 || *user == '\0' || (strcmp(user, "root") == 0))
		return (PAM_SUCCESS);

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
		else
			syslog(LOG_DEBUG, "illegal option %s", argv[i]);
	}

	if (allow_service_request(user,pg,NULL)) {
		syslog(LOG_ERR, "pam_pts: service granted to %s", user);
		return(PAM_SUCCESS);
	}
	else {
		syslog(LOG_ERR, "pam_pts: ACCESS DENIED for %s", user);
		deny_service(pam_convp,
			usedenyfile ? denyfile : DEFAULT_DENY_FILE);
		return(PAM_PERM_DENIED);
	}
}
