#ifndef _swift_client_c
#define _swift_client_c

#include <stdio.h>
#include "swiftclient.h"

#include <stdlib.h>
#include <Python.h>

/* see http://docs.python.org/3.4/extending/embedding.html */
/* see http://docs.python.org/2/extending/extending.html */


private PyObject * get_auth(struct swift_config const *sptr, PyObject * pModule) {
	PyObject * someList = Py_BuildValue("(sss)", sptr->auth_url, sptr->user, sptr->password);
	if (! ( someList && PyTuple_Check(someList) )) {
		printf("Py - get_auth : can not build the function args\n");		
		return 0;
	}
	PyObject * someKwords = Py_BuildValue("{s:s,s:s}",
					     "auth_version", sptr->auth_version,
					     "tenant_name", sptr->tenant_name);
	if (! ( someKwords && PyDict_Check(someKwords) )) {
		printf("Py - get_auth : can not build the function dictionary\n");
		Py_DECREF(someList);
		return ((PyObject *) 0);
	}

	PyObject * pFunc = PyObject_GetAttrString(pModule, "get_auth");
	if (! (pFunc && PyCallable_Check(pFunc)) ) {
		printf("Py - get_auth : the 'get_auth' function was not found\n");
		Py_DECREF(someList);
		Py_DECREF(someKwords);
		return ((PyObject *) 0);
	}

	PyObject * urlAndToken = PyObject_Call(pFunc, someList, someKwords);
	Py_DECREF(someList);
	Py_DECREF(someKwords);
	Py_DECREF(pFunc);
	if (! ( urlAndToken && PyTuple_Check(urlAndToken)
		&& (2 == PyTuple_Size(urlAndToken)) ) ) {
		printf("Py get_auth : error during 'get_auth' call\n");
		if ( PyErr_Occurred() ) {
			PyErr_Print();
			PyErr_Clear();
		}
		return ((PyObject *) 0);
	}

	return urlAndToken;
}


typedef int (*signature) (PyObject *, PyObject *, void *);


/* Handle python setup and authentification
 * Return an error code or 0 when it succeed
 */
private int do_when_authentified(struct swift_config const *sptr, signature funPointer, void * extra_args) {
	PyObject * pName, * pModule;

	Py_InitializeEx(0);

	pName = PyUnicode_FromString("swiftclient.client");
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if (! (pModule && PyModule_Check(pModule) )) {
		printf("Py : swiftclient module not found\n");
		return 1;
	}

	if ( check_debug() ) {
		PyRun_SimpleString("from swiftclient import client\n"
				   "import logging\n"
				   "logging.basicConfig()\n"
				   "client.logger.setLevel(logging.DEBUG)\n");
	}

	PyObject * urlAndToken = get_auth(sptr, pModule);
	if (! urlAndToken ) {
		Py_DECREF(pModule);
		return 2;
	}
	
	int retCode = ((signature) *funPointer)(pModule, urlAndToken, extra_args);

	Py_DECREF(urlAndToken);
	Py_DECREF(pModule);
	
	Py_Finalize();

	return retCode;
}


private int ll_get_account(PyObject * pModule, PyObject * urlAndToken, void * args) {
	// we call this function with an array of 2 args
	void **tabOfArgs = (void **) args; 
	long *ptrBC = (long *) tabOfArgs[0];
	char ***dst = (char ***) tabOfArgs[1];

	PyObject * pFunc;
	pFunc = PyObject_GetAttrString(pModule, "get_account");
	if (! (pFunc && PyCallable_Check(pFunc)) ) {
		printf("Py : the 'get_account' function was not found\n");
		return 101;
	}

	PyObject * pyArgs = PyTuple_New(2);
	Py_INCREF(PyTuple_GetItem(urlAndToken, 0));
	PyTuple_SetItem(pyArgs, 0, PyTuple_GetItem(urlAndToken, 0));
	Py_INCREF(PyTuple_GetItem(urlAndToken, 1));
	PyTuple_SetItem(pyArgs, 1, PyTuple_GetItem(urlAndToken, 1));

	PyObject * result = PyObject_Call(pFunc, pyArgs, NULL);
	Py_DECREF(pFunc);
	Py_DECREF(pyArgs);
	
	if ( PyErr_Occurred() ) {
		printf("Py : error during 'get_account' call\n");
		PyErr_Print();
		PyErr_Clear();
		return 102;
	}
	
	if (! ( result && PyTuple_Check(result)
		&& (2 == PyTuple_Size(result)) ) ) {
		printf("Py get_account : error during 'get_auth' call\n");
		if ( PyErr_Occurred() ) {
			PyErr_Print();
			PyErr_Clear();
		}
		return 103;
	}
	
	PyObject * lContainers = PyTuple_GetItem(result, 1);
	*ptrBC = PyList_Size(lContainers);
	
	char **dstValues = malloc(sizeof(char *) * *ptrBC);
	long i;
	for (i = 0; i < *ptrBC; i++) {
		PyObject *elt = PyList_GetItem(lContainers, i);
		PyObject *tmpString = PyUnicode_FromString("name");
		dstValues[i] = allocate_string(PyString_AsString( PyDict_GetItem(elt, tmpString) ));
		Py_DECREF(tmpString);
	}

	Py_DECREF(result);

	*dst = dstValues;
	
	return 0;
}


public int swift_list_buckets(struct swift_config const *sptr, long *ptrBC, char ***dst) {
	if ( !sptr ) {
		return 10;
	}

	void *args[2] = { (void *) ptrBC, (void *) dst };
	return ( do_when_authentified(sptr, &ll_get_account, args) );
}


/* Create the container on the swift provider
   Return 0 when it succeed, otherwise return an error code > 0
*/
private int ll_put_container(PyObject *pModule, PyObject *urlAndToken, void *args) {
	char const *bucketname = (char const *) args;
	PyObject *pFunc;

	pFunc = PyObject_GetAttrString(pModule, "put_container");
	if (! (pFunc && PyCallable_Check(pFunc)) ) {
		printf("Py : the 'put_container' function was not found\n");
		Py_DECREF(urlAndToken);
		return 101;
	}

	PyObject *pbucketName = PyUnicode_FromString(bucketname);
	PyObject *pyArgs = PyTuple_New(3);
	Py_INCREF(PyTuple_GetItem(urlAndToken, 0));
	PyTuple_SetItem(pyArgs, 0, PyTuple_GetItem(urlAndToken, 0));
	Py_INCREF(PyTuple_GetItem(urlAndToken, 1));
	PyTuple_SetItem(pyArgs, 1, PyTuple_GetItem(urlAndToken, 1));
	Py_INCREF(pbucketName);
	PyTuple_SetItem(pyArgs, 2, pbucketName);

	PyObject_Call(pFunc, pyArgs, NULL);
	Py_DECREF(pFunc);
	Py_DECREF(pyArgs);
	Py_DECREF(urlAndToken);
	Py_DECREF(pbucketName);
	
	if ( PyErr_Occurred() ) {
		printf("Py : error during 'put_container' call\n");
		PyErr_Print();
		PyErr_Clear();
		return 102;
	}
	
	return 0;
}


public int swift_create_bucket(struct swift_config const *sptr, char const *container_name) {
	if ( ! container_name || !sptr ) {
		return 10;
	}

	return ( do_when_authentified(sptr, &ll_put_container, (void *) container_name) );
}


private int ll_get_bucket_entrypoint(PyObject *pModule, PyObject *urlAndToken, void *args) {
	// we call this function with an array of 2 args
	void **tabOfArgs = (void **) args; 
	char *bucketName = (char *) tabOfArgs[0];
	struct entry_point **dst = (struct entry_point **) tabOfArgs[1];
	
	PyObject *endpoint = PyTuple_GetItem(urlAndToken, 0);
	PyObject *token = PyTuple_GetItem(urlAndToken, 1);

	if ( PyUnicode_Check(endpoint) && PyUnicode_Check(token) ) {
		struct entry_point *result = malloc( sizeof(struct entry_point) );
		asprintf(&(result->url), "%s/%s", PyString_AsString(endpoint), bucketName);
		result->token = allocate_string( PyString_AsString(token) );
		*dst = result;
		return 0;
	} else {
		return 101;
	}
}


public int swift_get_bucket_entrypoint(struct swift_config const *sptr, char const *container_name, struct entry_point **dst) {
	if ( !container_name || !sptr ) {
		return 10;
	}

	void *args[2] = { (void *) container_name, (void *) dst };
	return ( do_when_authentified(sptr, &ll_get_bucket_entrypoint, args) );
}


private int ll_delete_container(PyObject *pModule, PyObject *urlAndToken, void *args) {
	char const *bucketname = (char const *) args;
///
	PyObject *maimM = PyImport_AddModule("__main__");

	int counter = 0;
	char *freshname;
	asprintf(&freshname, "z%d_swiftprocci_freshmodule_%d", random(), counter);
	while ( PyObject_HasAttrString(maimM, freshname) ) {
		counter++;
		free(freshname);
		asprintf(&freshname, "z%d_swiftprocci_freshmodule_%d", random(), counter);
	}

	char *code;
	asprintf(&code,
		 "def %s(url, token, bucketname):\n"
		 "\twhile True:\n"
		 "\t\t(rep, lo) = client.get_container(url, token, bucketname)\n"
		 "\t\tprint rep\n"
		 "\t\tif int(rep['x-container-object-count']) <= 0:\n"
		 "\t\t\tbreak\n"
		 "\t\tfor (o) in lo:\n"
		 "\t\t\tprint o\n"
		 "\t\t\tclient.delete_object(url, token, bucketname, o['name'])\n"
		 "\tclient.delete_container(url, token, bucketname)\n",
		 freshname);

	PyRun_SimpleString(code);
	free(code);

	PyObject *gFunc = PyObject_GetAttrString(maimM, freshname);
	free(freshname);
	if (! (gFunc && PyCallable_Check(gFunc)) ) {
		printf("Py : the 'g' function was not found\n");
		return 101;
	}

	PyObject *pbucketName = PyUnicode_FromString(bucketname);
	PyObject *pyArgs = PyTuple_New(3);
	Py_INCREF(PyTuple_GetItem(urlAndToken, 0));
	PyTuple_SetItem(pyArgs, 0, PyTuple_GetItem(urlAndToken, 0));
	Py_INCREF(PyTuple_GetItem(urlAndToken, 1));
	PyTuple_SetItem(pyArgs, 1, PyTuple_GetItem(urlAndToken, 1));
	Py_INCREF(pbucketName);
	PyTuple_SetItem(pyArgs, 2, pbucketName);
	
	PyObject_Call(gFunc, pyArgs, NULL);
	Py_DECREF(gFunc);
	Py_DECREF(pyArgs);
	Py_DECREF(pbucketName);

	if ( PyErr_Occurred() ) {
		printf("Py : error during 'delete_container' call\n");
		PyErr_Print();
		PyErr_Clear();
		return 102;
	}

	return 0;
}

public int swift_delete_bucket(struct swift_config const *sptr, char const *container_name) {
	if ( !container_name || !sptr ) {
		return 10;
	}

	return ( do_when_authentified(sptr, &ll_delete_container, (void *) container_name) );
}


#endif	/* _swift_client_c */
	/* ------------ */
