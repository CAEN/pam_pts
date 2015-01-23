pam_pts
=======

PAM module that uses AFS PTS groups for managing access

<P>
<A NAME="lbAB">&nbsp;</A>
<H2>NAME</H2>

pam_pts - Control authorization via AFS PTS Groups
<P>
<A NAME="lbAC">&nbsp;</A>
<H2>SYNOPSIS</H2>

<B>account required pam_pts.so</B>

<P>
<A NAME="lbAD">&nbsp;</A>
<H2>DESCRIPTION</H2>

The <B>pam_pts</B> module is designed to allow control over who is
authorized to use a service by the user's membership in a PTS group.
<P>
Configuration is managed by an setting the path to all authorization
files in the &quot;authorization file&quot;, which by default is
<I>/etc/security/pam_pts.confdir</I>.  If this file does not exist, then this pam
module always suceeds.  The contents of this file should simply be a
path on the disk.  If the path doesn't exist, then this module always
denies. 
<P>
In the configured path, there are two possible files <B>pam_pts</B>
will look for.  The first is the hostname, as returned by the
gethostname() syscall.  If that file doesn't exist, then it reads the
file <I>default</I>.  If this file doesn't exist, then the pam module
denies access.
<P>
The configuration file lists PTS groups and what service is allowed.
Each line starts with a PTS group, then a list of services, separated
by a space, tab or comma.  All leading whitespace, empty lines
and #-delimited comments are ignored.  All lines with improper formatting
are skipped.
<P>
Each line in the configuration line is read, and the PAM service
calling the pam_pts function is matched against the service in the
line.  There are two special names for services, &quot;deny&quot; and &quot;*&quot;.
For &quot;deny&quot; the user is checked against the PTS group, and if the user
is a member, then authorization is denied.  If the PAM service matches
one of the services described on the line, or the service is &quot;*&quot;, then
pam_pts checks to see if the user is a member of the PTS group, and if
they are, then the module succeeds.  If the module gets to the end of
the file and there have been no matches, then the module denies
authorization.
<P>
<A NAME="lbAE">&nbsp;</A>
<H2>ARGUMENTS</H2>

<P>
<DL COMPACT>
<DT>debug<DD>
turns on debugging via <B>syslog</B>(3).  Debugging messages are logged with
priority <I>LOG_DEBUG</I>.
<P>
<DT>denyfile=<I>/path/to/file</I><DD>
defines the file to print to the user when they are denied.
<P>
<DT>nowarn<DD>
does not log any messages on failure
<P>
</DL>
<A NAME="lbAF">&nbsp;</A>
<H2>FILES</H2>

<I>/etc/security/pam_pts.confdir</I>
<BR>

<I>/etc/security/pam_pts/default</I>
<BR>

<I>/etc/security/pam_pts/&lt;hostname&gt;</I>
<BR>

<P>
<A NAME="lbAG">&nbsp;</A>
<H2>SEE ALSO</H2>

<B>pam_krb5</B>(5)
