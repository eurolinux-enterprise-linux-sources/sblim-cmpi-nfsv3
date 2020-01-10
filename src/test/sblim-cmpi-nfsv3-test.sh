#!/bin/sh

# Substitute the current hostname into cim/Linux_NFSv3SystemSetting.cim
HOSTNAME=$( hostname -f )
FILE=cim/Linux_NFSv3SystemSetting.cim
mv $FILE $FILE.orig
cat $FILE.orig | sed -e "s|HOSTNAME|$HOSTNAME|g" > $FILE

# Test classes
CLASSNAMES='Linux_NFSv3SystemConfiguration Linux_NFSv3SystemSetting Linux_NFSv3SettingContext'

# Full pathname of the target config file
SOURCECONFIGFILE=exports.nfsv3
DESTCONFIGFILE=/etc/exports

# Install an example config file if none exists
unset CLEANUP
if [[ ! -a $DESTCONFIGFILE ]]; then
  echo "Copying test config file $SOURCECONFIGFILE to $DESTCONFIGFILE ..."
  cp -p $SOURCECONFIGFILE $DESTCONFIGFILE
  CLEANUP=true
fi

# Run the tests for each class
for CLASSNAME in $CLASSNAMES; do
   echo "Running tests for $CLASSNAME..."
   ./run.sh $CLASSNAME
done

# Cleanup
if [[ $CLEANUP = true ]]; then
  echo "Removing test config file $DESTCONFIGFILE ..."
  rm -f $DESTCONFIGFILE
fi
