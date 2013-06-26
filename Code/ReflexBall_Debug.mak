#
# ZDS II Make File - ReflexBall, Debug
#

CC = @C:\PROGRA~1\ZiLOG\ZDSII_~1.3\bin\eZ8cc
ASM = @C:\PROGRA~1\ZiLOG\ZDSII_~1.3\bin\eZ8asm
LINK = @C:\PROGRA~1\ZiLOG\ZDSII_~1.3\bin\eZ8link
LIB = @C:\PROGRA~1\ZiLOG\ZDSII_~1.3\bin\eZ8lib

CFLAGS =  \
 -alias -asm -bitfieldsize:32 -charsize:8 -const:RAM -debug  \
 -define:_Z8F6403 -define:_Z8ENCORE_640_FAMILY  \
 -define:_Z8ENCORE_F640X -define:_MODEL_LARGE -doublesize:32  \
 -NOexpmac -floatsize:32 -NOfplib -genprintf -NOglobalcopy  \
 -NOglobalcse -NOglobalfold -intrinsic -intsize:16 -intsrc  \
 -NOjmpopt -NOkeepasm -NOkeeplst -NOlist -NOlistinc -localcopy  \
 -localcse -localfold -longsize:32 -NOloopopt -maxerrs:50 -model:L  \
 -NOoptlink -optsize -peephole -NOpromote -quiet -regvar -revaa  \
 -NOsdiopt -shortsize:16  \
 -stdinc:"C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std;C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog;C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\Z8Encore_F640X"  \
 -strict -NOwatch -cpu:Z8F6403  \
 -asmsw:" -cpu:Z8F6403 -define:_Z8F6403=1 -define:_Z8ENCORE_640_FAMILY=1 -define:_Z8ENCORE_F640X=1 -define:_MODEL_LARGE=1 -include:C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std;C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog;C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\Z8Encore_F640X -revaa"

AFLAGS =  \
 -debug -define:_Z8F6403=1 -define:_Z8ENCORE_640_FAMILY=1  \
 -define:_Z8ENCORE_F640X=1 -define:_MODEL_LARGE=1 -genobj  \
 -NOigcase  \
 -include:"C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std;C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog;C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\Z8Encore_F640X"  \
 -list -NOlistmac -name -pagelen:56 -pagewidth:80 -quiet -sdiopt  \
 -warn -NOzmasm -revaa -cpu:Z8F6403

OUTDIR = L:\PHQFBI~D\ROIZBJ~K\

Debug: ReflexBall

deltarget: 
	@if exist L:\PHQFBI~D\ROIZBJ~K\ReflexBall.lod  \
            del L:\PHQFBI~D\ROIZBJ~K\ReflexBall.lod

clean: 
	@if exist L:\PHQFBI~D\ROIZBJ~K\ReflexBall.lod  \
            del L:\PHQFBI~D\ROIZBJ~K\ReflexBall.lod
	@if exist L:\PHQFBI~D\ROIZBJ~K\zsldevinit.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\zsldevinit.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\time.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\time.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\ansi.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\ansi.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\LED.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\LED.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\lut.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\lut.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\main.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\main.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\math.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\math.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\reflexball.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\reflexball.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\gameport.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\gameport.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\startScreen.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\startScreen.obj
	@if exist L:\PHQFBI~D\ROIZBJ~K\ascii.obj  \
            del L:\PHQFBI~D\ROIZBJ~K\ascii.obj

rebuildall: clean Debug

relink: deltarget Debug

LIBS = 

OBJS =  \
            L:\PHQFBI~D\ROIZBJ~K\zsldevinit.obj  \
            L:\PHQFBI~D\ROIZBJ~K\time.obj  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.obj  \
            L:\PHQFBI~D\ROIZBJ~K\LED.obj  \
            L:\PHQFBI~D\ROIZBJ~K\lut.obj  \
            L:\PHQFBI~D\ROIZBJ~K\main.obj  \
            L:\PHQFBI~D\ROIZBJ~K\math.obj  \
            L:\PHQFBI~D\ROIZBJ~K\reflexball.obj  \
            L:\PHQFBI~D\ROIZBJ~K\gameport.obj  \
            L:\PHQFBI~D\ROIZBJ~K\startScreen.obj  \
            L:\PHQFBI~D\ROIZBJ~K\ascii.obj

ReflexBall: $(OBJS)
	 $(LINK)  @L:\PHQFBI~D\ROIZBJ~K\ReflexBall_Debug.linkcmd

L:\PHQFBI~D\ROIZBJ~K\zsldevinit.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\zsldevinit.asm  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8dev.inc  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.inc
	 $(ASM)  $(AFLAGS) L:\PHQFBI~D\ROIZBJ~K\zsldevinit.asm

L:\PHQFBI~D\ROIZBJ~K\time.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\time.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.h  \
            L:\PHQFBI~D\ROIZBJ~K\time.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\time.c

L:\PHQFBI~D\ROIZBJ~K\ansi.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\ansi.c

L:\PHQFBI~D\ROIZBJ~K\LED.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\LED.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\LED.h  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.h  \
            L:\PHQFBI~D\ROIZBJ~K\charset.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\LED.c

L:\PHQFBI~D\ROIZBJ~K\lut.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\lut.c  \
            L:\PHQFBI~D\ROIZBJ~K\lut.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\lut.c

L:\PHQFBI~D\ROIZBJ~K\main.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\main.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\LED.h  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.h  \
            L:\PHQFBI~D\ROIZBJ~K\ascii.h  \
            L:\PHQFBI~D\ROIZBJ~K\gameport.h  \
            L:\PHQFBI~D\ROIZBJ~K\math.h  \
            L:\PHQFBI~D\ROIZBJ~K\ROIZBJ~K.H  \
            L:\PHQFBI~D\ROIZBJ~K\SBD22I~Z.H  \
            L:\PHQFBI~D\ROIZBJ~K\time.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\main.c

L:\PHQFBI~D\ROIZBJ~K\math.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\math.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\lut.h  \
            L:\PHQFBI~D\ROIZBJ~K\math.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\math.c

L:\PHQFBI~D\ROIZBJ~K\reflexball.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\reflexball.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\LED.h  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.h  \
            L:\PHQFBI~D\ROIZBJ~K\ascii.h  \
            L:\PHQFBI~D\ROIZBJ~K\levels.h  \
            L:\PHQFBI~D\ROIZBJ~K\math.h  \
            L:\PHQFBI~D\ROIZBJ~K\ROIZBJ~K.H  \
            L:\PHQFBI~D\ROIZBJ~K\SBD22I~Z.H  \
            L:\PHQFBI~D\ROIZBJ~K\time.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\reflexball.c

L:\PHQFBI~D\ROIZBJ~K\gameport.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\gameport.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.h  \
            L:\PHQFBI~D\ROIZBJ~K\gameport.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\gameport.c

L:\PHQFBI~D\ROIZBJ~K\startScreen.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\startScreen.c  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\FORMAT.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\std\STDARG.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\defines.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\dmadefs.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\ez8.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\gpio.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\SIO.H  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uart.h  \
            C:\PROGRA~1\ZiLOG\ZDSII_~1.3\include\zilog\uartdefs.h  \
            L:\PHQFBI~D\ROIZBJ~K\LED.h  \
            L:\PHQFBI~D\ROIZBJ~K\ansi.h  \
            L:\PHQFBI~D\ROIZBJ~K\ascii.h  \
            L:\PHQFBI~D\ROIZBJ~K\gameport.h  \
            L:\PHQFBI~D\ROIZBJ~K\math.h  \
            L:\PHQFBI~D\ROIZBJ~K\ROIZBJ~K.H  \
            L:\PHQFBI~D\ROIZBJ~K\SBD22I~Z.H  \
            L:\PHQFBI~D\ROIZBJ~K\time.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\startScreen.c

L:\PHQFBI~D\ROIZBJ~K\ascii.obj :  \
            L:\PHQFBI~D\ROIZBJ~K\ascii.c  \
            L:\PHQFBI~D\ROIZBJ~K\ascii.h
	 $(CC)  $(CFLAGS) L:\PHQFBI~D\ROIZBJ~K\ascii.c

