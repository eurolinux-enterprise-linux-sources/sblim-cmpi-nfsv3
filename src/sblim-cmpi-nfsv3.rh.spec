#
# $Id: 
#
# Package spec for sblim-cmpi-nfsv3 - Fedora/RedHat flavor
#

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Summary: SBLIM NFSV3 Instrumentation
Name: sblim-cmpi-nfsv3
Version: 1.0.14
Release: 1
Group: Systems Management/Base
URL: http://www.sblim.org
License: EPL

Source0: http://prdownloads.sourceforge.net/sblim/%{name}-%{version}.tar.bz2

BuildRequires: tog-pegasus-devel >= 2.5.1
BuildRequires: sblim-cmpi-base-devel >= 1.5

Requires: tog-pegasus >= 2.5.1
Requires: sblim-cmpi-base >= 1.5

%Description
Standards Based Linux Instrumentation NFSV3 Providers

%Package devel
Summary: SBLIM NFSV3 Instrumentation Header Development Files
Group: Systems Management/Base
Requires: %{name} = %{version}-%{release}

%Description devel
SBLIM Base NFSV3 Development Package

%Package test
Summary: SBLIM NFSV3 Instrumentation Testcases
Group: Systems Management/Base
Requires: %{name} = %{version}-%{release}
Requires: sblim-testsuite

%Description test
SBLIM Base NFSV3 Testcase Files for SBLIM Testsuite

%prep

%setup -q

%build

%configure TESTSUITEDIR=%{_datadir}/sblim-testsuite CIMSERVER=pegasus

make %{?_smp_mflags}

%clean

rm -rf $RPM_BUILD_ROOT

%install

rm -rf $RPM_BUILD_ROOT

make DESTDIR=$RPM_BUILD_ROOT install

# remove unused libtool files
rm -f $RPM_BUILD_ROOT/%{_libdir}/*a
rm -f $RPM_BUILD_ROOT/%{_libdir}/cmpi/*a

%pre
%define SCHEMA %{_datadir}/%{name}/Linux_NFSv3SystemConfiguration.mof \
                %{_datadir}/%{name}/Linux_NFSv3SystemSetting.mof
%define REGISTRATION %{_datadir}/%{name}/Linux_NFSv3SystemConfiguration.registration \
                %{_datadir}/%{name}/Linux_NFSv3SystemSetting.registration

# If upgrading, deregister old version
if [ $1 -gt 1 ]
then
  %{_datadir}/%{name}/provider-register.sh -d -t pegasus \
	-r %{REGISTRATION} -m %{SCHEMA} > /dev/null 2>&1
fi

%post
# Register Schema and Provider - this is highly provider specific

%{_datadir}/%{name}/provider-register.sh -t pegasus \
	-r %{REGISTRATION} -m %{SCHEMA} > /dev/null 2>&1

/sbin/ldconfig

%preun
# Deregister only if not upgrading 
if [ $1 -eq 0 ]
then
  %{_datadir}/%{name}/provider-register.sh -d -t pegasus \
	-r %{REGISTRATION} -m %{SCHEMA} > /dev/null 2>&1
fi

%postun -p  /sbin/ldconfig

%files

%defattr(-,root,root) 
%docdir %{_datadir}/doc/%{name}-%{version}
%{_datadir}/%{name}
%{_datadir}/doc/%{name}-%{version}
%{_libdir}/*.so.*
%{_libdir}/cmpi/*.so

%files devel

%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/*.so

%files test

%defattr(-,root,root)
%{_datadir}/sblim-testsuite

%changelog
* Mon Dec 12 2005 Lynn Moss <lynnmoss@us.ibm.com>	1.0.13
  - initial support
