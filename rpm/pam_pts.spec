%define confdir /etc/security/pam_pts

Name:           pam_pts
Version:        0.1
Release:        5%{?dist}
Summary:        PAM module that uses PTS membership to authorize access

Group:          System Environment/Base
License:        Public Domain
URL:            http://caen.engin.umich.edu/
Source0:        pam_pts-0.1.tar.gz

BuildRequires:  openafs-devel, openafs-authlibs-devel, krb5-devel, pam-devel
Requires:       openafs-client, openafs-authlibs, krb5-libs, pam

%description
PAM module that uses PTS membership to authorize access


%prep
%setup -q

%build
make %{?_smp_mflags} CPPFLAGS='-Wall -fpic -I. -DAUTHORIZE_FILE=\"/etc/security/pam_pts.confdir\"'

%install
rm -rf $RPM_BUILD_ROOT
install -D -m 0755 pam_pts.so $RPM_BUILD_ROOT/%{_lib}/security/pam_pts.so

# Install man page
install -D -m 0755 pam_pts.man $RPM_BUILD_ROOT/%{_mandir}/man8/pam_pts.8

# set up configuration
mkdir -p $RPM_BUILD_ROOT/etc/security/pam_pts
echo %confdir > $RPM_BUILD_ROOT/etc/security/pam_pts.confdir
install -D -m 0755 rpm/pam_pts.default $RPM_BUILD_ROOT%{confdir}/default

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc 00.Notes pam.conf rpm/README.config
/%{_lib}/security/pam_pts.so
%config(noreplace) /etc/security/pam_pts.confdir
%dir /etc/security/pam_pts
%config(noreplace) /etc/security/pam_pts/default
%{_mandir}/man8/pam_pts.8.gz

%changelog
* Wed Jul 23 2014 Jonathan S. Billings <jsbillin@umich.edu> - 0.1-5
- Fix specfile to reflect new location of config file

* Wed Jul 02 2014 Jonathan S. Billings <jsbillin@umich.edu> - 0.1-4
- Improve logging and fix location of configuration file

* Wed Jul 02 2014 Jonathan S. Billings <jsbillin@umich.edu> - 0.1-3
- Fix syslog() function

* Wed Jul 02 2014 Jonathan S. Billings <jsbillin@umich.edu> - 0.1-2
- Added libresolv dependency

* Thu Aug 08 2013 Jonathan S. Billings <jsbillin@umich.edu> - 0.1-1
- Built initial version of package

