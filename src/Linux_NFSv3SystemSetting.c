/* ---------------------------------------------------------------------------
 * Linux_SystemSetting.c
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
 * Description:	Linux_SystemSetting instance provider source code template
 * Interface:   Common Manageability Programming Interface (CMPI)
 *
 * iMPORTANT: This file must be renamed to the desired new
 * Linux_SystemSetting instance provider souce code file; e.g.
 * 	mv Linux_SystemSetting.c Linux_NFSv3SystemSetting.c
 * Then put the new classname into the CMInstanceMIStub() at the bottom 
 * of this file. NO OTHER CHANGES ARE NECESSARY TO CUSTOMIZE THIS FILE!
 * --------------------------------------------------------------------------- */

/* Required CMPI library headers */
#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"
#include "OSBase_Common.h"
#include "cmpiOSBase_Common.h"

/* Required provider headers */
#include "util/Linux_SystemConfigurationUtil.h"	
#include "GLOBALS.h"


/* ---------------------------------------------------------------------------
 * GLOBAL DEFINITIONS
 * --------------------------------------------------------------------------- */

static const CMPIBroker * _BROKER;

/* ---------------------------------------------------------------------------*//* private declarations                                                       */#ifdef CMPI_VER_100
#define Linux_NFSv3SystemSettingSetInstance Linux_NFSv3SystemSettingModifyInstance
#endif

/* ---------------------------------------------------------------------------
 * EXPORTED LIBRARY FUNCTIONS
 * --------------------------------------------------------------------------- */

/*
 * EnumInstanceNames
 */
CMPIStatus Linux_NFSv3SystemSettingEnumInstanceNames(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   CMPIObjectPath * objectpath;			/* CIM object path of each new instance */
   void * instances;				/* Handle for the list of instances */
   char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   int count = 0;				/* Number of instances found */

   _OSBASE_TRACE(1,("EnumInstanceNames() called"));

   /* Get the list of instances */
   if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
      _OSBASE_TRACE(1,("EnumInstanceNames() : Failed to get list of instances"));
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get list of instances");
   }

   /* Enumerate all the instances and return a new CIM object path for each */
   while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
      if (instance != NULL) {
         /* Return the object path of this instance */
	 count++;
	 _OSBASE_TRACE(1,("EnumInstanceNames() : Adding object path #%d to results", count));
         /* CMReturnObjectPath(results, CMGetObjectPath(instance, NULL)); */
	 objectpath = CMGetObjectPath(instance, NULL);
	 CMSetNameSpace(objectpath, namespace);
	 CMReturnObjectPath(results, objectpath); 
      }
   }
   Linux_NFSv3_endReadingInstances(instances);

   /* Check if found any instances */
   if (count == 0) {
      _OSBASE_TRACE(1,("EnumInstanceNames() : No instances found"));
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("EnumInstanceNames() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * EnumInstances
 */
CMPIStatus Linux_NFSv3SystemSettingEnumInstances(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference, 
		const char ** properties) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;		/* CIM instance for each new instance */
   void * instances;				/* Handle for the list of instances */
   char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   int count = 0;                               /* Number of instances found */

   _OSBASE_TRACE(1,("EnumInstances() called"));

   /* Get the list of instances */
   if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
      _OSBASE_TRACE(1,("EnumInstances() : Failed to get list of instances"));
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get list of instances");
   }

   /* Enumerate all the instances and return a new CIM instance for each */
   while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
      if (instance != NULL) {
	 /* Return this instance */
	 count++;
         _OSBASE_TRACE(1,("EnumInstances() : Adding instance #%d to results", count));
         CMReturnInstance(results, instance);
      }
   }
   Linux_NFSv3_endReadingInstances(instances);

   /* Check if found any instances */
   if (count == 0) {
      _OSBASE_TRACE(1,("EnumInstances() : No instances found"));
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("EnumInstances() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * GetInstance
 */
CMPIStatus Linux_NFSv3SystemSettingGetInstance(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference, 
		const char ** properties) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;		/* CIM instance for each new instance */
   void * instances;                            /* Handle for the list of instances */
   char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   int found = 0;				/* Does the reference object actually exist */

   _OSBASE_TRACE(1,("GetInstance() called"));

   /* Get the list of instances */
   if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
      _OSBASE_TRACE(1,("GetInstance() : Failed to get list of instances"));
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get list of instances");
   }

   /* Enumerate all the instances until we find the desired CIM instance */
   while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
      if (instance != NULL) {
	 /* Check if this instance matches the desired instance */
         if (Linux_NFSv3_sameObject(CMGetObjectPath(instance, NULL), reference)) {
            /* Return this instance */
            _OSBASE_TRACE(1,("GetInstance() : Adding instance to results"));
            CMReturnInstance(results, instance);
            found = 1;
  	    break;
         }
      } 
   }
   Linux_NFSv3_endReadingInstances(instances);

   /* Check if found the desired instance */
   if (!found) {
      _OSBASE_TRACE(1,("GetInstance() : Instance not found"));
      CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Instance not found");
   } 

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("GetInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * SetInstance
 */
CMPIStatus Linux_NFSv3SystemSettingSetInstance(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference,
		const CMPIInstance * newinstance, 
		const char **properties) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   void * instances;                            /* Handle for the old list of instances */
   void * newinstances;				/* Handle for the new list of instances */
   char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   int found = 0;				/* Does the reference object actually exist */

   if (_READONLY) {
      /* Cannot save changes to instances */
      CMReturn(CMPI_RC_ERR_NOT_SUPPORTED);
   }

   _OSBASE_TRACE(1,("SetInstance() called"));

   /* Get the old list of instances */
   if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
      _OSBASE_TRACE(1,("SetInstance() : Failed to get old list of instances"));
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get old list of instances");
   }

   /* Start writing a new list of instances */
   if ((newinstances = Linux_NFSv3_startWritingInstances()) == NULL) {
      _OSBASE_TRACE(1,("SetInstance() : Failed to start new list of instances"));
      Linux_NFSv3_endReadingInstances(instances);
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to start new list of instances");
   }

   /* Enumerate all the old instances and copy them to the new instances */
   while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
      if (instance != NULL) {
	 /* Check if this instance matches the desired instance */
         if (Linux_NFSv3_sameObject(CMGetObjectPath(instance, NULL), reference)) {
            /* Replace the old instance with the new instance */
	    instance = newinstance;
	    found = 1;
         }

         /* Copy the instance to the list of new instances */
         if (!Linux_NFSv3_writeNextInstance(newinstances, instance)) {
            _OSBASE_TRACE(1,("SetInstance() : Failed to write instance"));
            CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Failed to write instance");
	    break;
	 }
      } 
   }
   Linux_NFSv3_endReadingInstances(instances);

   /* Check if found the desired instance */
   if ((status.rc == CMPI_RC_OK) && !found) {
      _OSBASE_TRACE(1,("SetInstance() : Instance not found"));
      CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Instance not found");
   }

   /* Commit the changes */
   Linux_NFSv3_endWritingInstances(newinstances, (status.rc == CMPI_RC_OK));

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("SetInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * CreateInstance
 */
CMPIStatus Linux_NFSv3SystemSettingCreateInstance(
                CMPIInstanceMI * mi,
                const CMPIContext * context,
                const CMPIResult * results,
                const CMPIObjectPath * reference,
                const CMPIInstance * newinstance)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   CMPIObjectPath * newobjectpath;
   void * instances;                            /* Handle for the old list of instances */
   void * newinstances;                         /* Handle for the new list of instances */
   char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   int found = 0;                               /* Does the reference object already exist */
   CMPIData data;

   if (_READONLY) {
      /* Cannot create new instances */
      CMReturn(CMPI_RC_ERR_NOT_SUPPORTED);
   }

   _OSBASE_TRACE(1,("CreateInstance() called"));

   /* Create a valid object path for this new instance */
   newobjectpath = CMNewObjectPath(_BROKER, namespace, _SETTINGCLASSNAME, &status);
   CMAddKey(newobjectpath, "SystemName", get_system_name(), CMPI_chars);
   CMAddKey(newobjectpath, "SystemCreationClassName", CSCreationClassName, CMPI_chars);
   CMAddKey(newobjectpath, "CreationClassName", _SETTINGCLASSNAME, CMPI_chars);
   data = CMGetProperty(newinstance, "SettingID", NULL);
   CMAddKey(newobjectpath, "SettingID", CMGetCharPtr(data.value.string), CMPI_chars);
   _OSBASE_TRACE(1,("CreateInstance() : creating new instance '%s'",CMGetCharPtr(CDToString(_BROKER, newobjectpath, NULL))));
   
   /* Get the old list of instances */
   if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
      _OSBASE_TRACE(1,("CreateInstance() : Failed to get old list of instances"));
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get old list of instances");
   }

   /* Start writing a new list of instances */
   if ((newinstances = Linux_NFSv3_startWritingInstances()) == NULL) {
      _OSBASE_TRACE(1,("CreateInstance() : Failed to start new list of instances"));
      Linux_NFSv3_endReadingInstances(instances);
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to start new list of instances");
   }

   /* Enumerate all the old instances and copy them to the new instances */
   while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
      if (instance != NULL) {
         /* Check if this instance matches the desired instance */
         if (Linux_NFSv3_sameObject(CMGetObjectPath(instance, &status), newobjectpath)) {
            /* New instance already exists! */
	    found = 1;
	    break;
         }

         /* Copy the instance to the list of new instances */
         if (!Linux_NFSv3_writeNextInstance(newinstances, instance)) { 
            _OSBASE_TRACE(1,("CreateInstance() : Failed to write instance"));
            CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Failed to write instance");
	    break;
	 }
      }
   }
   Linux_NFSv3_endReadingInstances(instances);

   /* Check if found the desired instance */
   if ((status.rc == CMPI_RC_OK) && found) {
      _OSBASE_TRACE(1,("SetInstance() : Instance already exists"));
      CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Instance already exists");
   }

   /* Write the new instance to the list of new instances */
   if ((status.rc == CMPI_RC_OK) && !Linux_NFSv3_writeNextInstance(newinstances, newinstance)) {
      _OSBASE_TRACE(1,("CreateInstance() : Failed to write new instance"));
      CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Failed to write new instance");
   }

   /* Commit the changes */
   Linux_NFSv3_endWritingInstances(newinstances, (status.rc == CMPI_RC_OK));

   /* Return the object path of the new instance */
   if (status.rc == CMPI_RC_OK) {
      CMReturnObjectPath(results, newobjectpath);
   }
      
   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("CreateInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * DeleteInstance
 */
CMPIStatus Linux_NFSv3SystemSettingDeleteInstance(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   void * instances;                            /* Handle for the old list of instances */
   void * newinstances;                         /* Handle for the new list of instances */
   char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   int found = 0;                               /* Does the reference object actually exist */

   if (_READONLY) {
      /* Cannot delete existing instances */
      CMReturn(CMPI_RC_ERR_NOT_SUPPORTED);
   }

   _OSBASE_TRACE(1,("DeleteInstance() called"));

   /* Get the old list of instances */
   if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
      _OSBASE_TRACE(1,("DeleteInstance() : Failed to get old list of instances"));
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get old list of instances");
   }

   /* Start a new list of instances */
   if ((newinstances = Linux_NFSv3_startWritingInstances()) == NULL) {
      _OSBASE_TRACE(1,("DeleteInstance() : Failed to start new list of instances"));
      Linux_NFSv3_endReadingInstances(instances);
      CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to start new list of instances");
   }

   /* Enumerate all the old instances and copy them to the new instances */
   while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
      if (instance != NULL) {
         /* Check if this instance matches the desired instance */
         if (Linux_NFSv3_sameObject(CMGetObjectPath(instance, &status), reference)) {
	    /* Skip copying this instance */
            found = 1;
	    continue;
         }
           
	 /* Copy the instance to the list of new instances */
         if (!Linux_NFSv3_writeNextInstance(newinstances, instance)) {
            _OSBASE_TRACE(1,("DeleteInstance() : Failed to write instance"));
            CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Failed to write instance");
	    break;
         }
      }
   }
   Linux_NFSv3_endReadingInstances(instances);

   /* Check if found the desired instance */
   if ((status.rc == CMPI_RC_OK) && !found) {
      _OSBASE_TRACE(1,("DeleteInstance() : Instance not found"));
      CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "Instance not found");
   }

   /* Commit the changes */
   Linux_NFSv3_endWritingInstances(newinstances, (status.rc == CMPI_RC_OK));

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("DeleteInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * ExecQuery
 */
CMPIStatus Linux_NFSv3SystemSettingExecQuery(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference, 
		const char * language, 
		const char * query) 
{
   /* Cannot execute queries against instances */
   CMReturn(CMPI_RC_ERR_NOT_SUPPORTED);
}


/*
 * Cleanup
 */
CMPIStatus Linux_NFSv3SystemSettingCleanup(
                CMPIInstanceMI * mi,
                const CMPIContext * context,
		CMPIBoolean terminate)
{
   /* Nothing needs to be done for cleanup */
   CMReturn(CMPI_RC_OK);
}


/*
 * Factory method - PUT THE PROVIDER SETTINGCLASSNAME HERE!
 */
CMInstanceMIStub(Linux_NFSv3SystemSetting , Linux_NFSv3SystemSetting, _BROKER, CMNoHook);

