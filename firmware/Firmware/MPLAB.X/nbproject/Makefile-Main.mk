#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Main.mk)" "nbproject/Makefile-local-Main.mk"
include nbproject/Makefile-local-Main.mk
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Main
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/713068069/usb_device.o ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/usb_descriptors.o ${OBJECTDIR}/_ext/1472/Led.o ${OBJECTDIR}/_ext/1472/ISR.o ${OBJECTDIR}/_ext/1472/Delay.o ${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o ${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o ${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o ${OBJECTDIR}/_ext/1472/SampleFastestSingle.o ${OBJECTDIR}/_ext/1472/usb_functions.o ${OBJECTDIR}/_ext/1472/SampleScope.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/713068069/usb_device.o.d ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d ${OBJECTDIR}/_ext/1472/Led.o.d ${OBJECTDIR}/_ext/1472/ISR.o.d ${OBJECTDIR}/_ext/1472/Delay.o.d ${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o.d ${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o.d ${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o.d ${OBJECTDIR}/_ext/1472/SampleFastestSingle.o.d ${OBJECTDIR}/_ext/1472/usb_functions.o.d ${OBJECTDIR}/_ext/1472/SampleScope.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/713068069/usb_device.o ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/usb_descriptors.o ${OBJECTDIR}/_ext/1472/Led.o ${OBJECTDIR}/_ext/1472/ISR.o ${OBJECTDIR}/_ext/1472/Delay.o ${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o ${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o ${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o ${OBJECTDIR}/_ext/1472/SampleFastestSingle.o ${OBJECTDIR}/_ext/1472/usb_functions.o ${OBJECTDIR}/_ext/1472/SampleScope.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Main.mk dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4455
MP_PROCESSOR_OPTION_LD=18f4455
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0x5dc0 -u_DEBUGCODELEN=0x240 -u_DEBUGDATASTART=0x3f4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/Led.o: ../Led.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Led.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/Led.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -d__DEBUG -d__MPLAB_DEBUGGER_PK3=1 -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/Led.lst\" -e\"${OBJECTDIR}/_ext/1472/Led.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/Led.o\" ../Led.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Led.o 
	
${OBJECTDIR}/_ext/1472/Delay.o: ../Delay.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Delay.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/Delay.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -d__DEBUG -d__MPLAB_DEBUGGER_PK3=1 -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/Delay.lst\" -e\"${OBJECTDIR}/_ext/1472/Delay.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/Delay.o\" ../Delay.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Delay.o 
	
${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o: ../SampleDelayedInterleaved.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -d__DEBUG -d__MPLAB_DEBUGGER_PK3=1 -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o\" ../SampleDelayedInterleaved.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o 
	
${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o: ../SampleDelayedSingle.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleDelayedSingle.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -d__DEBUG -d__MPLAB_DEBUGGER_PK3=1 -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleDelayedSingle.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleDelayedSingle.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o\" ../SampleDelayedSingle.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o 
	
${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o: ../SampleFastestInterleaved.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -d__DEBUG -d__MPLAB_DEBUGGER_PK3=1 -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o\" ../SampleFastestInterleaved.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o 
	
${OBJECTDIR}/_ext/1472/SampleFastestSingle.o: ../SampleFastestSingle.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleFastestSingle.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleFastestSingle.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -d__DEBUG -d__MPLAB_DEBUGGER_PK3=1 -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleFastestSingle.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleFastestSingle.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleFastestSingle.o\" ../SampleFastestSingle.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleFastestSingle.o 
	
else
${OBJECTDIR}/_ext/1472/Led.o: ../Led.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Led.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/Led.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/Led.lst\" -e\"${OBJECTDIR}/_ext/1472/Led.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/Led.o\" ../Led.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Led.o 
	
${OBJECTDIR}/_ext/1472/Delay.o: ../Delay.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Delay.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/Delay.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/Delay.lst\" -e\"${OBJECTDIR}/_ext/1472/Delay.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/Delay.o\" ../Delay.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/Delay.o 
	
${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o: ../SampleDelayedInterleaved.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o\" ../SampleDelayedInterleaved.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleDelayedInterleaved.o 
	
${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o: ../SampleDelayedSingle.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleDelayedSingle.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleDelayedSingle.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleDelayedSingle.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o\" ../SampleDelayedSingle.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleDelayedSingle.o 
	
${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o: ../SampleFastestInterleaved.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o\" ../SampleFastestInterleaved.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleFastestInterleaved.o 
	
${OBJECTDIR}/_ext/1472/SampleFastestSingle.o: ../SampleFastestSingle.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleFastestSingle.o.d 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/_ext/1472/SampleFastestSingle.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -q -p$(MP_PROCESSOR_OPTION) -u  -l\"${OBJECTDIR}/_ext/1472/SampleFastestSingle.lst\" -e\"${OBJECTDIR}/_ext/1472/SampleFastestSingle.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/_ext/1472/SampleFastestSingle.o\" ../SampleFastestSingle.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleFastestSingle.o 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/713068069/usb_device.o: ../../Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/713068069 
	@${RM} ${OBJECTDIR}/_ext/713068069/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/713068069/usb_device.o   ../../Microchip/USB/usb_device.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/713068069/usb_device.o 
	
${OBJECTDIR}/_ext/1702484533/usb_function_hid.o: ../../Microchip/USB/HID\ Device\ Driver/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1702484533 
	@${RM} ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o   "../../Microchip/USB/HID Device Driver/usb_function_hid.c" 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/main.o   ../main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/main.o 
	
${OBJECTDIR}/_ext/1472/usb_descriptors.o: ../usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/usb_descriptors.o   ../usb_descriptors.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usb_descriptors.o 
	
${OBJECTDIR}/_ext/1472/ISR.o: ../ISR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ISR.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/ISR.o   ../ISR.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/ISR.o 
	
${OBJECTDIR}/_ext/1472/usb_functions.o: ../usb_functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_functions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/usb_functions.o   ../usb_functions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usb_functions.o 
	
${OBJECTDIR}/_ext/1472/SampleScope.o: ../SampleScope.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleScope.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/SampleScope.o   ../SampleScope.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleScope.o 
	
else
${OBJECTDIR}/_ext/713068069/usb_device.o: ../../Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/713068069 
	@${RM} ${OBJECTDIR}/_ext/713068069/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/713068069/usb_device.o   ../../Microchip/USB/usb_device.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/713068069/usb_device.o 
	
${OBJECTDIR}/_ext/1702484533/usb_function_hid.o: ../../Microchip/USB/HID\ Device\ Driver/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1702484533 
	@${RM} ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o   "../../Microchip/USB/HID Device Driver/usb_function_hid.c" 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1702484533/usb_function_hid.o 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/main.o   ../main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/main.o 
	
${OBJECTDIR}/_ext/1472/usb_descriptors.o: ../usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/usb_descriptors.o   ../usb_descriptors.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usb_descriptors.o 
	
${OBJECTDIR}/_ext/1472/ISR.o: ../ISR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ISR.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/ISR.o   ../ISR.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/ISR.o 
	
${OBJECTDIR}/_ext/1472/usb_functions.o: ../usb_functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_functions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/usb_functions.o   ../usb_functions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usb_functions.o 
	
${OBJECTDIR}/_ext/1472/SampleScope.o: ../SampleScope.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/SampleScope.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I".." -I"../../Microchip/Include"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/1472/SampleScope.o   ../SampleScope.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/SampleScope.o 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"osci.map" -i  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    -i
else
dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"osci.map" -i  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    -i
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Main
	${RM} -r dist/Main

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
