/* ---------------------------------------------------------------------------
 * Linux_SystemConfiguration.c
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
 * Description:	Linux_SystemConfiguration instance provider source code template
 * Interface:   Common Manageability Programming Interface (CMPI)
 *
 * IMPORTANT: This file must be renamed to the desired new
 * Linux_SystemConfiguration instance provider souce code file; e.g.
 *      mv Linux_SystemConfiguration.c Linux_NFSv3SystemConfiguration.c
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
#define Linux_NFSv3SystemConfigurationSetInstance Linux_NFSv3SystemConfigurationModifyInstance
#endif

/* ---------------------------------------------------------------------------
 * EXPORTED LIBRARY FUNCTIONS
 * --------------------------------------------------------------------------- */

/*
 * EnumInstanceNames
 */
CMPIStatus Linux_NFSv3SystemConfigurationEnumInstanceNames(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   CMPIInstance * instance;                     /* CIM instance of new instance */
   CMPIObjectPath * objectpath;			/* CIM object path of each new instance */

   _OSBASE_TRACE(1,("EnumInstanceNames() called"));

   /* Create a new instance for this Linux_SystemConfiguration */
   instance = Linux_NFSv3_makeConfigInstance(_BROKER, namespace);
   if (instance != NULL) {
      /* Return the object path of this instance */
      _OSBASE_TRACE(1,("EnumInstanceNames() : Adding object path to results"));
      /* CMReturnObjectPath(results, CMGetObjectPath(instance, NULL)); */
      objectpath = CMGetObjectPath(instance, NULL);
      CMSetNameSpace(objectpath, namespace);
      CMReturnObjectPath(results, objectpath);
   } else {
      status.rc = CMPI_RC_ERR_FAILED;
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("EnumInstanceNames() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * EnumInstances
 */
CMPIStatus Linux_NFSv3SystemConfigurationEnumInstances(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference, 
		const char ** properties) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   CMPIInstance * instance;			/* CIM instance of new instance */

   _OSBASE_TRACE(1,("EnumInstances() called"));

   /* Create a new instance for this Linux_SystemConfiguration */
   instance = Linux_NFSv3_makeConfigInstance(_BROKER, namespace);
   if (instance != NULL) {
      /* Return this instance */
      _OSBASE_TRACE(1,("EnumInstanceNames() : Adding instance to results"));
      CMReturnInstance(results, instance);
   } else {
      status.rc = CMPI_RC_ERR_FAILED;
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("EnumInstanceNames() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * GetInstance
 */
CMPIStatus Linux_NFSv3SystemConfigurationGetInstance(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference, 
		const char ** properties) 
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   CMPIInstance * instance;			/* CIM instance of new instance */

   _OSBASE_TRACE(1,("GetInstance() called"));

   /* Create a new instance for this Linux_SystemConfiguration */
   instance = Linux_NFSv3_makeConfigInstance(_BROKER, namespace);

   /* Check if this instance matches the desired instance (always should!) */
   if (instance != NULL && Linux_NFSv3_sameObject(CMGetObjectPath(instance, NULL), reference)) {
      /* Return this instance */
      _OSBASE_TRACE(1,("GetInstance() : Adding instance to results"));
      CMReturnInstance(results, instance);
   } else {
      status.rc = CMPI_RC_ERR_FAILED;
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("GetInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * SetInstance
 */
CMPIStatus Linux_NFSv3SystemConfigurationSetInstance(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference,
		const CMPIInstance * newinstance, 
		const char **properties) 
{
   /* Cannot modify instances */
   CMReturn(CMPI_RC_ERR_NOT_SUPPORTED);
}


/*
 * CreateInstance
 */
CMPIStatus Linux_NFSv3SystemConfigurationCreateInstance(
                CMPIInstanceMI * mi,
                const CMPIContext * context,
                const CMPIResult * results,
                const CMPIObjectPath * reference,
                const CMPIInstance * newinstance)
{
   /* Cannot create new instances */
   CMReturn(CMPI_RC_ERR_NOT_SUPPORTED);
}


/*
 * DeleteInstance
 */
CMPIStatus Linux_NFSv3SystemConfigurationDeleteInstance(
		CMPIInstanceMI * mi, 
		const CMPIContext * context, 
		const CMPIResult * results, 
		const CMPIObjectPath * reference) 
{
   /* Cannot delete instances */
   CMReturn(CMPI_RC_ERR_NOT_SUPPORTED);
}


/*
 * ExecQuery
 */
CMPIStatus Linux_NFSv3SystemConfigurationExecQuery(
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
CMPIStatus Linux_NFSv3SystemConfigurationCleanup(
                CMPIInstanceMI * mi,
                const CMPIContext * context,
		CMPIBoolean terminate)
{
   /* Nothing needs to be done for cleanup */
   CMReturn(CMPI_RC_OK);
}


/*
 * Factory method - PUT THE PROVIDER CONFIGCLASSNAME HERE!
 */
CMInstanceMIStub(Linux_NFSv3SystemConfiguration , Linux_NFSv3SystemConfiguration, _BROKER, CMNoHook);

