/* Required C library headers */
#include <stdio.h>
#include <string.h>

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
#define Linux_NFSv3SettingContextSetInstance Linux_NFSv3SettingContextModifyInstance 
#endif

/* ---------------------------------------------------------------------------
 * EXPORTED LIBRARY FUNCTIONS
 * --------------------------------------------------------------------------- */

/*
 * EnumInstanceNames
 */
CMPIStatus Linux_NFSv3SettingContextEnumInstanceNames(
	CMPIInstanceMI * mi,
	const CMPIContext * context,
	const CMPIResult * results,
	const CMPIObjectPath * reference)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */
   int rc;					/* Utility function return code */

   _OSBASE_TRACE(1,("EnumInstanceNames() called"));

   /* Enumerate the target class using the CMPI base utility function */
   rc = _assoc_create_inst_1toN( _BROKER, context, results, reference,
		_ASSOCCLASSNAME, _CONFIGCLASSNAME, _SETTINGCLASSNAME, "Context", "Setting", 0, 0, &status);
   if (rc != 0) {
      /* Make sure the status reflects the error */
      if (status.rc == CMPI_RC_OK) {  
         CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "_assoc_create_inst_1toN() failed");
      }
      _OSBASE_TRACE(1,("EnumInstanceNames() error - %s", CMGetCharPtr(status.msg)));
   }
   
   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("EnumInstanceNames() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/*
 * EnumInstances
 */
CMPIStatus Linux_NFSv3SettingContextEnumInstances(
	CMPIInstanceMI * mi,
	const CMPIContext * context,
	const CMPIResult * results,
	const CMPIObjectPath * reference,
	const char ** properties)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */
   int rc;					/* Utility function return code */
   
   _OSBASE_TRACE(1,("EnumInstances() called"));

   /* Enumerate the target class using the CMPI base utility function */
   rc = _assoc_create_inst_1toN( _BROKER, context, results, reference,
		_ASSOCCLASSNAME, _CONFIGCLASSNAME, _SETTINGCLASSNAME, "Context", "Setting", 0, 1, &status);
   if (rc != 0) {
      /* Make sure the status reflects the error */
      if (status.rc == CMPI_RC_OK) {
         CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "_assoc_create_inst_1toN() failed");
      }
      _OSBASE_TRACE(1,("EnumInstanceNames() error - %s", CMGetCharPtr(status.msg)));
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("EnumInstances() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/*
 * GetInstance
 */
CMPIStatus Linux_NFSv3SettingContextGetInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * context,
	const CMPIResult * results,
	const CMPIObjectPath * reference,
	const char ** properties)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */
   CMPIInstance * instance;			/* The desired CIM instance */

   _OSBASE_TRACE(1,("GetInstance() called"));

   instance = _assoc_get_inst( _BROKER, context, reference, _ASSOCCLASSNAME, "Context", "Setting", &status);
   if (instance != NULL) {
      CMReturnInstance(results, instance);
   } else {
      /* Make sure the status reflects the error */
      if (status.rc == CMPI_RC_OK) {
	 CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "_assoc_get_inst() failed");
      }
      _OSBASE_TRACE(1,("GetInstance() error - %s", CMGetCharPtr(status.msg)));
   }
   
   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("GetInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/*
 * SetInstance
 */
CMPIStatus Linux_NFSv3SettingContextSetInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * context,
	const CMPIResult * results,
	const CMPIObjectPath * reference,
	const CMPIInstance * newinstance,
	const char **properties)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */

   _OSBASE_TRACE(1,("SetInstance() called"));

   /* SetInstance() is not supported for associations */
   CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_NOT_SUPPORTED, "SetInstance() not supported");
   _OSBASE_TRACE(1,("SetInstance() error - %s", CMGetCharPtr(status.msg)));

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("SetInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/*
 * CreateInstance
 */
CMPIStatus Linux_NFSv3SettingContextCreateInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * context,
	const CMPIResult * results,
	const CMPIObjectPath * reference,
	const CMPIInstance * newinstance)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */

   _OSBASE_TRACE(1,("CreateInstance() called"));

   /* CreateInstance() is not supported for associations */
   CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_NOT_SUPPORTED, "CreateInstance() not supported");
   _OSBASE_TRACE(1,("CreateInstance() error - %s", CMGetCharPtr(status.msg)));

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("CreateInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/*
 * DeleteInstance
 */
CMPIStatus Linux_NFSv3SettingContextDeleteInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * context,
	const CMPIResult * results,
	const CMPIObjectPath * reference)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */

   _OSBASE_TRACE(1,("DeleteInstance() called"));

   /* DeleteInstance() is not supported for associations */
   CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_NOT_SUPPORTED, "DeleteInstance() not supported");
   _OSBASE_TRACE(1,("DeleteInstance() error - %s", CMGetCharPtr(status.msg)));

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("DeleteInstance() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/*
 * ExecQuery
 */
CMPIStatus Linux_NFSv3SettingContextExecQuery(
	CMPIInstanceMI * mi,
	const CMPIContext * context,
	const CMPIResult * results,
	const CMPIObjectPath * reference,
	const char * language,
	const char * query)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */

   _OSBASE_TRACE(1,("ExecQuery() called"));

   /* ExecQuery() is not supported for associations */
   CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_NOT_SUPPORTED, "ExecQuery() not supported");
   _OSBASE_TRACE(1,("ExecQuery() error - %s", CMGetCharPtr(status.msg)));

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("ExecQuery() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/*
 * Cleanup
 */
CMPIStatus Linux_NFSv3SettingContextCleanup(
       	CMPIInstanceMI * mi,
	const CMPIContext * context,
	CMPIBoolean terminate)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};	/* CIM return status */

   _OSBASE_TRACE(1,("Cleanup() called"));

   /* PUT ANY CLEANUP CODE HERE */

   /* Finished */
   _OSBASE_TRACE(1,("Cleanup() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}

/* --------------------------------------------------------------------------- */

/*
 * AssociatorNames
 */
CMPIStatus Linux_NFSv3SettingContextAssociatorNames(
		CMPIAssociationMI * mi,
		const CMPIContext * context,
		const CMPIResult * results,
		const CMPIObjectPath * reference,
		const char * assocClass,
		const char * resultClass,
		const char * role,
		const char * resultRole )
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   CMPIObjectPath * objectpath;
   const char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   char * sourceclass = CMGetCharPtr(CMGetClassName(reference, &status)); /* Class of the reference object */
   void * instances;                            /* Handle for the list of instances */
   int count = 0;                               /* Number of instances found */
   
   _OSBASE_TRACE(1,("AssociatorNames() called"));

   /* Check if the source class is a Linux_SystemConfiguration or a Linux_SystemSetting */
   if (strcmp(sourceclass, _SETTINGCLASSNAME) == 0) {
      /* If Linux_SystemSetting then return the Linux_SystemConfiguration */
      instance = Linux_NFSv3_makeConfigInstance(_BROKER, namespace);
      if (instance != NULL) {
         objectpath = CMGetObjectPath(instance, NULL);
         CMSetNameSpace(objectpath, namespace);
         CMReturnObjectPath(results, objectpath);
      } else {
	 _OSBASE_TRACE(1,("AssociatorNames() : Failed to create configuration instance"));
	 CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get create configuration instance");
      }
   }
   else if (strcmp(sourceclass, _CONFIGCLASSNAME) == 0) {
      /* If Linux_SystemConfiguration then return *all* its Linux_SystemSettings */
      /* NOTE - This is identical to Linux_SystemSettingEnumInstanceNames! */
  
      /* Get the list of instances */
      if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
         _OSBASE_TRACE(1,("AssociatorNames() : Failed to get list of instances"));
         CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get list of instances");
      }
  
      /* Enumerate all the instances and return a new CIM object path for each */
      while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
         if (instance != NULL) {
            /* Return the object path of this instance */
            count++;
            _OSBASE_TRACE(1,("AssociatorNames() : Adding object path #%d to results", count));
            objectpath = CMGetObjectPath(instance, NULL);
            CMSetNameSpace(objectpath, namespace);
            CMReturnObjectPath(results, objectpath);
         }
      }
      Linux_NFSv3_endReadingInstances(instances);

      /* Check if found any instances */
      if (count == 0) {
         _OSBASE_TRACE(1,("AssociatorNames() : No instances found"));
         //CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "No instances found");
      }
   }
   else {
      _OSBASE_TRACE(1,("AssociatorNames() : Unrecognized reference class %s", sourceclass));
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("AssociatorNames() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * Associators
 */
CMPIStatus Linux_NFSv3SettingContextAssociators(
		CMPIAssociationMI * mi,
		const CMPIContext * context, 
		const CMPIResult * results,
		const CMPIObjectPath * reference,
		const char * assocClass,
		const char * resultClass,
		const char * role,
		const char * resultRole,
		const char ** propertyList )
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   const char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   char * sourceclass = CMGetCharPtr(CMGetClassName(reference, &status)); /* Class of the reference object */
   void * instances;                            /* Handle for the list of instances */
   int count = 0;                               /* Number of instances found */

   _OSBASE_TRACE(1,("Associators() called"));

   /* Check if the source class is a Linux_SystemConfiguration or a Linux_SystemSetting */
   if (strcmp(sourceclass, _SETTINGCLASSNAME) == 0) {
      /* If Linux_SystemSetting then return the Linux_SystemConfiguration */
      instance = Linux_NFSv3_makeConfigInstance(_BROKER, namespace);
      if (instance != NULL) {
         CMReturnInstance(results, instance);
      } else {
         _OSBASE_TRACE(1,("Associators() : Failed to create configuration instance"));
         CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get create configuration instance");
      }
   }
   else if (strcmp(sourceclass, _CONFIGCLASSNAME) == 0) {
      /* If Linux_SystemConfiguration then return *all* its Linux_SystemSettings */
      /* NOTE - This is identical to Linux_SystemSettingEnumInstances! */
     
      /* Get the list of instances */
      if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
         _OSBASE_TRACE(1,("Associators() : Failed to get list of instances"));
         CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get list of instances");
      }

      /* Enumerate all the instances and return a new CIM instance for each */
      while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
         if (instance != NULL) {
            /* Return this instance */
            count++;
            _OSBASE_TRACE(1,("Associators() : Adding object path #%d to results", count));
            CMReturnInstance(results, instance);
         }
      }
      Linux_NFSv3_endReadingInstances(instances);

      /* Check if found any instances */
      if (count == 0) {
         _OSBASE_TRACE(1,("Associators() : No instances found"));
         //CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "No instances found");
      }
   }
   else {
      _OSBASE_TRACE(1,("Associators() : Unrecognized reference class %s", sourceclass));
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("Associators() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * ReferenceNames
 */
CMPIStatus Linux_NFSv3SettingContextReferenceNames(
		CMPIAssociationMI * mi,
                const CMPIContext * context,
                const CMPIResult * results,
                const CMPIObjectPath * reference,
                const char * assocClass,
                const char * role)
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   CMPIObjectPath * objectpath;			/* CIM object path for each new instance */
   CMPIObjectPath * refobjectpath;		/* CIM object path for each new reference */
   const char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   char * sourceclass = CMGetCharPtr(CMGetClassName(reference, &status)); /* Class of the reference object */
   void * instances;                            /* Handle for the list of instances */
   int count = 0;                               /* Number of instances found */

   _OSBASE_TRACE(1,("ReferenceNames() called"));

   /* Check if the source class is a Linux_SystemConfiguration or a Linux_SystemSetting */
   if (strcmp(sourceclass, _SETTINGCLASSNAME) == 0) {
      /* If Linux_SystemSetting then return the Linux_SystemConfiguration */
      instance = Linux_NFSv3_makeConfigInstance(_BROKER, namespace);
      if (instance != NULL) {
          /* Create a new reference object path for this association */
          refobjectpath = CMNewObjectPath(_BROKER, namespace, _ASSOCCLASSNAME, &status);
	  objectpath = CMGetObjectPath(instance, NULL);
          CMSetNameSpace(objectpath, namespace);
	  CMAddKey(refobjectpath, "Context", &objectpath, CMPI_ref);
          CMAddKey(refobjectpath, "Setting", &reference, CMPI_ref);
          CMReturnObjectPath(results, refobjectpath);
      } else {
         _OSBASE_TRACE(1,("ReferenceNames() : Failed to create configuration instance"));
         CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get create configuration instance");
      }
   }
  else if (strcmp(sourceclass, _CONFIGCLASSNAME) == 0) {
      /* If Linux_SystemConfiguration then return *all* its Linux_SystemSettings */

      /* Get the list of instances */
      if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
         _OSBASE_TRACE(1,("ReferenceNames() : Failed to get list of instances"));
         CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get list of instances");
      }

      /* Enumerate all the instances and return a new CIM reference for each */
      while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
         if (instance != NULL) {
            /* Return the object path of this instance */
            count++;
            _OSBASE_TRACE(1,("ReferenceNames() : Adding reference #%d to results", count));
	    refobjectpath = CMNewObjectPath(_BROKER, namespace, _ASSOCCLASSNAME, &status);
	    CMAddKey(refobjectpath, "Context", &reference, CMPI_ref);
	    objectpath = CMGetObjectPath(instance, NULL);
            CMSetNameSpace(objectpath, namespace);
	    CMAddKey(refobjectpath, "Setting", &objectpath, CMPI_ref);
	    CMReturnObjectPath(results, refobjectpath);
         }
      }
      Linux_NFSv3_endReadingInstances(instances);

      /* Check if found any instances */
      if (count == 0) {
	 _OSBASE_TRACE(1,("ReferenceNames() : No references found"));
	 //CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "No references found");
      }
   }
   else {
      _OSBASE_TRACE(1,("ReferenceNames() : Unrecognized reference class %s", sourceclass));
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("ReferenceNames() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * References
 */
CMPIStatus Linux_NFSv3SettingContextReferences(
		CMPIAssociationMI * mi,
		const CMPIContext * context,
		const CMPIResult * results,
		const CMPIObjectPath * reference,
		const char * assocClass,
		const char * role,
		const char ** propertyList )
{
   CMPIStatus status = {CMPI_RC_OK, NULL};      /* CIM return status */
   const CMPIInstance * instance;               /* CIM instance for each new instance */
   CMPIObjectPath * objectpath;                 /* CIM object path for each new instance */
   CMPIInstance * refinstance;			/* CIM instance for each new reference */
   char * namespace = CMGetCharPtr(CMGetNameSpace(reference, NULL)); /* Current CIM namespace */
   char * sourceclass = CMGetCharPtr(CMGetClassName(reference, &status)); /* Class of the reference object */
   void * instances;                            /* Handle for the list of instances */
   int count = 0;                               /* Number of instances found */

   _OSBASE_TRACE(1,("References() called"));

   /* Check if the source class is a Linux_SystemConfiguration or a Linux_SystemSetting */
   if (strcmp(sourceclass, _SETTINGCLASSNAME) == 0) {
      /* If Linux_SystemSetting then return the Linux_SystemConfiguration */
      instance = Linux_NFSv3_makeConfigInstance(_BROKER, namespace);
      if (instance != NULL) {
         /* Create a new reference object path for this association */
         refinstance = CMNewInstance(_BROKER, CMNewObjectPath(_BROKER, namespace, _ASSOCCLASSNAME, &status), &status);
         objectpath = CMGetObjectPath(instance, NULL);
         CMSetNameSpace(objectpath, namespace);
         CMSetProperty(refinstance, "Context", &objectpath, CMPI_ref);
         CMSetProperty(refinstance, "Setting", &reference, CMPI_ref);
         CMReturnInstance(results, refinstance);
      } else {
         _OSBASE_TRACE(1,("References() : Failed to create configuration instance"));
         CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get create configuration instance");
      }
   }
   else if (strcmp(sourceclass, _CONFIGCLASSNAME) == 0) {
      /* If Linux_SystemConfiguration then return *all* its Linux_SystemSettings */

      /* Get the list of instances */
      if ((instances = Linux_NFSv3_startReadingInstances()) == NULL) {
         _OSBASE_TRACE(1,("References() : Failed to get list of instances"));
         CMReturnWithChars(_BROKER, CMPI_RC_ERR_FAILED, "Failed to get list of instances");
      }

      /* Enumerate all the instances and return a new CIM reference for each */
      while (Linux_NFSv3_readNextInstance(instances, &instance, _BROKER, namespace) != EOF) {
         if (instance != NULL) {
            /* Return the object path of this instance */
            count++;
            _OSBASE_TRACE(1,("References() : Adding reference #%d to results", count));
            refinstance = CMNewInstance(_BROKER, CMNewObjectPath(_BROKER, namespace, _ASSOCCLASSNAME, &status), &status);
            CMSetProperty(refinstance, "Context", &reference, CMPI_ref);
            objectpath = CMGetObjectPath(instance, NULL);
            CMSetNameSpace(objectpath, namespace);
            CMSetProperty(refinstance, "Setting", &objectpath, CMPI_ref);
            CMReturnInstance(results, refinstance);
         }
      }
      Linux_NFSv3_endReadingInstances(instances);

      /* Check if found any instances */
      if (count == 0) {
         _OSBASE_TRACE(1,("References() : No references found"));
         //CMSetStatusWithChars(_BROKER, &status, CMPI_RC_ERR_FAILED, "No references found");
      }
   }
   else {
      _OSBASE_TRACE(1,("References() : Unrecognized reference class %s", sourceclass));
   }

   /* Finished */
   CMReturnDone(results);
   _OSBASE_TRACE(1,("References() %s",(status.rc == CMPI_RC_OK)? "succeeded":"failed"));
   return status;
}


/*
 * AssociationCleanup
 */
CMPIStatus Linux_NFSv3SettingContextAssociationCleanup(
		CMPIAssociationMI * mi,
		const CMPIContext * context,
		CMPIBoolean terminate)
{
   /* Nothing needs to be done for cleanup */
   _OSBASE_TRACE(1,("AssociationCleanup() called"));
   _OSBASE_TRACE(1,("AssociationCleanup() succeeded"));
   CMReturn(CMPI_RC_OK);
}

/* --------------------------------------------------------------------------- */

/*
 *  Factory methods - PUT THE PROVIDER ASSOCCLASSNAME HERE!
 */
CMInstanceMIStub(Linux_NFSv3SettingContext , Linux_NFSv3SettingContext, _BROKER, CMNoHook);
CMAssociationMIStub(Linux_NFSv3SettingContext , Linux_NFSv3SettingContext, _BROKER, CMNoHook);

