/*
 * Copyright (c) 1992-1995, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#include <security/pam_appl.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "utils.h"

void
__free_msg(num_msg, msg)
	int num_msg;
	struct pam_message *msg;
{
	int 			i;
	struct pam_message 	*m;

	if (msg) {
		m = msg;
		for (i = 0; i < num_msg; i++, m++) {
			if (m->msg)
				free(m->msg);
		}
		free(msg);
	}
}

/*
 * __free_resp():
 *	free storage for responses used in the call back "pam_conv" functions
 */

void
__free_resp(num_msg, resp)
	int num_msg;
	struct pam_response *resp;
{
	int			i;
	struct pam_response	*r;

	if (resp) {
		r = resp;
		for (i = 0; i < num_msg; i++, r++) {
			if (r->resp)
				free(r->resp);
		}
		free(resp);
	}
}

/*
 * __display_errmsg():
 *	display error message by calling the call back functions
 *	provided by the application through "pam_conv" structure
 */

int
__display_errmsg(conv_funp, num_msg, messages, conv_apdp)
	int (*conv_funp)();
	int num_msg;
	char messages[PAM_MAX_NUM_MSG][PAM_MAX_MSG_SIZE];
	void *conv_apdp;
{
	struct pam_message	*msg;
	struct pam_message	*m;
	struct pam_response	*resp;
	int			i;
	int			k;
	int			retcode;

	msg = (struct pam_message *)calloc(num_msg,
					sizeof (struct pam_message));
	if (msg == NULL) {
		return (PAM_CONV_ERR);
	}
	m = msg;

	i = 0;
	k = num_msg;
	resp = NULL;
	while (k--) {
		/*
		 * fill out the pam_message structure to display error message
		 */
		m->msg_style = PAM_ERROR_MSG;
		m->msg = (char *)malloc(PAM_MAX_MSG_SIZE);
		if (m->msg != NULL)
			(void) strcpy(m->msg, (const char *)messages[i]);
		else
			continue;
		m++;
		i++;
	}

	/*
	 * Call conv function to display the message,
	 * ignoring return value for now
	 */
	retcode = conv_funp(num_msg, &msg, &resp, conv_apdp);
	__free_msg(num_msg, msg);
	__free_resp(num_msg, resp);
	return (retcode);
}
