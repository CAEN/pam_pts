#ifndef _AUTH_UMICH_H
#define	_AUTH_UMICH_H

#ifndef AUTHORIZE_FILE
#define AUTHORIZE_FILE  "/.authorize"
#endif

int allow_service_request(const char *user,
			  const char *service, const char *file);

#endif /* _AUTH_UMICH_H */
