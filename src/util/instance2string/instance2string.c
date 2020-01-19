#include <string.h>

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"
#include "OSBase_Common.h"
#include "cmpiOSBase_Common.h"

#define _MAXINSTANCELENGTH 1024

/*
 * instance2string
 */
int Linux_NFSv3_instance2string (const CMPIInstance * instance, char ** buffer)
{
   CMPIData data;			/* General purpose CIM data storage for CIM property values */
   char string[_MAXINSTANCELENGTH];	/* General purpose string buffer for formatting values */
   char * str;				/* General purpose string pointer */

   /* Create a text buffer to hold the new config file entry */
   *buffer = malloc(_MAXINSTANCELENGTH);
   strcpy(*buffer, "");

   /* First write out any comments associated with this instance */
   data = CMGetProperty(instance, "Description", NULL);
   if (!CMIsNullValue(data)) {
      str = CMGetCharPtr(data.value.string);
      if (str[0] != '#') strcat(*buffer, "# "); /* Make sure the description becomes a comment */
      strcat(strcat(*buffer, str), "\n");
   }

   /* Write out the config file entry values for this instance */

   data = CMGetProperty(instance, "Directory", NULL);
   if (!CMIsNullValue(data)) {
      strcat(strcat(*buffer, CMGetCharPtr(data.value.string)), "\t");
   } else {
      /* If the Directory attribute is not set then get it from the SettingID instead */
      data = CMGetProperty(instance, "SettingID", NULL);
      str = index(CMGetCharPtr(data.value.string),':');
      strcat(strcat(*buffer, str+1), "\t");
   }

   data = CMGetProperty(instance, "RemoteHost", NULL);
   if (!CMIsNullValue(data)) {
      strcat(*buffer, CMGetCharPtr(data.value.string));
   } else {
      /* If the RemoteHost attribute is not set then get it from the SettingID instead */
      data = CMGetProperty(instance, "SettingID", NULL);
      str = index(CMGetCharPtr(data.value.string),':'); 
      *str = '\0';
      if (strlen(CMGetCharPtr(data.value.string)) > 0) {
         strcat(*buffer, CMGetCharPtr(data.value.string));
      }
   }
   strcat(*buffer, "(");

   data = CMGetProperty(instance, "Permission", NULL);
   if (!CMIsNullValue(data)) {
      str = CMGetCharPtr(data.value.string);
      if (*str != '\0') {
         if (strcmp(str,"rw") && strcmp(str,"ro")) {
            _OSBASE_TRACE(1,("instance2string() : Invalid property value: Permission='%s'",str));
            return 0;
         }
         strcat(strcat(*buffer, CMGetCharPtr(data.value.string)), ",");
      }
   }

   data = CMGetProperty(instance, "Secure", NULL);
   if (!CMIsNullValue(data)) strcat(strcat(*buffer, data.value.boolean? "secure":"insecure"), ","); 

   data = CMGetProperty(instance, "Squash", NULL);
   if (!CMIsNullValue(data)) {
      str = CMGetCharPtr(data.value.string);
      if (*str != '\0') {
         if (strcmp(str,"root_squash") && strcmp(str,"no_root_squash") && strcmp(str,"all_squash")) {
	    _OSBASE_TRACE(1,("instance2string() : Invalid property value: Squash='%s'",str));
	    return 0;
         }
         strcat(strcat(*buffer, CMGetCharPtr(data.value.string)), ",");
      }
   }

   data = CMGetProperty(instance, "Sync", NULL);
   if (!CMIsNullValue(data)) strcat(strcat(*buffer, data.value.boolean? "sync":"async"), ",");
      
   data = CMGetProperty(instance, "Delay", NULL);
   if (!CMIsNullValue(data)) strcat(strcat(*buffer, data.value.boolean? "wdelay":"no_wdelay"), ",");

   data = CMGetProperty(instance, "Hide", NULL);
   if (!CMIsNullValue(data)) strcat(strcat(*buffer, data.value.boolean? "hide":"nohide"), ",");
     
   data = CMGetProperty(instance, "SubtreeCheck", NULL);
   if (!CMIsNullValue(data)) strcat(strcat(*buffer, data.value.boolean? "subtree_check":"no_subtree_check"), ",");
    
   data = CMGetProperty(instance, "SecureLocks", NULL);
   if (!CMIsNullValue(data)) strcat(strcat(*buffer, data.value.boolean? "secure_locks":"insecure_locks"), ",");

   data = CMGetProperty(instance, "AnonUID", NULL);
   if (!CMIsNullValue(data)) {
      sprintf(string, "anonuid=%u", data.value.uint16);
      strcat(strcat(*buffer, string), ",");
   }

   data = CMGetProperty(instance, "AnonGID", NULL);
   if (!CMIsNullValue(data)) {
      sprintf(string, "anongid=%u", data.value.uint16);
      strcat(strcat(*buffer, string), ",");
   }

   if ((*buffer)[strlen(*buffer)-1] == ',') (*buffer)[strlen(*buffer)-1] = '\0'; /* remove last comma */
   strcat(*buffer, ")\n");

   _OSBASE_TRACE(1,("instance2string() : New instance entry is\nSTART-->%s<--END", *buffer));
   return 1;
}

