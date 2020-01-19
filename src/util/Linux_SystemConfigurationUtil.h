/* ---------------------------------------------------------------------------
 * Linux_SystemConfigurationUtil.h
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
 * Description:	Utility functions for Linux_SystemConfiguration providers
 * Interface:	Common Manageability Programming Interface (CMPI)
 *
 * IMPORTANT: DO NOT RENAME OR MODIFY THIS FILE!
 * --------------------------------------------------------------------------- */

/* Required CMPI library headers */
#include "cmpidt.h"


/* ---------------------------------------------------------------------------
 * EXPORTED LIBRARY FUNCTIONS
 * --------------------------------------------------------------------------- */

/* These functions are called by the external XML parser to set instance property values */
int NFSv3setProperty( const char * name, const char * typename, const char * valuebuffer );
int NFSv3setArrayProperty( const char * name, const char * typename, const char * valuebuffer );

/* General purpose functions */
int Linux_NFSv3_sameObject( const CMPIObjectPath * object1, const CMPIObjectPath * object2 );

/* Read instance data */
void * Linux_NFSv3_startReadingInstances();
int Linux_NFSv3_readNextInstance( void * instances, const CMPIInstance ** nextinstance, const CMPIBroker * broker, const char * namespace );
void Linux_NFSv3_endReadingInstances( void * instances );

/* Write instance data */
void * Linux_NFSv3_startWritingInstances();
int Linux_NFSv3_writeNextInstance( void * instances, const CMPIInstance * nextinstance );
void Linux_NFSv3_endWritingInstances( void * newinstances, int commit );

/* Make configuration instance */
CMPIInstance * Linux_NFSv3_makeConfigInstance( const CMPIBroker * broker, const char * namespace );

