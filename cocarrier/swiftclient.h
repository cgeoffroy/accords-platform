#ifndef _swift_client_h
#define _swift_client_h

#include "swiftconfig.h"

struct entry_point {
	char * url;
	char * token;
};

/* Retrieve the list of bucket
 * Return an error code > 0.
 *   Otherwise, return 0 when it succeed and write the count of bucket inside the value pointed by ptrBucketsCount.
 *   Then save the address of a newly allocated array containing the name of each bucket inside dst.
 */
public int swift_list_buckets(struct swift_config const *sptr, long *ptrBucketsCount, char ***dst);


/* Create a bucket on the provider
 * Return 0 when it succeed, otherwise it return an error code > 0
 */
public int swift_create_bucket(struct swift_config const *sptr, char const *container_name);


/* Retrieve the information to acces a bucket, like his url and token.
 * Return 0 when it succeed, and write the newly allocad result in the 'dst' variable.
 *   When an error occur, it return an error code > 0.
 */
public int swift_get_bucket_entrypoint(struct swift_config const *sptr, char const *container_name, struct entry_point **dst);


/* Delete a bucket on the provider
 * Return 0 when it succeed, otherwise it return an error code > 0
 */
public int swift_delete_bucket(struct swift_config const *sptr, char const *container_name);


#endif	/* _swift_client_h */
	/* ------------ */
