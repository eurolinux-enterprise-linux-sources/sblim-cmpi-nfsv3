/* ---------------------------------------------------------------------------
 * Linux_SystemConfigurationUtil.c
 *
 * Copyright (c) IBM Corp. 2004, 2009
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:	Dr. Gareth S. Bestor <bestorga@us.ibm.com>
 * Description: Utility functions for the generic config file instance provider
 * Interface:   Common Manageability Programming Interface (CMPI)
 *
 * IMPORTANT: This file must be renamed to the desired new
 * Linux_SystemConfiguration classname; e.g.
 *	mv Linux_SystemConfigurationUtil.c Linux_NFSv3SystemConfigurationUtil.c
 * In most cases, no other changes are necessary to customize this file for 
 * new Linux_SystemConfiguration instance providers!
 * --------------------------------------------------------------------------- */

/* Required C library headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

/* Required CMPI library headers */
#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"
#include "OSBase_Common.h"
#include "cmpiOSBase_Common.h"

/* Required provider headers */
#include "Linux_SystemConfigurationUtil.h"
#include "GLOBALS.h"


/* ---------------------------------------------------------------------------
 * GLOBAL DEFINITIONS
 * --------------------------------------------------------------------------- */

static const CMPIBroker * _BROKER;
static const CMPIInstance * _INSTANCE = NULL;	/* The current instance being read */

/* Record containing essential file info: filename and file handle */
typedef struct {
   char name[_MAXNAMELENGTH];
   FILE * handle;
} _FILEINFO;


/* ---------------------------------------------------------------------------
 * IMPORTED EXTERNALLY DEFINED FUNCTIONS
 * --------------------------------------------------------------------------- */

extern int NFSv3yyparsefile( FILE * infile, FILE * outfile );                /* Config file parser */
extern int NFSv3xmlyyparse();						/* XML instance parser */
extern int NFSv3xmlyyrestart( FILE * infile );                               /* Redirect XML parser input to a file */
extern int Linux_NFSv3_instance2string( const CMPIInstance * instance, char ** buffer );  /* Instance-to-text formatter */


/* ---------------------------------------------------------------------------
 * INTERNAL UTILITY FUNCTIONS
 * --------------------------------------------------------------------------- */

/* Convert a type name string to its corresponding CIM simple data type */
static CMPIType _CMTypeFromChars( const char * typename, CMPIStatus * status )
{
   CMSetStatus(status, CMPI_RC_OK);
  
   if (!strcmp(typename,"string")) return CMPI_string;
   else if (!strcmp(typename,"dateTime")) return CMPI_dateTime;
   else if (!strcmp(typename,"boolean")) return CMPI_boolean;
   else if (!strcmp(typename,"char16")) return CMPI_char16;
   else if (!strcmp(typename,"uint8")) return CMPI_uint8;
   else if (!strcmp(typename,"sint8")) return CMPI_sint8;
   else if (!strcmp(typename,"uint16")) return CMPI_uint16;
   else if (!strcmp(typename,"sint16")) return CMPI_sint16;
   else if (!strcmp(typename,"uint32")) return CMPI_uint32;
   else if (!strcmp(typename,"sint32")) return CMPI_sint32;
   else if (!strcmp(typename,"uint64")) return CMPI_uint64;
   else if (!strcmp(typename,"sint64")) return CMPI_sint64;
   else if (!strcmp(typename,"real32")) return CMPI_real32;
   else if (!strcmp(typename,"real64")) return CMPI_real64;
   else {
      _OSBASE_TRACE(1,("_CMTypeFromChars() : Unrecognized type name '%s'", typename));
      CMSetStatus(status, CMPI_RC_ERR_FAILED);
      return CMPI_null;
   }
}


/* Convert a value string to its corresponding CIM simple data value */
static CMPIValue _CMValueFromChars( const char * valuebuffer, CMPIType type, CMPIStatus * status, const CMPIBroker * broker )
{
   CMPIValue value;             /* CIM value corresponding to the valuebuffer */

   CMSetStatus(status, CMPI_RC_OK);
   errno = 0;

   switch (type) {
      case CMPI_string:
         value.string = CMNewString(broker, valuebuffer, status);
         if (status->rc != CMPI_RC_OK) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_string",valuebuffer));
         }
         break;

      case CMPI_dateTime:
	 value.dateTime = CMNewDateTimeFromChars(broker, valuebuffer, status);
         if (status->rc != CMPI_RC_OK) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_dateTime",valuebuffer));
         }
         break;

      case CMPI_boolean:
	 if (!strcmp(valuebuffer,"true") || !strcmp(valuebuffer,"TRUE")) {
	    value.boolean = 1;
	 } else if (!strcmp(valuebuffer,"false") || !strcmp(valuebuffer,"FALSE")) {
	    value.boolean = 0;
	 } else {
	    _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_boolean",valuebuffer));
	    CMSetStatus(status, CMPI_RC_ERR_FAILED);
	 }
	 break;

      case CMPI_char16:
	 value.char16 = strtoul(valuebuffer,NULL,0);
	 if (errno) {
	    _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_char16",valuebuffer));
	    CMSetStatus(status, CMPI_RC_ERR_FAILED);
	 }
	 break;

      case CMPI_uint8:
	 value.uint8 = strtoul(valuebuffer,NULL,0);
	 if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_uint8",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_sint8:
     	 value.sint8 = strtol(valuebuffer,NULL,0);
	 if (errno) {
	    _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_sint8",valuebuffer));
	    CMSetStatus(status, CMPI_RC_ERR_FAILED);
	 }
	 break;

      case CMPI_uint16:
	 value.uint16 = strtoul(valuebuffer,NULL,0);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_uint16",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_sint16:
	 value.sint16 = strtol(valuebuffer,NULL,0);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_sint16",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_uint32:
	 value.uint32 = strtoul(valuebuffer,NULL,0);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_uint32",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_sint32:
	 value.sint32 = strtol(valuebuffer,NULL,0);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_sint32",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_uint64:
	 value.uint64 = strtoul(valuebuffer,NULL,0);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_uint64",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_sint64:
	 value.sint64 = strtol(valuebuffer,NULL,0);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_sint64",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_real32:
	 value.real32 = strtod(valuebuffer,NULL);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_real32",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      case CMPI_real64:
	 value.real64 = strtod(valuebuffer,NULL);
         if (errno) {
            _OSBASE_TRACE(1,("_CMValueFromChars() : Failed to convert '%s' to CMPI_real64",valuebuffer));
            CMSetStatus(status, CMPI_RC_ERR_FAILED);
         }
         break;

      default:
         _OSBASE_TRACE(1,("_CMValueFromChars() : Unrecognized CIM type=%d",type));
         CMSetStatus(status, CMPI_RC_ERR_FAILED);
         break;
   }
   return value;
}


/* Compare two CIM data values to see if they are identical */
static int _CMSameValue( CMPIData value1, CMPIData value2 )
{
   CMPIValue v1 = value1.value;
   CMPIValue v2 = value2.value;

   /* Check that the type of the two CIM values is the same */
   if (value1.type != value2.type) return 0;

   /* Check that the value of the two CIM values is the same */
   switch (value1.type) {
      case CMPI_string:   return !strcmp(CMGetCharPtr(v1.string), CMGetCharPtr(v2.string));
      case CMPI_dateTime: return CMGetBinaryFormat(v1.dateTime, NULL) == CMGetBinaryFormat(v2.dateTime, NULL);
      case CMPI_boolean:  return v1.boolean == v2.boolean;
      case CMPI_char16:   return v1.char16 == v2.char16;
      case CMPI_uint8:    return v1.uint8 == v2.uint8;
      case CMPI_sint8:    return v1.sint8 == v2.sint8;
      case CMPI_uint16:   return v1.uint16 == v2.uint16;
      case CMPI_sint16:   return v1.sint16 == v2.sint16;
      case CMPI_uint32:   return v1.uint32 == v2.uint32;
      case CMPI_sint32:   return v1.sint32 == v2.sint32;
      case CMPI_uint64:   return v1.uint64 == v2.uint64;
      case CMPI_sint64:   return v1.sint64 == v2.sint64;
      case CMPI_real32:   return v1.real32 == v2.real32;
      case CMPI_real64:   return v1.real64 == v2.real64;
      default: _OSBASE_TRACE(1,("_CMSameValue() : Unrecognized type CIM type=%d",value1.type)); return 0;
   }
}


/* ---------------------------------------------------------------------------
 * EXPORTED LIBRARY FUNCTIONS
 * --------------------------------------------------------------------------- */

/*
 * setProperty
 */
int NFSv3setProperty( const char * name, const char * typename, const char * valuebuffer )
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* Return status of CIM operations */
   CMPIType type;				/* CIM type corresponding to typename */
   CMPIValue value;				/* CIM value corresponding to valuebuffer */

   if (_INSTANCE == NULL) return 0;

   _OSBASE_TRACE(2,("setProperty() : name='%s' type='%s' value='%s'", name, typename, valuebuffer));

   /* Convert type to CIM type */
   type = _CMTypeFromChars(typename, &status);
   if (status.rc != CMPI_RC_OK) return 0;

   /* Convert value to CIM value */
   value = _CMValueFromChars(valuebuffer, type, &status, _BROKER);
   if (status.rc != CMPI_RC_OK) return 0;

   /* Set the property's simple data value */
   status = CMSetProperty(_INSTANCE, name, &value, type);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("setProperty() : Failed to set property value"));
      return 0;
   }
   return 1;
}


/*
 * setArrayProperty
 */
int NFSv3setArrayProperty( const char * name, const char * typename, const char * valuebuffer )
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* Return status of CIM operations */
   CMPIType type;                               /* CIM type corresponding to typename */
   CMPIValue value;                             /* CIM value corresponding to valuebuffer */
   CMPIData olddata;				/* Old property value array */
   CMPICount oldsize;                           /* Size of the old property value array */
   CMPIData newdata;				/* New property value array */
   CMPIData data;	  			/* General purpose CIM data to copy array vlaues */ 
   CMPIType arraytype;				/* CIM array type of the property */
   int i;

   if (_INSTANCE == NULL) return 0;

   _OSBASE_TRACE(2,("setArrayProperty() : name='%s' type='%s' value='%s'", name, typename, valuebuffer));

   /* Convert type to CIM type */
   type = _CMTypeFromChars(typename, &status);
   if (status.rc != CMPI_RC_OK) return 0;

   /* Convert value to CIM value */
   value = _CMValueFromChars(valuebuffer, type, &status, _BROKER);
   if (status.rc != CMPI_RC_OK) return 0;

   /* Check if the property array already contains some values */
   olddata = CMGetProperty(_INSTANCE, name, &status);
   if (status.rc == CMPI_RC_OK && !CMIsNullValue(olddata) && CMIsArray(olddata)) {
      oldsize = CMGetArrayCount(olddata.value.array,NULL);
   } else {
      oldsize = 0;
   }

   /* Create a new larger array to store the new value */
   newdata.value.array = CMNewArray(_BROKER, oldsize+1, type, &status);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("setArrayProperty() : Failed to create new property array"));
      return 0;
   }
     
   /* Add the new value to the end of the new array */
   status = CMSetArrayElementAt(newdata.value.array, oldsize, &value, type);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("setArrayProperty() : Failed to set new value in new property array"));
      return 0;
   }

   /* Copy the old array values to the new array */
   for (i=0; i<oldsize; i++) {
      data = CMGetArrayElementAt(olddata.value.array, i, &status);
      if (status.rc != CMPI_RC_OK) {
         _OSBASE_TRACE(1,("setArrayProperty() : Failed to get value from old property array"));
         return 0;
      }

      status = CMSetArrayElementAt(newdata.value.array, i, &(data.value), type); 
      if (status.rc != CMPI_RC_OK) {
         _OSBASE_TRACE(1,("setArrayProperty() : Failed to set value in new property array"));
         return 0;
      }
   }

   /* Determine the CIM array type based on the value type */
   switch (type) {
      case CMPI_string:   arraytype = CMPI_stringA; break;
      case CMPI_dateTime: arraytype = CMPI_dateTimeA; break; 
      case CMPI_boolean:  arraytype = CMPI_booleanA; break;
      case CMPI_char16:   arraytype = CMPI_char16A; break;
      case CMPI_uint8:    arraytype = CMPI_uint8A; break;
      case CMPI_sint8:    arraytype = CMPI_sint8A; break;
      case CMPI_uint16:   arraytype = CMPI_uint16A; break;
      case CMPI_sint16:   arraytype = CMPI_sint16A; break;
      case CMPI_uint32:   arraytype = CMPI_uint32A; break;
      case CMPI_sint32:   arraytype = CMPI_sint32A; break;
      case CMPI_uint64:   arraytype = CMPI_uint64A; break;
      case CMPI_sint64:   arraytype = CMPI_sint64A; break;
      case CMPI_real32:   arraytype = CMPI_real32A; break;
      case CMPI_real64:   arraytype = CMPI_real64A; break;
      default: _OSBASE_TRACE(1,("setArrayProperty() : Unrecognized type CIM type=%d",type)); return 0;
   }

   status = CMSetProperty(_INSTANCE, name, &(newdata.value), arraytype);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("setArrayProperty() : Failed to set array property"));
      return 0;
   }
   return 1;
}

/* ----------------------------------------------------------------------------------- */

/*
 * sameObject
 */
int Linux_NFSv3_sameObject( const CMPIObjectPath * object1, const CMPIObjectPath * object2 )
{
   CMPIData key1, key2;
   CMPIString * keyname = NULL;
   int numkeys1, numkeys2, i;

   /* Check that the two objects have the same number of keys */
   numkeys1 = CMGetKeyCount(object1, NULL);
   numkeys2 = CMGetKeyCount(object2, NULL);
   if (numkeys1 != numkeys2) return 0;

   /* Go through the list of keys for the first object */
   for (i=0; i<numkeys1; i++) {
      /* Retrieve the key from both objects */
      key1 = CMGetKeyAt(object1, i, &keyname, NULL);
      key2 = CMGetKey(object2, CMGetCharPtr(keyname), NULL);

      /* Check that both keys exist and have been set */
      if (CMIsNullValue(key1) || CMIsNullValue(key2)) return 0;

      /* Check that the type of the two keys is the same */
      if (key1.type != key2.type) return 0;

      /* Check that the value of the two keys is the same */
      if (!_CMSameValue(key1,key2)) return 0;
   }

   /* If we get here then all the keys must have matched */
   return 1;
}

/* ----------------------------------------------------------------------------------- */

/*
 * startReadingInstances
 */
void * Linux_NFSv3_startReadingInstances()
{
   FILE * sourcefile;			/* Handle for the original config file */
   _FILEINFO * xmlfileinfo;		/* Filename and handle for the translated XML config file */ 

   /* Read from the original config file */
   if ((sourcefile = fopen(_CONFIGFILE,"r")) == NULL) {
      _OSBASE_TRACE(1,("startReadingInstances() : Cannot read from config file %s", _CONFIGFILE));
      return NULL;
   }

   /* Write a new temp file containing the translated XML config file */
   xmlfileinfo = malloc(sizeof(_FILEINFO));
   tmpnam(xmlfileinfo->name);
   if ((xmlfileinfo->handle = fopen(xmlfileinfo->name,"w")) == NULL) {
      _OSBASE_TRACE(1,("startReadingInstances() : Cannot write to XML config file %s", xmlfileinfo->name));
      fclose(sourcefile);
      free(xmlfileinfo);
      return NULL;
   }
   
   /* Parse the original config file to the temp XML config file */   
   _OSBASE_TRACE(2,("startReadingInstances() : Parsing %s to %s", _CONFIGFILE, xmlfileinfo->name));
   if (NFSv3yyparsefile(sourcefile, xmlfileinfo->handle) != 0) {
      _OSBASE_TRACE(1,("startReadingInstances() : Error parsing config file"));
      fclose(sourcefile);
      fclose(xmlfileinfo->handle);
      free(xmlfileinfo);
      return NULL;
   }

   /* Reopen the translated XML config file to read in XML instance data */
   fclose(sourcefile);
   fclose(xmlfileinfo->handle);
   if ((xmlfileinfo->handle = fopen(xmlfileinfo->name,"r")) == NULL) {
      _OSBASE_TRACE(1,("startReadingInstances() : Cannot read from XML config file %s", xmlfileinfo->name));
      free(xmlfileinfo);
      return NULL;
   }

   /* Set the XML parser to start reading from the XML config file */
   NFSv3xmlyyrestart(xmlfileinfo->handle);
   return xmlfileinfo;
}


/*
 * readNextInstance
 */
int Linux_NFSv3_readNextInstance( void * instances, const CMPIInstance ** instance, const CMPIBroker * broker, const char * namespace )
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* Return status of CIM operations */
   CMPIObjectPath * objectpath;			/* CIM object path of the current instance */
   CMPIValue value;				/* General purpose CIM value */
   struct stat fileinfo;			/* stat() info about the config file */
   int rc;					/* xmlyyparse() return code */

   _BROKER = broker;

   /* Create a new CIM object path for this instance */
   objectpath = CMNewObjectPath(broker, namespace, _SETTINGCLASSNAME, &status);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("readNextInstance() : Failed to create new object path - %s", CMGetCharPtr(status.msg)));
      *instance = NULL;
      return EOF;
   }

   /* Create a new CIM instance for the new object path */
   *instance = CMNewInstance(broker, objectpath, &status);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("readNextInstance() : Failed to create new instance - %s", CMGetCharPtr(status.msg)));
      *instance = NULL;
      return EOF;
   }

   /* Set the initial key properties for the Linux_ConfigFile class */
   /* Note - these values can be over-written by the instance data in the config file */
   CMSetProperty(*instance, "SystemName", get_system_name(), CMPI_chars);
   CMSetProperty(*instance, "SystemCreationClassName", CSCreationClassName, CMPI_chars);
   CMSetProperty(*instance, "CreationClassName", _SETTINGCLASSNAME, CMPI_chars);
   /* Note - the SettingID key property is set by each config file instance */ 
   CMSetProperty(*instance, "Filename", _CONFIGFILE, CMPI_chars);

   /* stat() the config file to set the Readable and Writeable properties */
   if (stat(_CONFIGFILE, &fileinfo) == 0) {
      value.boolean = ((fileinfo.st_mode & S_IRUSR) != 0);
      CMSetProperty(*instance, "Readable", &value, CMPI_boolean);
      value.boolean = ((fileinfo.st_mode & S_IWUSR) != 0);
      CMSetProperty(*instance, "Writeable", &value, CMPI_boolean);
   } else {
      _OSBASE_TRACE(1,("readNextInstance() : Failed to stat() config file"));
   }

   /* Parse the next instance in the config file to set the remaining property values */
   _INSTANCE = *instance; /* _INSTANCE is used by setProperty(), which is called by the external parser */
   rc = NFSv3xmlyyparse();
   if (rc == EOF) {
      _OSBASE_TRACE(1,("readNextInstance() : End of config file"));
      *instance = NULL;
      return EOF;
   } else if (rc != 0) {
      _OSBASE_TRACE(1,("readNextInstance() : Error occurred when parsing next instance"));
      *instance = NULL;
      return 0;
   }
   return 1;
}


/*
 * endReadingInstances
 */
void Linux_NFSv3_endReadingInstances( void * instances )
{
   if (instances != NULL) {
      fclose(((_FILEINFO *)instances)->handle);
      remove(((_FILEINFO *)instances)->name); 
      free(instances);
   }
}

/* ----------------------------------------------------------------------------------- */

/*
 * startWritingInstances
 */
void * Linux_NFSv3_startWritingInstances()
{
   _FILEINFO * newfileinfo;	/* Filename and handle for the new config file */

   /* Open a temp file to contain the new config file */
   newfileinfo = malloc(sizeof(_FILEINFO));
   tmpnam(newfileinfo->name);
   if ((newfileinfo->handle = fopen(newfileinfo->name,"w")) == NULL) {
      _OSBASE_TRACE(1,("startWritingInstances() : Cannot write to new config file %s", newfileinfo->name));
      free(newfileinfo);
      return NULL;
   }
   return newfileinfo;
}


/*
 * writeNextInstance
 */
int Linux_NFSv3_writeNextInstance( void * newinstances, const CMPIInstance * instance )
{
   char * textbuffer;		/* Buffer containing config file text representation of the instance */
   int rc = 0;			/* Return status */

   if (newinstances != NULL) {
      /* Convert the CIM instance to config file formatted text */
      if (Linux_NFSv3_instance2string(instance, &textbuffer)) {
        if (fputs(textbuffer, ((_FILEINFO *)newinstances)->handle) != EOF) rc = 1; 
        free(textbuffer);
      }
   }
   return rc; 
}


/*
 * endWritingInstances
 */
void Linux_NFSv3_endWritingInstances( void * newinstances, int commit )
{
   char * command;			/* Shell 'cp' command */
  
   if ( newinstances != NULL) {
     fclose(((_FILEINFO *)newinstances)->handle);

     if (commit) {
        /* Copy the new config file over the original config file */
        _OSBASE_TRACE(1,("endWritingInstances() : Commiting changes to config file"));
        command = malloc(strlen(((_FILEINFO *)newinstances)->name)+strlen(_CONFIGFILE)+9);
        sprintf(command, "cp -f %s %s\n", ((_FILEINFO *)newinstances)->name, _CONFIGFILE);
        if (system(command) != 0) {
           _OSBASE_TRACE(1,("endWritingInstances() : Failed to overwrite config file with changes"));
	}
        free(command);
     } else {
	_OSBASE_TRACE(1,("endWritingInstances() : Config file unchanged"));
     }

     remove(((_FILEINFO *)newinstances)->name);
     free(newinstances);
   }
}


/*
 * makeConfigInstance
 */
CMPIInstance * Linux_NFSv3_makeConfigInstance( const CMPIBroker * broker, const char * namespace )
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* Return status of CIM operations */
   CMPIObjectPath * objectpath;			/* CIM object path of this instance */
   CMPIInstance * instance;			/* CIM instance for this instance */
   CMPIValue value;				/* General purpose CIM value */
   struct stat fileinfo;			/* stat() info about the config file */

   /* Create a new CIM object path for this instance */
   objectpath = CMNewObjectPath(broker, namespace, _CONFIGCLASSNAME, &status);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("makeConfigInstance() : Failed to create new object path - %s", CMGetCharPtr(status.msg)));
      return NULL;
   }

   /* Create a new CIM instance for the new object path */
   instance = CMNewInstance(broker, objectpath, &status);
   if (status.rc != CMPI_RC_OK) {
      _OSBASE_TRACE(1,("makeConfigInstance() : Failed to create new instance - %s", CMGetCharPtr(status.msg)));
      return NULL;
   }

   /* Set the key properties for this instance */
   CMSetProperty(instance, "SystemName", get_system_name(), CMPI_chars);
   CMSetProperty(instance, "SystemCreationClassName", CSCreationClassName, CMPI_chars);
   CMSetProperty(instance, "CreationClassName", _CONFIGCLASSNAME, CMPI_chars);
   CMSetProperty(instance, "Name", _CONFIGNAME, CMPI_chars);

   /* stat() the config file to set the Readable and Writeable properties */
   CMSetProperty(instance, "Filename", _CONFIGFILE, CMPI_chars);
   if (stat(_CONFIGFILE, &fileinfo) == 0) {
      value.boolean = ((fileinfo.st_mode & S_IRUSR) != 0);
      CMSetProperty(instance, "Readable", &value, CMPI_boolean);
      value.boolean = ((fileinfo.st_mode & S_IWUSR) != 0);
      CMSetProperty(instance, "Writeable", &value, CMPI_boolean);
   } else {
      _OSBASE_TRACE(1,("makeInstance() : Failed to stat() config file"));
   }

   return instance;
}

