# $Id: 
# ==================================================================
# (C) Copyright IBM Corp. 2005, 2009
#
# THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
# ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
# CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
#
# You can obtain a current copy of the Eclipse Public License from
# http://www.opensource.org/licenses/eclipse-1.0.php
#
# Author:       Lynn Moss <lynnmoss@us.ibm.com>
# Description:  Automake input file for NFSV3 Providers
# ==================================================================


# SBLIM Include Directory
sblimincdir = $(includedir)/sblim

# Start from an empty extra distribution file list
EXTRA_DIST=

# docdir defines where the documentation goes
docdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)

#
# Automake instructions for documentation
#
doc_DATA=README AUTHORS COPYING 

# ADD EXTRA DOC FILES IF PRESENT
doc_DATA+=DEBUG README.tog-pegasus
EXTRA_DIST+=DEBUG README.tog-pegasus

# providerdir defines where provider libraries will be installed (@PROVIDERDIR@ is set by the configure script). 
providerdir = @PROVIDERDIR@


#
# Automake instructions for main dir
#

#  NFSv3 CMPI provider libraries
provider_LTLIBRARIES = 	libLinux_NFSv3SettingContext.la \
			libLinux_NFSv3SystemConfiguration.la \
			libLinux_NFSv3SystemSetting.la 

# SettingContext
libLinux_NFSv3SettingContext_la_SOURCES = \
		Linux_NFSv3SettingContext.c
libLinux_NFSv3SettingContext_la_LIBADD = \
		-lpthread -lcmpiOSBase_Common \
		-lLinux_NFSv3SystemConfigurationUtil
libLinux_NFSv3SettingContext_la_LDFLAGS = -avoid-version -shared

# SystemConfiguration
libLinux_NFSv3SystemConfiguration_la_SOURCES = \
		Linux_NFSv3SystemConfiguration.c
libLinux_NFSv3SystemConfiguration_la_LIBADD = \
		-lpthread -lcmpiOSBase_Common \
		-lLinux_NFSv3SystemConfigurationUtil
libLinux_NFSv3SystemConfiguration_la_LDFLAGS = -avoid-version -shared

# SettingContext
libLinux_NFSv3SystemSetting_la_SOURCES = \
		Linux_NFSv3SystemSetting.c
libLinux_NFSv3SystemSetting_la_LIBADD = \
		-lpthread -lcmpiOSBase_Common \
		-lLinux_NFSv3SystemConfigurationUtil 
libLinux_NFSv3SystemSetting_la_LDFLAGS = -avoid-version -shared

# NFSv3 support utility library
# Automake instructions for lex and yacc files
AM_YFLAGS=-d
BUILT_SOURCES=parser.c lexer.c \
	xmlparser.c xmllexer.c

lib_LTLIBRARIES = libLinux_NFSv3SystemConfigurationUtil.la

libLinux_NFSv3SystemConfigurationUtil_la_SOURCES = \
		util/Linux_NFSv3SystemConfigurationUtil.c \
		util/instance2string/instance2string.c \
		util/parser/lexer.l \
		util/parser/parser.y \
		util/xmlparser/xmllexer.l \
		util/xmlparser/xmlparser.y 

libLinux_NFSv3SystemConfigurationUtil_la_LIBADD = \
		-lpthread -lcmpiOSBase_Common 
libLinux_NFSv3SystemConfigurationUtil_la_LDFLAGS = -avoid-version -shared

lexer.c: util/parser/lexer.l util/parser/parser.y
	lex -t util/parser/lexer.l | sed -e "s/yy/NFSv3yy/g" > lexer.c

parser.c: util/parser/lexer.l util/parser/parser.y
	yacc -p NFSv3yy -d -o parser.c util/parser/parser.y

xmllexer.c: util/xmlparser/xmllexer.l util/xmlparser/xmlparser.y
	lex -t util/xmlparser/xmllexer.l | sed -e "s/yy/NFSv3xmlyy/g" > xmllexer.c

xmlparser.c: util/xmlparser/xmllexer.l util/xmlparser/xmlparser.y
	yacc -p NFSv3xmlyy -d -o xmlparser.c util/xmlparser/xmlparser.y 

# Installable Header Files 
# Non-Installable Header Files
noinst_HEADERS = GLOBALS.h \
	util/Linux_SystemConfigurationUtil.h

# We must explicity add the RPM spec files to the distribution package
EXTRA_DIST+=$(PACKAGE).spec $(PACKAGE).rh.spec

#
# Automake instructions for ./mof subdir
#
SCHEMAS=mof/Linux_NFSv3SystemSetting.mof \
	mof/Linux_NFSv3SystemConfiguration.mof 
REGISTRATIONS=mof/Linux_NFSv3SystemSetting.registration \
	mof/Linux_NFSv3SystemConfiguration.registration

# We must explicity add all the schema files to the distribution package
pkgdata_DATA=$(SCHEMAS) $(REGISTRATIONS)
pkgdata_SCRIPTS=provider-register.sh
EXTRA_DIST+=mof $(pkgdata_SCRIPTS)

# Register the provider(s) and class definition(s) to the current CIM server/CIMOM
postinstall:
	sh provider-register.sh -t @CIMSERVER@ -r $(REGISTRATIONS) -m $(SCHEMAS)

preuninstall:
	sh provider-register.sh -d -t @CIMSERVER@ -r $(REGISTRATIONS) -m $(SCHEMAS)

dist-hook:
	test -d "$(distdir)" &&	rm -rf `find $(distdir) -type d -name CVS`
#
# Automake instructions for ./test subdir
#
EXTRA_DIST+=test README.TEST
if TESTSUITE
testsuitedir=@TESTSUITEDIR@
testsuitesystemdir=@TESTSUITEDIR@/system/linux
testsuitecimdir=@TESTSUITEDIR@/cim

testsuite_SCRIPTS=test/sblim-cmpi-nfsv3-test.sh

testsuite_DATA=test/exports.nfsv3

testsuitesystem_DATA=test/system/linux/Linux_NFSv3SystemConfiguration.system \
	test/system/linux/Linux_NFSv3SettingContext.system \
	test/system/linux/Linux_NFSv3SystemSetting.system

testsuitecim_DATA=test/cim/Linux_NFSv3SystemConfiguration.cim \
	test/cim/Linux_NFSv3SettingContext.cim \
	test/cim/Linux_NFSv3SystemSetting.cim 

runtest: install
	cd $(DESTDIR)$(TESTSUITEDIR) &&	./sblim-cmpi-nfsv3-test.sh

doc_DATA+=README.TEST
endif

# Rules for standalone testing of the parser and xml parser.  To run these
# tests issue:
#     make runparsertest
#          AND
#     make runxmlparsertest
#
# Note that the program CFLAGS are a workaround so that the programs source
# files will have a different name than the source files included in the
# library to keep libtool happy.

EXTRA_DIST+= util/parser/samples/exports \
		util/xmlparser/samples/exports.xml \
		util/xmlparser/samples/xinetd.xml
noinst_PROGRAMS = parsertest xmlparsertest

parsertest_SOURCES = util/parser/parser.y \
			util/parser/lexer.l
parsertest_CFLAGS = $(AM_CFLAGS)

xmlparsertest_SOURCES = util/xmlparser/xmlparser.y \
			util/xmlparser/xmllexer.l \
			util/xmlparser/setProperty.c
xmlparsertest_CFLAGS = $(AM_CFLAGS)

runparsertest: parsertest
	./parsertest < ./util/parser/samples/exports

runxmlparsertest: xmlparsertest
	./xmlparsertest < ./util/xmlparser/samples/exports.xml
	./xmlparsertest < ./util/xmlparser/samples/xinetd.xml
