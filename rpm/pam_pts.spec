Name:           pam_pts
Version:        0.1
Release:        1%{?dist}
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
make %{?_smp_mflags} CPPFLAGS='-Wall -fpic -I. -DAUTHORIZE_FILE=\"/etc/pam_pts.confdir\"'

%install
rm -rf $RPM_BUILD_ROOT
install -D -m 0755 pam_pts.so $RPM_BUILD_ROOT/%{_lib}/security/pam_pts.so

# Install man page
install -D -m 0755 pam_pts.man $RPM_BUILD_ROOT/%{_mandir}/man8/pam_pts.8

# set up configuration
mkdir -p $RPM_BUILD_ROOT/etc
touch $RPM_BUILD_ROOT/etc/pam_pts.confdir

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc 00.Notes pam.conf rpm/README.config
/%{_lib}/security/pam_pts.so
%ghost %config /etc/pam_pts.confdir
%{_mandir}/man8/pam_pts.8.gz

%changelog
* Thu Aug 08 2013 Jonathan S. Billings <jsbillin@umich.edu> - 0.1-1
- Built initial version of package

