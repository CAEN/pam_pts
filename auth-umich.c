#include <afs/stds.h>
#include <afs/cellconfig.h>
#include <afs/ptserver.h>
#include <afs/ptuser.h>

#define AUTHORIZE_FILE  "/etc/pam_pts.confdir"

#define AFSCONF_CLIENTNAME "/usr/vice/etc"

int allow_service_request(const char *user,
			  const char *service, const char *file)
{
  FILE *fp;
  char buf[1024];
  char * cp;
  static char sep[] = ", \t";	/* field separators for authorize file. */
  int result;
  
  /* If authorization file not configured, allow access. */
  if (!(fp = fopen(file ? file : AUTHORIZE_FILE, "r"))) {
      return 1;
  }
 
  while (fgets(buf, sizeof(buf), fp)) {
      /* Skip anything but ^\/absolutepath */ 
      if (*buf != '/') {
          continue;	
      }
  }
  fclose(fp);

  /* Open the authorization file. Deny access if we can't find it. */

  /* Kill trailing WS */
  for ( cp = (char *)strtok(buf, "\n \t") ; *cp ; ++cp) ;
  *cp++ = '/';
  gethostname(cp, (buf+sizeof(buf))-cp);
  if (!(fp = fopen(buf, "r"))) {
      strcpy(cp, "default");
      if (!(fp = fopen(buf, "r"))) {
        return 0;
      }
  }
  /* If pts server initialization fails, deny access. */
  if (pr_Initialize(0, AFSCONF_CLIENTNAME, 0)) {
    return 0;
  }
  
  /* Check every line in the file. */
  result = 0;
  while (fgets(buf, sizeof(buf), fp)) {
    char *group, *svc;
    int deny, match, flag;

    /* Get rid of leading whitespace, skip comments
       and empty lines, and strip newlines. */
    for (cp = buf; *cp == ' ' || *cp == '\t'; cp++) ;
    if (*cp == '#' || *cp == '\n' || !*cp) {
      continue;
    }
    if (cp = (char *)strchr(buf, '\n')) {
      *cp = 0;
    }

    /* Parse line. */
    match = deny = 0;
    if (!(group = (char *)strtok(buf, sep))) {
      continue;
    }
    for (svc = (char *)strtok(NULL, sep); svc != 0; 
         svc = (char *)strtok(NULL, sep)) {
      if (!strcasecmp(svc, service) || !strcmp(svc, "*")) {
        match = 1;
      }
      if (!strcasecmp(svc, "deny")) {
        deny = 1;
      }
    }
    if (match || deny) {
      /* Service matches, do we have group membership? */
      if ((!pr_IsAMemberOf(user, group, &flag) && flag) ||
          !strcasecmp(group, "system:anyuser") ||
          !strcasecmp(group, user)) {
        result = deny ? 0 : 1;
        break;
      }
    }
  }
  fclose(fp);
  return(result);
}
