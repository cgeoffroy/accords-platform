#ifndef	_testswift_c
#define	_testswift_c

#include <stdlib.h>
#include <stdio.h>

#include "standard.h"
#include "swiftclient.c"

private	int	verbose=1;
private	int	debug=0;
private	char *	operator="accords";
private	char *	zone="europe";
private	char *	tls=(char *) 0;

public	int	check_debug()		{	return( debug );	}
public	int	check_verbose()		{	return( verbose );	}
public	char *	default_zone()		{	return( zone );		}
public	char *	default_operator()	{	return( operator );	}
public	char *	default_tls()		{	return( tls );		}
public	int	failure(int status, char * message)
{
	if ( status )
		printf("**failure(%u,%s)**\n",status,message);
	return( status );
}

private int terminate_command(int status, char *msg) {
	if (status)
		return failure(status, msg);
	else {
		printf("Result:\n%s", msg);
		return 0;
	}
}

private int swift_get_command( struct swift_config const *confPtr, char * subject, int nbOpts, char *opts[] ) {
	int status;
	char *msg;

	if ( !strcmp( subject, "entrypoint" ) && nbOpts >= 1) {
		struct entry_point *ep;
		status = swift_get_bucket_entrypoint(confPtr, opts[0], &ep);
		msg = "error during the entrypoint call";
		if ( !status )
			asprintf(&msg, "\turl='%s'\n\ttoken='%s'\n", ep->url, ep->token);

	} else {
		status = 100;
		msg = "unknown subject or invalid options for the 'get' command";
	}

	return terminate_command(status, msg);
}

private int swift_create_command( struct swift_config const *confPtr, char * subject, int nbOpts, char *opts[] ) {
	int status;
	char *msg;

	if ( !strcmp( subject, "bucket" ) && nbOpts >= 1 ) {
		status =  swift_create_bucket(confPtr, opts[0]);
		msg = "error during the bucket creation";
		if ( !status)
			asprintf(&msg, "\tbucket successfully created\n");

	} else {
		status = 100;
		msg = "unknown subject or invalid options for the 'create' command";
	}

	return terminate_command(status, msg);
}


private int swift_list_command( struct swift_config const *confPtr, char * subject, int nbOpts, char *opts[] ) {
	int status;
	char *msg;

	if ( !strcmp( subject, "buckets" ) && nbOpts == 0 ) {
		long nbB, i;
		char **dst;
		status =  swift_list_buckets(confPtr, &nbB, &dst);
		msg = "error during the buckets listing";
		if ( !status) {
			printf("[");
			for (i = 0; i < nbB; i++) {
				printf("%s, ", dst[i]);
			}
			printf("]\n");
			asprintf(&msg, "\tbucket successfully listed\n");
		}

	} else {
		status = 100;
		msg = "unknown subject or invalid options for the 'list' command";
	}

	return terminate_command(status, msg);
}


private int swift_delete_command( struct swift_config const *confPtr, char * subject, int nbOpts, char *opts[] ) {
	int status;
	char *msg;

	if ( !strcmp( subject, "bucket" ) && nbOpts >= 1 ) {
		status =  swift_delete_bucket(confPtr, opts[0]);
		msg = "error during the bucket deletion";
		if ( !status)
			asprintf(&msg, "\tbucket successfully deleted\n");

	} else {
		status = 100;
		msg = "unknown subject or invalid options for the 'delete' command";
	}

	return terminate_command(status, msg);
}


private	int dc_operation( struct swift_config const *confPtr, char *command, char *subject, int nbOpts, char *opts[] )
{
	if (!( strcmp( command, "create" ) )) {
		return ( swift_create_command(confPtr, subject, nbOpts, opts) );

	} else if (!( strcmp( command, "get" ) )) {
		return ( swift_get_command(confPtr, subject, nbOpts, opts) );

	} else if (!( strcmp( command, "list" ) )) {
		return ( swift_list_command(confPtr, subject, nbOpts, opts) );

	} else if (!( strcmp( command, "delete" ) )) {
		return ( swift_delete_command(confPtr, subject, nbOpts, opts) );

	} 
	
	return failure(10, "unknown command");
}


private int	swift_test_main( int argc, char * argv[] )
{
	char *	tenant_name=(char *) 0;
	char *	auth_version=(char *) 0;
	char * 	user=(char *) 0;
	char *	agent="swift-testclient/1.0";
	char *	auth_url=(char *) 0;
	char *  password=(char *) 0;
	char *	command=(char *) 0;
	char *	subject=(char *) 0;
	int	argi=1;
	char * aptr;
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			struct swift_config confPtr;
			asprintf(&(confPtr.id), "%l", random());
			asprintf(&(confPtr.name), "name_testswift_%d", random());
			confPtr.tenant_name = tenant_name;
			confPtr.current = 0;
			confPtr.auth_version = auth_version;
			confPtr.auth_url = auth_url;
			confPtr.user = user;
			confPtr.password = password;
			confPtr.description = "a description for the Swift test client";

			if (!( command ))
				command = aptr;
			else if (!( subject )) {
				subject = aptr;
				return ( dc_operation( &confPtr, command, subject, argc - argi, &(argv[argi]) ) );
			} else {
				return ( dc_operation( &confPtr, command, (char *) 0, 0, (char **) 0 ) );
			}
		}
		else if ( *(aptr+1) = '-' )
		{
			aptr+=2;
			if (!( strcmp(aptr,"tenant_name" )))
				tenant_name = argv[argi++];
			else if (!( strcmp(aptr,"auth_version" )))
				auth_version = argv[argi++];
			else if (!( strcmp(aptr,"auth_url" )))
				auth_url = argv[argi++];
			else if (!( strcmp(aptr,"user" )))
				user = argv[argi++];
			else if (!( strcmp(aptr,"password" )))
				password = argv[argi++];
			else if (!( strcmp( aptr, "verbose" ) ))
				verbose =1;
			else if (!( strcmp( aptr, "debug" ) ))
				debug =1;
			else
			{
				printf("\nincorrect option: %s\n",aptr);
				return(2);
			}
		}
		else
		{
			printf("\nincorrect option: %s\n",aptr);
			return(3);
		}
	}

	printf("\nnot enought parameters\n");
	return( -1 );
}

private int swift_test_banner()
{
	printf("\n   CompatibleOne Swift Test Client : Version 1.0a.0.01 ");
	printf("\n   Beta Version 14/06/2013 ");
	printf("\n   Copyright (c) 2013 Geoffroy\n");
	printf("\n   Options:\n");
	printf("\n   --tenant_name     {tenant name}    specify Swift tenant name ");
	printf("\n   --auth_version    {version}        specify Swift authentification version ");
	printf("\n   --auth_url        {url}            specify Swift authentification endpoint    ");
	printf("\n   --user            {username}       specify Swift user name ");
	printf("\n   --password        {password}       specify Swift password ");
	printf("\n   --verbose                          activate information messages ");
	printf("\n   --debug                            activate debug messages \n");
	printf("\n   Methods:\n");
	printf("\n   list   [ buckets ]");
	printf("\n   create bucket name ");
	printf("\n   get    entrypoint bucket_name ");
	printf("\n\n");
	return( 0 );
}

public int	main( int argc, char * argv[] )
{
	int	status;
	if ( argc == 1 )
		return( swift_test_banner() );
	else if (!( status = swift_test_main(argc, argv) ))
		return(status);
	else
	{
		printf("**failure(%u,%s)**\n",status,argv[0]);
		return(status);
	}
}

#endif 	/* _testswift_c */
	/* -------- */

