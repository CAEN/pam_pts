#ifndef _AUTH_UMICH_H
#define	_AUTH_UMICH_H

#define AUTHORIZE_FILE  "/etc/pam_pts.confdir"
#define AFSCONF_CLIENTNAME "/usr/vice/etc"

int allow_service_request(const char *user,
			  const char *service, const char *file);

#endif /* _AUTH_UMICH_H */
