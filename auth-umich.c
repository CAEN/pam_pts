#include <afs/stds.h>
#include <afs/cellconfig.h>
#include <afs/ptserver.h>

#define AUTHORIZE_FILE  "/.authorize"

#define  AFSCONF_CLIENTNAME "/usr/vice/etc"

int allow_service_request(const char *user,
			  const char *service, const char *file)
{
  FILE *fp;
  char buf[1024], *cp;
  static char sep[] = ", \t";	/* field separators for authorize file. */
  int result;
  
  /* If authorization file not configured, allow access. */
  if (!(fp = fopen(file ? file : AUTHORIZE_FILE, "r")))
    return 1;
  
  while (fgets(buf, sizeof(buf), fp))
    if (*buf != '/') continue;	/* Skip anything but ^\/absolutepath */
  fclose(fp);

  /* Open the authorization file. Deny access if we can't find it. */
  for (cp = strtok(buf, "\n \t"); *cp; ++cp) ;	/* Kill trailing WS */
  *cp++ = '/';
  gethostname(cp, (buf+sizeof(buf))-cp);
  if (!(fp = fopen(buf, "r"))) {
    strcpy(cp, "default");
    if (!(fp = fopen(buf, "r")))
      return 0;
  }
  /* If pts server initialization fails, deny access. */
  if (pr_Initialize(0, AFSCONF_CLIENTNAME, 0)) return 0;
  
  /* Check every line in the file. */
  result = 0;
  while (fgets(buf, sizeof(buf), fp))
    {
      char *group, *svc;
      int deny, match, flag;

      /* Get rid of leading whitespace, skip comments
	 and empty lines, and strip newlines. */
      for (cp = buf; *cp == ' ' || *cp == '\t'; cp++) ;
      if (*cp == '#' || *cp == '\n' || !*cp) continue;
      if (cp = strchr(buf, '\n')) *cp = 0;
      
      /* Parse line. */
      match = deny = 0;
      if (!(group = strtok(buf, sep))) continue;
      for (svc = strtok(NULL, sep); svc != 0; svc = strtok(NULL, sep)) {
	if (!strcasecmp(svc, service) || !strcmp(svc, "*")) match = 1;
	if (!strcasecmp(svc, "deny")) deny = 1;
      }
      if (match || deny)
	/* Service matches, do we have group membership? */
	if ((!pr_IsAMemberOf(user, group, &flag) && flag) ||
	    !strcasecmp(group, "system:anyuser") ||
	    !strcasecmp(group, user)) {
	  result = deny ? 0 : 1;
	  break;
	}
    }
  fclose(fp);
  return(result);
}
