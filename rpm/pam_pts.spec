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
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
install -D -m 0755 pam_pts.so $RPM_BUILD_ROOT/%{_lib}/security/pam_pts.so

# set up configuration
mkdir -p $RPM_BUILD_ROOT/etc
echo '/etc/security/pam_pts' >  $RPM_BUILD_ROOT/etc/pam_pts.confdir
chmod 0700 $RPM_BUILD_ROOT/etc/pam_pts.confdir
install -d -m 0755 $RPM_BUILD_ROOT/etc/security/pam_pts

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc 00.Notes pam.conf rpm/README.config
/%{_lib}/security/pam_pts.so
%config /etc/pam_pts.confdir
%dir /etc/security/pam_pts


%changelog
