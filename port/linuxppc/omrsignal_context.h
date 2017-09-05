/*******************************************************************************
 * Copyright (c) 1991, 2015 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code is also Distributed under one or more Secondary Licenses,
 * as those terms are defined by the Eclipse Public License, v. 2.0: GNU
 * General Public License, version 2 with the GNU Classpath Exception [1]
 * and GNU General Public License, version 2 with the OpenJDK Assembly
 * Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * Contributors:
 *   Multiple authors (IBM Corp.) - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "omrport.h"


#define MAX_UNIX_SIGNAL_TYPES  _NSIG

#define __USE_GNU 1
#include <dlfcn.h>
#undef __USE_GNU

/*
 * User context structure
 * The user context structure has the same layout as a
 * sigcontext structure until the uc_link structure, which
 * follows the sigcontext structure.
 */
typedef struct J9PlatformSignalInfo {
	ucontext_t *context;
#if 0
	/* This neutered chunk of code is left here for reference for the underlying types accessed
	 * via context->uc_mcontext.
	 */
#if defined(LINUXPPC64)
	struct sigcontext *ucmContext;
#else
	union uc_regs_ptr *ucmContext;
#endif
#endif
	Dl_info dl_info;

} J9PlatformSignalInfo;

typedef struct J9UnixSignalInfo {
	struct J9PlatformSignalInfo platformSignalInfo;
	uint32_t portLibrarySignalType;
	void *handlerAddress;
	void *handlerAddress2;
	siginfo_t *sigInfo;
} J9UnixSignalInfo;

uint32_t infoForFPR(struct OMRPortLibrary *portLibrary, struct J9UnixSignalInfo *info, int32_t index, const char **name, void **value);
uint32_t infoForGPR(struct OMRPortLibrary *portLibrary, struct J9UnixSignalInfo *info, int32_t index, const char **name, void **value);
uint32_t infoForModule(struct OMRPortLibrary *portLibrary, struct J9UnixSignalInfo *info, int32_t index, const char **name, void **value);
uint32_t infoForControl(struct OMRPortLibrary *portLibrary, struct J9UnixSignalInfo *info, int32_t index, const char **name, void **value);
uint32_t infoForSignal(struct OMRPortLibrary *portLibrary, struct J9UnixSignalInfo *info, int32_t index, const char **name, void **value);
void fillInUnixSignalInfo(struct OMRPortLibrary *portLibrary, void *contextInfo, struct J9UnixSignalInfo *j9Info);


