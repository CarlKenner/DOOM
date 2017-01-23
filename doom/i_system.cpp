/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

// $Log:$
//
// DESCRIPTION:

===========================================================================
*/

static const char
rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdarg.h>
#include <sys/time.h>
#include <unistd.h>

#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"

#include "d_net.h"
#include "g_game.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"




int	mb_used = 6;


void
I_Tactile
( int	on,
 int	off,
 int	total )
{
  // UNUSED.
 on = off = total = 0;
}

ticcmd_t	emptycmd;
ticcmd_t*	I_BaseTiccmd(void)
{
	return &emptycmd;
}


int  I_GetHeapSize (void)
{
	return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{
	*size = mb_used*1024*1024;
	return (byte *) malloc (*size);
}



//
// I_GetTime
// returns time in 1/70th second tics
//
int  I_GetTime (void)
{
	struct timeval	tp;
	struct timezone	tzp;
	int			newtics;
	static int		basetime=0;
 
	gettimeofday(&tp, &tzp);
	if (!basetime)
	basetime = tp.tv_sec;
	newtics = (tp.tv_sec-basetime)*TICRATE + tp.tv_usec*TICRATE/1000000;
	return newtics;
}



//
// I_Init
//
void I_Init (void)
{
	I_InitSound();
	//  I_InitGraphics();
}

//
// I_Quit
//
void I_Quit (void)
{
	D_QuitNetGame ();
	I_ShutdownSound();
	I_ShutdownMusic();
	M_SaveDefaults ();
	I_ShutdownGraphics();
	exit(0);
}

void I_WaitVBL(int count)
{
#ifdef SGI
	sginap(1);                                           
#else
#ifdef SUN
	sleep(0);
#else
	usleep (count * (1000000/70) );                                
#endif
#endif
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

byte*	I_AllocLow(int length)
{
	byte*	mem;
		
	mem = (byte *)malloc (length);
	memset (mem,0,length);
	return mem;
}


//
// I_Error
//
extern boolean demorecording;

void I_Error (char *error, ...)
{
	va_list	argptr;

	// Message first.
	va_start (argptr,error);
	fprintf (stderr, "Error: ");
	vfprintf (stderr,error,argptr);
	fprintf (stderr, "\n");
	va_end (argptr);

	fflush( stderr );

	// Shutdown. Here might be other errors.
	if (demorecording)
	G_CheckDemoStatus();

	D_QuitNetGame ();
	I_ShutdownGraphics();
	
	exit(-1);
}
