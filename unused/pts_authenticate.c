/*
 * Copyright (c) 1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <syslog.h>
#include <libintl.h>

/*
 *
 * Sample module for pam_sm_authenticate.
 *
 */

/*
 * pam_sm_authenticate		- Authenticate user
 */

int
pam_sm_authenticate(
	pam_handle_t		*pamh,
	int 			flags,
	int			argc,
	const char		**argv)
{
	char			*user;
	struct pam_conv 	*pam_convp;
	int			err;

	err = pam_get_user(pamh, &user, NULL);
	if (err != PAM_SUCCESS)
		return (err);

	err = pam_get_item(pamh, PAM_CONV, (void**) &pam_convp);
	if (err != PAM_SUCCESS)
		return (err);

	return (PAM_SUCCESS);
}
