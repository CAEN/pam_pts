/*
 * Copyright (c) 1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <syslog.h>

int
pam_sm_open_session(
	pam_handle_t *pamh,
	int	flags,
	int	argc,
	const char **argv)
{
	return (PAM_SUCCESS);
}
