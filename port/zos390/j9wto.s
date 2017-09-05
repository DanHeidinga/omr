***********************************************************************
* Copyright (c) 1991, 2014 IBM Corp. and others
* 
* This program and the accompanying materials are made available under
* the terms of the Eclipse Public License 2.0 which accompanies this
* distribution and is available at 
* https://www.eclipse.org/legal/epl-1.0/ or the Apache License, 
* Version 2.0 which accompanies this distribution and is available at  
* https://www.apache.org/licenses/LICENSE-2.0.
* 
* This Source Code is also Distributed under one or more Secondary 
* Licenses, as those terms are defined by the Eclipse Public License, 
*  v. 2.0: GNU General Public License, version 2 with the GNU 
* Classpath Exception [1] and GNU General Public License, version 2 
* with the OpenJDK Assembly Exception [2].
* 
* [1] https://www.gnu.org/software/classpath/license.html
* [2] http://openjdk.java.net/legal/assembly-exception.html
*
* Contributors:
*   Multiple authors (IBM Corp.) - initial API and implementation 
*   and/or initial documentation
* 
***********************************************************************

         TITLE 'j9wto.s'

         AIF ('&SYSPARM' EQ 'BIT64').JMP1
_WTO     EDCXPRLG BASEREG=8
         LR    3,1
         AGO .JMP2
.JMP1    ANOP
_WTO     CELQPRLG BASEREG=8
         LR    3,1
         SAM31
.JMP2    ANOP
         WTO   MF=(E,(3))
         AIF ('&SYSPARM' EQ 'BIT64').JMP3
         EDCXEPLG
         AGO .JMP4
.JMP3    ANOP
         SAM64
         CELQEPLG
.JMP4    ANOP
*
         END
