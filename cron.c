#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <syslog.h>


int
pam_sm_acct_mgmt(
        pam_handle_t *pamh,
        int     flags,
        int     argc,
        const char **argv)
{

        char    *user;
        char    *pg;
        int     i;
        int     debug = 0;
        int     nowarn = 0;
        int     usedenyfile = 0;
        int     error = 0;
        struct pam_conv *pam_convp;

        if (pam_get_item(pamh, PAM_USER, (const void **)&user) != PAM_SUCCESS)
                return (PAM_SERVICE_ERR);

        if (pam_get_item(pamh, PAM_SERVICE, (const void **)&pg) != PAM_SUCCESS)
                return (PAM_SERVICE_ERR);

        error = pam_get_item(pamh, PAM_CONV, (const void**) &pam_convp);
        if (error != PAM_SUCCESS)
                return (error);

return (PAM_SUCCESS);

}

extern int
pam_sm_authenticate(
        pam_handle_t    *pamh,
        int             flags,
        int             argc,
        const char      **argv)
{

return (PAM_SUCCESS);

}

extern int
pam_sm_open_session(
        pam_handle_t    *pamh,
        int             flags,
        int             argc,
        const char      **argv)
{
    return PAM_SUCCESS;
}


extern int
pam_sm_close_session(
        pam_handle_t    *pamh,
        int             flags,
        int             argc,
        const char      **argv)
{
    return PAM_SUCCESS;
}

extern int
pam_sm_setcred(
        pam_handle_t    *pamh,
        int             flags,
        int             argc,
        const char      **argv)
{
    return PAM_SUCCESS;
}

