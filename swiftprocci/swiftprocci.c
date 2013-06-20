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
#ifndef	_swiftprocci_c
#define	_swiftprocci_c

#include "standard.h"
#include "swiftprocci.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

#include "swiftconfig.h"
#include "swift.h"
#include "swift.c"
#include "occiswift.c"
#include "swiftconfig.c"
#include "occiswiftconfig.c"


struct	accords_configuration SwiftProcci = {
        0,0, /* monitor, auto publication */
	1,1,0,0, /* verbose, debug, threads, ipv6 */
	(char *) 0,
	(char *) 0,
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_OPERATOR,
	_CORDS_DEFAULT_USER,
	_CORDS_DEFAULT_PASSWORD,
	"http",  80,
	"xmpp",  8000,
	"domain",
	"swiftprocci.xml",
	"europe",
	"storage",
	"swiftprocci",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(SwiftProcci.debug);		}
public	int	check_verbose()		{	return(SwiftProcci.verbose);		}
public	char *	default_publisher()	{	return(SwiftProcci.publisher);	}
public	char *	default_operator()	{	return(SwiftProcci.operator);	}
public	char *	default_tls()		{	return(SwiftProcci.tls);		}
public	char *	default_zone()		{	return(SwiftProcci.zone);		}

public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n*** failure %u",e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

/*	---------------------------------------------	*/  
/*			p r o c c i _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads swiftprocci configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	swiftprocci_load()
{
	load_accords_configuration( &SwiftProcci, "swiftprocci" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Request Swiftprocci : Version 1.0a.0.04");
	printf("\n   Beta Version : 29/05/2013");
	printf("\n   Copyright (c) 2013, 2012 Geoffroy, TAI");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			p r o c c i _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * swiftprocci_initialise(  void * v,struct rest_server * sptr )
{
	struct	rest_extension * xptr;
	if (!( xptr = rest_add_extension( sptr ) ))
		return((struct rest_server *) 0);
	else
	{
		xptr->net = (struct connection *) 0;
		return( sptr );
	}
}

/*	------------------------------------------------------------------	*/
/*			p r o c c i _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	swiftprocci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, SwiftProcci.user ) )
		return(0);
	else if ( strcmp( password, SwiftProcci.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			p r o c c i _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * swiftprocci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

#include "procciswift.c"

/*	------------------------------------------------------------------	*/
/*			p r o c c i _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	swiftprocci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_swift.xml");

	if (!( optr = build_swift( SwiftProcci.domain ) ))
		return( 27 );

	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = build_swift_configuration( SwiftProcci.domain ) ))
		return( 27 );

	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log( SwiftProcci.monitor );

	if (!( SwiftProcci.identity ))
		return( occi_server(  nptr, SwiftProcci.restport, SwiftProcci.tls, SwiftProcci.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( SwiftProcci.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			SwiftProcci.user, SwiftProcci.password,
			SwiftProcci.identity,  nptr, 
			SwiftProcci.restport, SwiftProcci.tls, 
			SwiftProcci.threads, first ) );
	}
}


/*	------------------------------------------------------------------	*/
/*				p r o c c i 					*/
/*	------------------------------------------------------------------	*/
private	int	swiftprocci(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	swiftprocci_load();
	while ( argi < argc )
	{
		if (!( aptr = argv[++argi] ))
			break;
		else if ( *aptr == '-' )
		{
			aptr++;
			switch( *(aptr++) )
			{
			case	'v'	:
				SwiftProcci.verbose=1;
				continue;
			case	'd'	:
				SwiftProcci.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = swiftprocci_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	------------------------------------------------------------------	*/
/*					m a i n 				*/
/*	------------------------------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( banner() );
	else	return( swiftprocci( argc, argv ) );

}


	/* --------- */
#endif	/* _swiftprocci_c */
	/* --------- */

