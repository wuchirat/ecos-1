//==========================================================================
//
//      include/net/netisr.h
//
//      
//
//==========================================================================
// ####BSDALTCOPYRIGHTBEGIN####                                             
// -------------------------------------------                              
// Portions of this software may have been derived from OpenBSD             
// or other sources, and if so are covered by the appropriate copyright     
// and license included herein.                                             
// -------------------------------------------                              
// ####BSDALTCOPYRIGHTEND####                                               
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    gthomas
// Contributors: gthomas
// Date:         2000-01-10
// Purpose:      
// Description:  
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================


/*	$OpenBSD: netisr.h,v 1.14 1999/12/08 15:58:30 itojun Exp $	*/
/*	$NetBSD: netisr.h,v 1.12 1995/08/12 23:59:24 mycroft Exp $	*/

/*
 * Copyright (c) 1980, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)netisr.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NET_NETISR_H_
#define _NET_NETISR_H_

/*
 * The networking code runs off software interrupts.
 *
 * You can switch into the network by doing splsoftnet() and return by splx().
 * The software interrupt level for the network is higher than the software
 * level for the clock (so you can enter the network in routines called
 * at timeout time).
 *
 * The routine to request a network software interrupt, setsoftnet(),
 * is defined in the machine-specific include files.
 */

/*
 * Each ``pup-level-1'' input queue has a bit in a ``netisr'' status
 * word which is used to de-multiplex a single software
 * interrupt used for scheduling the network code to calls
 * on the lowest level routine of each protocol.
 */
#ifdef __ECOS
#define NETISR_SOFTNET  1               // Just used to force an interrupt
#endif
#define	NETISR_IP	2		/* same as AF_INET */
#define	NETISR_IMP	3		/* same as AF_IMPLINK */
#define	NETISR_NS	6		/* same as AF_NS */
#define	NETISR_ISO	7		/* same as AF_ISO */
#define	NETISR_CCITT	10		/* same as AF_CCITT */
#define	NETISR_ATALK	16		/* same as AF_APPLETALK */
#define	NETISR_ARP	18		/* same as AF_LINK */
#define	NETISR_IPX	23		/* same as AF_IPX */
#define	NETISR_IPV6	24		/* same as AF_INET6 */
#define	NETISR_ISDN	26		/* same as AF_E164 */
#define	NETISR_NATM	27		/* same as AF_ATM */
#define	NETISR_PPP	28		/* for PPP processing */
#define	NETISR_BRIDGE	29		/* for bridge processing */

#ifndef _LOCORE
#ifdef _KERNEL
#ifdef __ECOS
#include <cyg/kernel/kapi.h>
extern cyg_flag_t netint_flags;  
#else
int	netisr;				/* scheduling bits for network */
#endif

void	arpintr __P((void));
void	ipintr __P((void));
void	ip6intr __P((void));
void	atintr __P((void));
void	nsintr __P((void));
void	clnlintr __P((void));
void	natmintr __P((void));
void	pppintr __P((void));
void	ccittintr __P((void));
void	bridgeintr __P((void));

#ifdef __ECOS
#define schednetisr(anisr) cyg_flag_setbits(&netint_flags, 1<<(anisr));
#else
#include <dev/rndvar.h>

#define	schednetisr(anisr)	\
	{ netisr |= 1<<(anisr); add_net_randomness(anisr); setsoftnet(); }
#endif
#endif
#endif

#endif // _NET_NETISR_H_
