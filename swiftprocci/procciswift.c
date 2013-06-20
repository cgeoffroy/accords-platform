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
#ifndef	_procciswift_c
#define	_procciswift_c

#include "cordslang.h"
#include "stdnode.h"
#include "swift.h"
#include "swiftclient.c"

/*
#define _ASPRINTF(target, fmt, args...) asprintf(target, "%s:%s:%d: "fmt, __FILE__, __FUNCTION__, __LINE__, args)
*/



/*	--------------------------------------------------------	*/
/* 	       r e s e t _ s w i f t _ s e r v e r      		*/
/*	--------------------------------------------------------	*/
private	int	reset_swift_server( struct swift * pptr )
{
	if ( pptr )
	{
		pptr->state = _OCCI_IDLE;
	}
	return(0);
}



/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ s w i f t _ c o n f i g u r a t i o n		*/
/* Given the name 'sptr' the function will search accross every 'swift_config' 	*/
/* that have been register in the corresponding occi "db" (inside 'occiswift.c')*/
/* until it find a correspondance (when the names are equal).			*/
/* ---------------------------------------------------------------------------- */
private	struct	swift_config * resolve_swift_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	swift_config * pptr=(struct swift_config *) 0;
	struct	occi_kind_node  * occi_first_swift_config_node();
	char *tmp;
	asprintf(&tmp, "~~~ SW - resolve_swift_configuration : '%s'", sptr);
	printf("%s\n", tmp);
	rest_log_message(tmp);
	rest_log_message( sptr );
	for (	nptr = occi_first_swift_config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next )
	{
		if (!( pptr = nptr->contents ))
			continue;
		else if (!( pptr->name ))
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))
			return( pptr );
	}
	return((struct swift_config *) 0);
}

/* Retrieve the bucket name from a swift instance
 * Return NULL when it succeed and write the bucket name inside dst as a newly allocated string.
 *   Incase of error, dst is left untouched and a 'struct rest_response' containing the error msg is write in 'aptr' and returned.
 */
private struct rest_response *retrieve_bucket_name(struct rest_response *aptr, struct swift const *pptr, char **dst) {
	if (!( pptr->node )) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : node id not found" ) );
	}

	printf("~~~ SW - rbn : fetching node '%s'\n", pptr->node);
	struct	occi_response * target_list_node, * target_node;

	/* resolve the node by his id */
	if (!( target_list_node = (struct occi_response *) cords_retrieve_named_instance_list(
		       "node", "occi.core.id", pptr->node, 
		       "CO-SWIFTPROCCI/PROCCISWIFT/RBN", default_tls()) )) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : fetching list of node failed" ) );
	}
	/* get the first node of the matching set */
	target_node = (struct occi_response *) cords_retrieve_named_instance(
			target_list_node, "CO-SWIFTPROCCI/PROCCISWIFT/RBN",
			default_tls() );
	/* free */
	target_list_node = occi_remove_response( target_list_node );
	if (! target_node) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : fetching first node failed" ) );	
	}

	/* retrive the type name */
	char *type_name = occi_extract_atribut(target_node, "occi", "node", "type");
	if (! type_name ) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : the type name was not found" ) );
	}
	printf("~~~ SW - rbn : type_name='%s'\n", type_name);

	/* free */
	target_node = occi_remove_response( target_node );

	/* resolve the objectstorage manifest by his name */
	target_list_node = (struct occi_response *) cords_retrieve_named_instance_list(
		"objectstorage_manifest", "occi.objectstorage_manifest.id",
		type_name, "CO-SWIFTPROCCI/PROCCISWIFT/RBN",
		default_tls());
	/* free */
	free(type_name);
	if (!( target_list_node )) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : fetching list of objectstorage manifest failed" ) );
	}

	/* get the first objectstorage of the matching set */
	target_node = (struct occi_response *) cords_retrieve_named_instance(
			target_list_node, "CO-SWIFTPROCCI/PROCCISWIFT/RBN",
			default_tls() );
	/* free */
	target_list_node = occi_remove_response( target_list_node );
	if (! target_node) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : fetching first objectstorage failed" ) );	
	}


//////

	/* /\* retrieve the bucket instance link *\/ */
	/* char * bILink = occi_extract_atribut(target_node, "occi", "objectstorage_manifest", "bucket"); */
	/* /\* free *\/ */
	/* target_node = occi_remove_response( target_node ); */
	/* if (! bILink ) { */
	/* 	return( rest_html_response( aptr, 400, "~~~ SW - start : the bucket instance link was not found" ) ); */
	/* } */
	/* printf("~~~ SW - start : bILink='%s'\n", bILink); */

	/* /\* fetching the bucket *\/ */
	/* target_node = occi_simple_get(  */
	/* 	bILink, */
	/* 	"CO-SWIFTPROCCI/PROCCISWIFT/START_SWIFT", */
	/* 	default_tls() ); */
	/* free(bILink); */

/////

	/* trying to get the link to the bucket */
	struct	occi_element  * elt_bucket;
	elt_bucket = occi_locate_element(target_node->first, _OCCI_LINKHEAD);
	if (! elt_bucket ) {
		target_node = occi_remove_response( target_node );
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : LINK elt not found" ) );
	}

	/* reading the link value */
	char *link_to_bucket = allocate_string( standard_message_link_value(elt_bucket->value) );
	target_node = occi_remove_response( target_node );	
	if (! link_to_bucket ) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : link to the bucket not found" ) );
	}
	printf("~~~ SW - rbn : link_to_bucket='%s'\n", link_to_bucket);

	/* fetching the bucket */
	target_node = occi_simple_get(
		link_to_bucket,
		"CO-SWIFTPROCCI/PROCCISWIFT/RBN",
		default_tls() );
	free(link_to_bucket);
////

	if (! target_node ) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : failure when retrieving the bucket" ) );
	}

	//occi_show_response(target_node);

	/* extract the name of the bucket */
	char *bucket_name = allocate_string( occi_extract_atribut(target_node, "occi", "bucket", "name") );
	/* free */
	target_node = occi_remove_response( target_node );
	if (! bucket_name ) {
		return( rest_html_response( aptr, 400, "~~~ SW - rbn : the name of the bucket was not found" ) );
	}
	printf("~~~ SW - rbn : bucket_name='%s'\n", bucket_name);

	*dst = bucket_name;
	
	return ((struct rest_response *) 0);
}


/* Retrieve the swift configuration from a swift instance
 * Return NULL when it succeed and write the configuration inside dst.
 *   Incase of error, dst is left untouched and a 'struct rest_response' containing the error msg is write in 'aptr' and returned.
 */
private struct rest_response *retrieve_swift_config(struct rest_response *aptr, struct swift const *pptr, struct swift_config **dst) {
	if (! pptr->profile ) {
		return( rest_html_response( aptr, 400, "~~~ SW - rsc : the swift instance doesn't have any profile" ) );
	}

	struct swift_config *result = (struct swift_config *) 0;
	/* ------------------------------------- */
	/* looking for the 'swift_configuration' */
	/* ------------------------------------- */
	if (!( result = resolve_swift_configuration( pptr->profile )))
	{
		//reset_swift_server( pptr );
		char *tmpmsg;
		asprintf(&tmpmsg, "~~~ SW - rsc : Configuration Not Found with profile='%s'", pptr->profile);
		rest_html_response( aptr, 400, tmpmsg );
		free(tmpmsg);
		return aptr;
	}
	printf("~~~ SW - rsc : configuration found, id='%s', tenant_name='%s'\n", result->id, result->tenant_name);

	*dst = result;

	return ((struct rest_response *) 0);
}


/*	-------------------------------------------	*/
/* 	      s t a r t  _ s w i f t		  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_swift(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	swift *pptr;
	struct	swift_config *configptr = (struct swift_config *) 0;

	/* --------------------------------- */
	/* retrieve the instance information */
	/* --------------------------------- */
	if (!( pptr = vptr )) {
	 	return( rest_html_response( aptr, 404, "~~~ SW - start : Invalid Action" ) );
	} else if ( pptr->state != _OCCI_IDLE )
		return	( rest_html_response( aptr, 200, "~~~ SW - start : OK" ) );
	
	/* pptr->profile contain the 'name' of the account element in the manifest */
	printf("~~~ SW - start pptr->profile='%s', pptr->account='%s'\n", pptr->profile, pptr->account);

	if ( retrieve_swift_config(aptr, pptr, &configptr) )
		return aptr;
	
	char *bucket_name = (char *) 0;
	if ( retrieve_bucket_name(aptr, pptr, &bucket_name) ) {
		liberate_swift_config(configptr);
		return aptr;
	}
	
	int error_code = swift_create_bucket(configptr, bucket_name);
	free(bucket_name);
	liberate_swift_config(configptr);
	if ( error_code ) {
		return( rest_html_response( aptr, 400, "~~~ SW - start : failed to  create the bucket" ) );
	}
	
	pptr->state = _OCCI_RUNNING;

	if ( rest_valid_string( pptr->price ) ) {
		char reference[512];
		sprintf(reference,"%s/%s/%s",SwiftProcci.identity,_CORDS_SWIFT,pptr->id);
		occi_send_transaction( _CORDS_SWIFT, pptr->price, "action=start", pptr->account, reference );
	}

	return( rest_html_response( aptr, 200, "~~~ SW - start : OK" ) );
}



/*	--------------------------------------------------------	*/
/* 	s t o p  _ o p e n n e b u l a _ p r o v i s i o n i n g  	*/
/*	--------------------------------------------------------	*/
private	struct	rest_response *stop_swift_provisioning(struct rest_response *aptr, struct swift * pptr)
{
	struct	swift_config *configptr = (struct swift_config *) 0;

	/* --------------------------------- */
	/* retrieve the instance information */
	/* --------------------------------- */
	if ( pptr->state == _OCCI_IDLE )
		return	( rest_html_response( aptr, 200, "~~~ SW - sfp : OK" ) );
	
	/* pptr->profile contain the 'name' of the account element in the manifest */
	printf("~~~ SW - sfp pptr->profile='%s', pptr->account='%s'\n", pptr->profile, pptr->account);

	if ( retrieve_swift_config(aptr, pptr, &configptr) )
		return aptr;
	
	char *bucket_name = (char *) 0;
	if ( retrieve_bucket_name(aptr, pptr, &bucket_name) )
		return aptr;
	
	int error_code = swift_delete_bucket(configptr, bucket_name);
	free(bucket_name);
	liberate_swift_config(configptr);
	if ( error_code ) {
		return( rest_html_response( aptr, 400, "~~~ SW - sfp : failed to  create the bucket" ) );
	}
	
	pptr->state = _OCCI_IDLE;

	return ((struct rest_response *) 0);
}



/*	-------------------------------------------	*/
/* 	      s t o p  _ s w i f t        	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_swift(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	swift * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ( stop_swift_provisioning(aptr, pptr) )
		return( rest_html_response( aptr, 400, "Not Found" ) );
	else
	{
		pptr->when = time((long *) 0);
		char		reference[512];
		sprintf(reference,"%s/%s/%s",SwiftProcci.identity,_CORDS_SWIFT,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			occi_send_transaction( _CORDS_SWIFT, pptr->price, "action=stop", pptr->account, reference );
		return( rest_html_response( aptr, 200, "~~~ SW - stop : OK" ) );
	}
}



/*	-------------------------------------------	*/
/* 	      c r e a t e _ s w i f t	 	*/
/*	-------------------------------------------	*/
private	int	create_swift(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	//struct	os_subscription * subptr=(struct os_subscription *) 0;
	struct	swift * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      r e t r i e v e _ s w i f t 	 	*/
/*	-------------------------------------------	*/
private	int	retrieve_swift(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	//struct	os_subscription * subptr=(struct os_subscription *) 0;
	struct	swift * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}
/*	-------------------------------------------	*/
/* 	      u p d a t e _ s w i f t		 	*/
/*	-------------------------------------------	*/
private	int	update_swift(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	//struct	os_subscription * subptr=(struct os_subscription *) 0;
	struct	swift * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}
/*	-------------------------------------------	*/
/* 	      d e l e t e _ s w i f t	 	 	*/
/*	-------------------------------------------	*/
private	int	delete_swift(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	//struct	os_subscription * subptr=(struct os_subscription *) 0;
	struct	swift * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	swift_interface = {
	create_swift,
	retrieve_swift,
	update_swift,
	delete_swift
	};

/*	-------------------------------------------	*/
/* 	       b u i l d _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
/*	this function is to be called to build the	*/
/*	complete swift occi category to offer	*/
/*	an openstack swift provisioning service	*/
/*	through a compatible one published occi		*/
/*	service interface.				*/
/*	-------------------------------------------	*/
public	struct	occi_category * build_swift( char * domain )
{
	//struct	os_subscription * subptr=(struct os_subscription *) 0;
	struct	occi_category * optr;
	if (!( optr = occi_swift_builder( domain, "swift" ) ))
		return( optr );
	else
	{
		optr->callback  = &swift_interface;
		optr->access |= _OCCI_PROVIDER;

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_swift)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_swift)))
			return( optr );
		else    return( optr );
	}
}

/*	-------------------------------------------	*/
/*	 s e t _ d e f a u l t _ o p e n s t a c k	*/
/*	-------------------------------------------	*/
private	int	set_default_swift(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	swift_config * pptr;
	struct	swift_config * wptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		while ( nptr->previous )
			nptr = nptr->previous;
		while ( nptr )
		{
			if ((wptr = nptr->contents) != (struct swift_config*) 0)
				wptr->current = 0,
			nptr = nptr->next;
		}
		pptr->current = 1;
		return(0);
	}
}

/*	---------------------------------------------------------	*/
/* 	b u i l d _ o p e n s t a c k _ c o n f i g u r a t i o n	*/
/*	---------------------------------------------------------	*/
/*	this category handles the configuration of the interface	*/
/*	to the swift server for the preceeding category of		*/
/*	provisioning instance requests.					*/
/*	---------------------------------------------------------	*/
public	struct	occi_category * build_swift_configuration( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_swift_config_builder( domain, "swift_configuration" ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"current","",set_default_swift)))
		return( optr );
	else	return( optr );
}

#endif	/* _procciswift_c */
	/* ----------- */
