/*
 * Copyright (c) 1992-1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef _PTS_UTILS_H
#define	_PTS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

void
__free_msg(int num_msg, struct pam_message *msg);

void
__free_resp(int num_msg, struct pam_response *resp);

int
__display_errmsg(
	int (*conv_funp)(),
	int num_msg,
	char messages[PAM_MAX_NUM_MSG][PAM_MAX_MSG_SIZE],
	void *conv_apdp
);

/*
 * PAM_MSG macro for return of internationalized text
 */

#define	PAM_MSG(pamh, number, string)\
	(char *) __pam_get_i18n_msg(pamh, "pam_unix", 3, number, string)

#ifdef __cplusplus
}
#endif

#endif /* _PTS_UTILS_H */
