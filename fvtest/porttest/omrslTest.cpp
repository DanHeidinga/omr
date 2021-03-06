/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/


/**
 * @file
 * @ingroup PortTest
 * @brief Verify port library file system.
 *
 * Exercise the API for port library shared library system operations.  These functions
 * can be found in the file @ref omrsl.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "omrport.h"


#include "testHelpers.hpp"
#include "testProcessHelpers.hpp"
#include "omrfileTest.h"

/**
 * Verify port library properly setup to run sl tests
 */
TEST(PortSlTest, sl_verify_function_slots)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsl_verify_function_slots";

	reportTestEntry(OMRPORTLIB, testName);

	if (NULL == OMRPORTLIB->sl_close_shared_library) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "portLibrary->sl_close_shared_library is NULL\n");
	}

	if (NULL == OMRPORTLIB->sl_lookup_name) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "portLibrary->sl_lookup_name is NULL\n");
	}

	if (NULL == OMRPORTLIB->sl_open_shared_library) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "portLibrary->sl_open_shared_library is NULL\n");
	}

	if (NULL == OMRPORTLIB->sl_shutdown) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "portLibrary->sl_shutdown is NULL\n");
	}

	if (NULL == OMRPORTLIB->sl_startup) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "portLibrary->sl_startup is NULL\n");
	}

	reportTestExit(OMRPORTLIB, testName);

}


/**
 * Basic test: load the port Library dll.
 */
TEST(PortSlTest, sl_test1)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsl_test1";
	uintptr_t handle;
	uintptr_t rc = 0;
	char sharedLibName[] = "sltestlib";

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsl_open_shared_library(sharedLibName, &handle, OMRPORT_SLOPEN_DECORATE);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "Unable to open %s, \n", sharedLibName, omrerror_last_error_message());
		goto exit;
	}

exit:

	reportTestExit(OMRPORTLIB, testName);
}

#if defined(AIXPPC)

/**
 * CMVC 197740
 * The exact message varies depending on the OS version. look for any of several possible strings.
 * @param dlerrorOutput actual dlerror() result
 * @param expectedOutputs list of possible strings, terminated by a null  pointer
 * @result true if one of expectedOutputs is found in dlerrorOutput
 */
static BOOLEAN
isValidLoadErrorMessage(const char *dlerrorOutput, const char *possibleMessageStrings[])
{
	BOOLEAN result = FALSE;
	uintptr_t candidate = 0;
	while (NULL != possibleMessageStrings[candidate]) {
		if (NULL != strstr(dlerrorOutput, possibleMessageStrings[candidate])) {
			result = TRUE;
			break;
		}
		++candidate;
	}
	return result;
}

/**
 * Loading dll with missing dependency on AIX, expecting a descriptive OS error message
 */
TEST(PortSlTest, sl_AixDLLMissingDependency)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsl_AixDLLMissingDependency";
	uintptr_t handle;
	uintptr_t rc = 0;
	char *sharedLibName = "aixbaddep";
	const char *osErrMsg;
	const char *possibleMessageStrings[] = {"0509-150", "Dependent module dummy.exp could not be loaded", NULL};

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsl_open_shared_library(sharedLibName, &handle, OMRPORT_SLOPEN_DECORATE);
	if (0 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, " Unexpectedly loaded %s, should have failed with dependency error\n", sharedLibName, omrerror_last_error_message());
		goto exit;
	}

	osErrMsg = omrerror_last_error_message();
	outputComment(OMRPORTLIB, "System error message=\n\"%s\"\n", osErrMsg);
	if (!isValidLoadErrorMessage(osErrMsg, possibleMessageStrings)) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, " Cannot find valid error code, should have failed with dependency error\n", sharedLibName, omrerror_last_error_message());
	}
exit:

	reportTestExit(OMRPORTLIB, testName);
}

/**
 * Loading dll of wrong platform on AIX, expecting a descriptive OS error message
 */
TEST(PortSlTest, sl_AixDLLWrongPlatform)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsl_AixDLLWrongPlatform";
	uintptr_t handle;
	uintptr_t rc = 0;
	/* Intentionally load 64bit dll on 32 bit AIX, and 32bit dll on 64bit AIX */
#if defined(OMR_ENV_DATA64)
	char *sharedLibName = "/usr/lib/jpa";
#else
	char *sharedLibName = "/usr/lib/jpa64";
#endif
	const char *osErrMsg;
	const char *possibleMessageStrings[] = {"0509-103", "System error: Exec format error",
											"0509-026", "System error: Cannot run a file that does not have a valid format",
											NULL};

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsl_open_shared_library(sharedLibName, &handle, OMRPORT_SLOPEN_DECORATE);
	if (0 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "Unexpectedly loaded %s, should have failed with dependency error\n", sharedLibName, omrerror_last_error_message());
		goto exit;
	}

	osErrMsg = omrerror_last_error_message();
	outputComment(OMRPORTLIB, "System error message=\n\"%s\"\n", osErrMsg);
	if (!isValidLoadErrorMessage(osErrMsg, possibleMessageStrings)) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "Cannot find valid error code, should have failed with wrong platform error\n", sharedLibName, omrerror_last_error_message());
	}
exit:

	reportTestExit(OMRPORTLIB, testName);
}
#endif /* defined(AIXPPC) */

