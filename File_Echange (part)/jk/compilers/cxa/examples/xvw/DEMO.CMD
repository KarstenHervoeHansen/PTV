/* @(#)demo.cmd	1.1 */
/* CrossView command play back file for demo.abs application.  */
/* This demo.abs application intentionally contains some bugs. */
/* When the demo.abs application is loaded into CrossView      */
/* you can run this play back file. The commands in this       */
/* play back file are work-arounds for these intentional bugs. */

/*---------------------------------------------------------------*/
/* The demo.abs application contains a loop that writes after an */
/* array. The loop counter 'loopvar' must end at 7 and not at 8. */
/* The breakpoint will test whether 'loopvar' is greater than or */
/* equal to 8.  If it is, the commands 'g 63' (change executing  */
/* position to line 63) and 'C' (continue) will be executed;     */
/* otherwise, just 'C' (continue) will be executed.              */
/* The 'Q' command suppresses the default echoing of the source  */
/* line when the breakpoint is hit.                              */
/*---------------------------------------------------------------*/
main#57+2 bi {Q; if (loopvar >= 8) {g 63; C} {C}}

/*---------------------------------------------------------------*/
/* The demo.abs application contains an infinite loop.           */
/* We can create a patch which skips over this loop              */
/* by setting a breakpoint at line 87 with a command list        */
/* containing the commands 'loopvar = 0' and 'C' (continue):     */
/*---------------------------------------------------------------*/
main#78 bi {loopvar = 0; C}
