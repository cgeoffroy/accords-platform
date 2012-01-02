/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_oncontract_c
#define	_oncontract_c

#include "occiclient.h"

public	char *	cords_extract_atribut( 
	struct occi_response * zptr, char * domain,
	char * category, char * nptr );

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_on_contract
{
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	struct	cords_vector	network;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
	struct	on_response *	flavors;
	struct	on_response *	images;
};

struct	on_compute_infos
{
	int	cores;
	int	speed;
	int	memory;
	int	storage;
	char *	id;
};

struct	on_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};


/*	-----------------------------------------------------------------	*/
/*	     t e r m i n a t e _ o p e n n e b u l a _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
private	int	terminate_opennebula_contract( int status, struct cords_on_contract * cptr )
{
	if ( cptr->node.message )
		cptr->node.message = occi_remove_response( cptr->node.message );
	if ( cptr->infrastructure.message )
		cptr->infrastructure.message = occi_remove_response( cptr->infrastructure.message );
	if ( cptr->compute.message )
		cptr->compute.message = occi_remove_response( cptr->compute.message );
	if ( cptr->network.message )
		cptr->network.message = occi_remove_response( cptr->network.message );
	if ( cptr->storage.message )
		cptr->storage.message = occi_remove_response( cptr->storage.message );
	if ( cptr->image.message )
		cptr->image.message = occi_remove_response( cptr->image.message );
	if ( cptr->system.message )
		cptr->system.message = occi_remove_response( cptr->system.message );
	if ( cptr->package.message )
		cptr->package.message = occi_remove_response( cptr->package.message );
	if ( cptr->flavors )
		cptr->flavors = liberate_on_response( cptr->flavors );
	if ( cptr->images  )
		cptr->images  = liberate_on_response( cptr->images  );
	return( status );
}

/*	---------------------------------------------------	*/
/*		o s _ n o r m a l i s e _ v a l u e 		*/
/*	---------------------------------------------------	*/
/*	this function takes a value string as its parameter	*/
/*	which may or not be terminated by a T,G,M,K which	*/
/*	indicates an explicite quantity. A default quantity	*/
/*	type is passed as the second parameter to be used	*/
/*	in case no explicite value is present.			*/
/*	The function returns the normalisation of the value	*/
/*	---------------------------------------------------	*/
private	int	on_normalise_value( char * sptr, int normal )
{
	int	factor=1;
	int	value=0;
	if ( normal == 'T' )
		factor = 1000000000;
	else if ( normal == 'G' )
		factor = 1000000;
	else if ( normal == 'M' )
		factor = 1000;
	else if ( normal == 'K' )
		factor = 1;
	else	factor = 1;

	value = atoi(sptr);

	while (( *sptr >= '0' ) && ( *sptr <= '9' )) sptr++;

	if ( *sptr == 'T' )
		factor = 1000000000;
	else if ( *sptr == 'G' )
		factor = 1000000;
	else if ( *sptr == 'M' )
		factor = 1000;
	else if ( *sptr == 'K' )
		factor = 1;

	return( value * factor );
}


/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ o p e n n e b u l a _ f l a v o r 		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_opennebula_flavor( struct cords_on_contract * cptr )
{
	struct	on_compute_infos	request;
	char *	vptr;

	/* -------------------------------------------------------------- */
	/* retrieve appropriate parameters from infrastructure components */
	/* -------------------------------------------------------------- */
	if (!( vptr = cords_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_MEMORY ) ))
		request.memory = 0;
	else	request.memory = on_normalise_value( vptr,'G' );

	if (!( vptr = cords_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_CORES ) ))
		request.cores = 0;
	else	request.cores = on_normalise_value( vptr,'U' );

	if (!( vptr = cords_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_SPEED ) ))
		request.speed = 0;
	else	request.speed = on_normalise_value(vptr,'G');
	
	if (!( vptr = cords_extract_atribut( cptr->storage.message, "occi", 
		_CORDS_STORAGE, _CORDS_SIZE ) ))
		request.storage = 0;
	else	request.storage = on_normalise_value(vptr,'G');

	if (( request.storage <= 40000000 )
	&& ( request.memory  <= 2048000 ))
		return( allocate_string( "small" ) );
	else if (( request.storage > 80000000 )
	     && ( request.memory  > 4096000 ))
		return( allocate_string( "large" ) );
	else	return( allocate_string( "medium" ) );


}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ o p e n n e b u l a _ i m a g e   		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_opennebula_image( struct cords_on_contract * cptr )
{
	struct	on_image_infos	request;
	struct	on_image_infos	image;
	struct	on_image_infos	best;
	char *			vptr;


	struct	xml_element * eptr;
	struct	xml_element * dptr;
	struct	xml_atribut * aptr;

	if (!( eptr = document_element( cptr->images->xmlroot, "STORAGE_COLLECTION" )))
		return((char *) 0);

	/* ---------------------------------------------------------- */
	/* retrieve appropriate parameters from node image components */
	/* ---------------------------------------------------------- */
	if (!( vptr = cords_extract_atribut( cptr->system.message, "occi", 
		_CORDS_SYSTEM, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.name = vptr;

	if (!( vptr = cords_extract_atribut( cptr->image.message, "occi", 
		_CORDS_IMAGE, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.other = vptr;

	memset( &best, 0, sizeof( struct on_image_infos ));

	for ( 	dptr=eptr->first;
		dptr != (struct xml_element *) 0;
		dptr = dptr->next )
	{
		/* ----------------------------------------------- */
		/* collect the information from the flavor element */
		/* ----------------------------------------------- */
		if (!( aptr = document_atribut( dptr, "href" ) ))
			continue;
		else	image.id = aptr->value;
		if (!( aptr = document_atribut( dptr, "name" ) ))
			continue;
		else	image.name = aptr->value;

		if ( (!( strncasecmp( request.name,  image.name, strlen( request.name  ) )))
		||   (!( strncasecmp( request.other, image.name, strlen( request.other ) ))))
		{
			best.id = image.id;
			best.name = image.name;
			break;
		}		
		else	continue;
	}
	if (!( best.id ))
		return( best.id );
	else 	return(allocate_string( best.id ));
}

/*	-----------------------------------------------------------------	*/
/*		c r e a t e _ o p e n n e b u l a _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	create_opennebula_contract(
		struct occi_category * optr,
		struct opennebula * pptr,
		char * agent,
		char * tls )
{
	struct	cords_on_contract contract;
	struct	os_response * flavors=(struct os_response *) 0;
	struct	os_response * images =(struct os_response *) 0;

	memset( &contract, 0, sizeof( struct cords_on_contract ));

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, agent, tls ) ))
		return( terminate_opennebula_contract( 570, &contract ) );

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	else if (!( contract.infrastructure.id = cords_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_opennebula_contract( 571, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, agent, tls ) ))
		return( terminate_opennebula_contract( 572, &contract ) );

	else if (!( contract.compute.id = cords_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_opennebula_contract( 573, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, agent, tls ) ))
		return( terminate_opennebula_contract( 574, &contract ) );

	else if (!( contract.network.id = cords_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_opennebula_contract( 575, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, agent, tls ) ))
		return( terminate_opennebula_contract( 576, &contract ) );

	else if (!( contract.storage.id = cords_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_opennebula_contract( 577, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, agent, tls ) ))
		return( terminate_opennebula_contract( 578, &contract ) );

	/* --------------------------------------------------------- */
	/* recover detailled list of OS Flavors and resolve contract */
	/* --------------------------------------------------------- */
	else if (!( contract.flavors = on_list_flavors() ))
		return( terminate_opennebula_contract( 579, &contract ) );
	else if (!( pptr->flavor = resolve_opennebula_flavor( &contract ) ))
		return( terminate_opennebula_contract( 580, &contract ) );
		
	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = cords_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_opennebula_contract( 581, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_opennebula_contract( 582, &contract ) );

	else if (!( contract.system.id = cords_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_opennebula_contract( 583, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_opennebula_contract( 584, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
	else if (!( contract.images = on_list_images() ))
		return( terminate_opennebula_contract( 585, &contract ) );
	else if (!( pptr->image = resolve_opennebula_image( &contract ) ))
		return( terminate_opennebula_contract( 586, &contract ) );
		
	else 
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( _CORDS_OPENNEBULA, default_operator(), agent, tls );
	
		return( terminate_opennebula_contract( 0, &contract ) );
	}

}

	/* ------------- */
#endif	/* _oncontract_c */
	/* ------------- */
