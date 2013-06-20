/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

#ifndef	_cords_language_h
#define	_cords_language_h

#define	_CORDS_NS	"http://www.compatibleone.fr/schemes/cords.xsd"

#define	_INHIBIT_AUTOSTART	1

/*	------------------------	*/
/*	 cords attribute values		*/
/*	------------------------	*/
#define	_CORDS_NULL	"(null)"
#define	_CORDS_ANY	"any"
#define	_CORDS_NONE	"none"
#define	_CORDS_SIMPLE	"simple"
#define	_CORDS_PUBLIC	"public"
#define	_CORDS_PRIVATE	"private"
#define	_CORDS_COMMON	"common"
#define	_CORDS_NORMAL	"normal"
#define	_CORDS_GLOBAL	"global"
#define	_CORDS_TO	"to"
#define	_CORDS_FROM	"from"
#define	_CORDS_PROTOCOL "protocol"

/*	------------------------	*/
/*	cords action identifiers	*/
/*	------------------------	*/
#define	_CORDS_START	"start"
#define	_CORDS_SUSPEND	"suspend"
#define	_CORDS_SAVE	"save"
#define	_CORDS_SNAPSHOT "snapshot"
#define	_CORDS_RESTART	"restart"
#define	_CORDS_STOP	"stop"
#define	_CORDS_INSTANCE	"instance"
#define	_CORDS_BUILD	"build"
#define	_CORDS_CHOOSE	"choose"
#define	_CORDS_CONSUME	"consume"
#define	_CORDS_RESTORE	"restore"
#define	_CORDS_RELEASE	"release"
#define	_CORDS_RESOLVE	"resolve"

/*	-------------------------	*/
/*	cords atribut identifiers	*/
/*	-------------------------	*/
#define	_CORDS_ID		"id"
#define	_CORDS_NAME		"name"
#define	_CORDS_MEMORY		"memory"
#define	_CORDS_ARCHITECTURE	"architecture"
#define	_CORDS_SPEED		"speed"
#define	_CORDS_CORES		"cores"
#define	_CORDS_SIZE		"size"
#define	_CORDS_TYPE		"type"
#define	_CORDS_ACCESS		"access"
#define	_CORDS_SCOPE		"scope"
#define	_CORDS_CREATED		"created"
#define	_CORDS_VALIDATION	"validation"
#define	_CORDS_INITIATOR	"initiator"
#define	_CORDS_SLA		"sla"
#define	_CORDS_VARIABLE		"variable"
#define	_CORDS_PENALTY		"penalty"
#define	_CORDS_REWARD		"reward"
#define	_CORDS_SERVICES		"services"
#define	_CORDS_CONDITIONS	"conditions"
#define	_CORDS_PROPERTY		"property"
#define	_CORDS_GUARANTEE	"guarantee"
#define	_CORDS_BUSINESS 	"business"
#define	_CORDS_GUARANTEES	"guarantees"
#define	_CORDS_IMPORTANCE	"importance"
#define	_CORDS_CONDITION	"condition"
#define	_CORDS_OBJECTIVE	"objective"
#define	_CORDS_OBLIGATED	"obligated"
#define	_CORDS_PARENTSERVICE	"parentservice"
#define	_CORDS_STARTDURATION	"startduration"
#define	_CORDS_STOPDURATION	"stopduration"
#define	_CORDS_SAVEDURATION	"saveduration"
#define	_CORDS_FLAGS		"flags"
#define	_CORDS_INITIATION	"initiaiton"
#define	_CORDS_EXPIRATION	"expiration"
#define	_CORDS_QUOTA		"quota"
#define	_CORDS_QUANTITY		"quantity"
#define	_CORDS_TIMER		"timer"

/*	-------------------------	*/
/*	cords element identifiers	*/
/*	-------------------------	*/
#define	_CORDS_PUBLICATION	"publication"

#define	_CORDS_IMPORT		"import"
#define	_CORDS_INVOICE		"invoice"

#define	_CORDS_MANIFEST		"manifest"
#define	_CORDS_DESCRIPTION	"description"
#define	_CORDS_SECURITY		"security"
#define	_CORDS_ACCOUNT		"account"
#define	_CORDS_USER		"user"

/*	------------------------	*/
/*	cords node atributs		*/
/*	------------------------	*/
#define	_CORDS_NODE		"node"
#define	_CORDS_PROVIDER		"provider"
#define	_CORDS_PROVISION	"provision"
#define	_CORDS_PROFILE 		"profile"
#define	_CORDS_INFRASTRUCTURE	"infrastructure"
#define	_CORDS_COMPUTE		"compute"
#define	_CORDS_STORAGE		"storage"
#define	_CORDS_NETWORK		"network"
#define	_CORDS_IMAGE		"image"
#define	_CORDS_SYSTEM		"system"
#define	_CORDS_PACKAGE		"package"
#define	_CORDS_FIREWALL		"firewall"
#define	_CORDS_PORT		"port"

#define	_CORDS_CONFIGURATION	"configuration"
#define	_CORDS_RELEASE		"release"
#define	_CORDS_INTERFACE	"interface"
#define	_CORDS_ACTION		"action"
#define	_CORDS_PARAMETER	"parameter"
#define	_CORDS_INSTRUCTION	"instruction"
#define	_CORDS_VALUE		"value"
#define	_CORDS_EXPRESSION	"expression"

#define	_CORDS_ALGORITHM	"algorithm"
#define	_CORDS_PLACEMENT	"placement"
#define	_CORDS_SOLUTION 	"solution"
#define	_CORDS_PLAN		"plan"
#define	_CORDS_AGREEMENT	"agreement"
#define	_CORDS_TERMS		"terms"
#define	_CORDS_TERM		"term"
#define	_CORDS_SERVICE		"service"

#define	_CORDS_OPENSTACK	"openstack"
#define	_CORDS_OPENNEBULA	"opennebula"
#define	_CORDS_WINDOWSAZURE	"windowsazure"
#define	_CORDS_SLAPOS		"slapos"
#define	_CORDS_PROACTIVE	"proactive"
#define	_CORDS_DELTACLOUD	"deltacloud"
#define _CORDS_EC2		"amazonEc2"
	
#define	_CORDS_EVENT		"event"
#define	_CORDS_MONITOR		"monitor"
#define	_CORDS_CONTROL		"control"
#define	_CORDS_REPORT		"report"
#define	_CORDS_SESSION		"session"
#define	_CORDS_PACKET		"packet"
#define	_CORDS_CONSUMER		"consumer"
#define	_CORDS_CONNECTION	"connection"
#define	_CORDS_PROBE		"probe"
#define	_CORDS_METRIC		"metric"
#define	_CORDS_ALERT		"alert"

#define	_CORDS_TRANSACTION	"transaction"
#define	_CORDS_APPLICATION	"application"
#define	_CORDS_SCHEDULE		"schedule"

#define	_CORDS_METADATA		"metadata"
#define	_CORDS_FILE		"file"
#define	_CORDS_SCRIPT		"script"
#define	_CORDS_VM		"vm"
#define	_CORDS_IPADDRESS	"ipaddress"

/*	------------------------	*/
/*	cords  parser  feed back	*/
/*	------------------------	*/
#define	_CORDS_ERROR		"error"
#define	_CORDS_WARNING		"warning"
#define	_CORDS_CHOICE		"choice"

/*	------------------------	*/
/*	cords machine attributes	*/
/*	------------------------	*/
#define	_CORDS_CONTRACT		"contract"
#define	_CORDS_TARIFICATION	"tarification"
#define	_CORDS_PRICE		"price"
#define	_CORDS_AUTHORIZATION	"authorization"
#define	_CORDS_HOSTNAME		"hostname"
#define	_CORDS_ROOTPASS		"rootpass"
#define	_CORDS_REFERENCE	"reference"
#define	_CORDS_CATEGORY		"category"

#define	_CORDS_SWIFT            "swift"

	/* ----------------- */
#endif	/* _cords_language_h */
	/* ----------------- */

