/*******************************************************************************
 * Copyright (c) 2016, 2016 IBM Corp. and others
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


/**
 * Description: An non-extensible class between two extensible classes,
 *    in an extensible class hierarchy, which is not allowed. 
 */

#define OMR_EXTENSIBLE __attribute__((annotate("OMR_Extensible")))

namespace OMR { class OMR_EXTENSIBLE Class1Ext {}; }
namespace TR  { class OMR_EXTENSIBLE Class1Ext : public OMR::Class1Ext {}; }

namespace OMR { class Class2NonExt : public TR::Class1Ext {}; }
namespace TR  { class Class2NonExt : public OMR::Class2NonExt {}; }

namespace OMR { class OMR_EXTENSIBLE Class3Ext : public TR::Class2NonExt {}; }
namespace TR  { class OMR_EXTENSIBLE Class3Ext : public OMR::Class3Ext {}; }
