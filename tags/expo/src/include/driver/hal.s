#-------------------------------------------------------------------------------
# (c) Copyright 2007 Actel Corporation.  All rights reserved.
# 
# Interrupt disabling/restoration for critical section protection.
#
# SVN $Revision: 171 $
# SVN $Date: 2008-03-11 14:48:48 +0000 (Tue, 11 Mar 2008) $
#
	.text

#-------------------------------------------------------------------------------
# 
#
HAL_disable_interrupts:	.global HAL_disable_interrupts
	mrs r0, PRIMASK
    cpsid I
    bx lr

#-------------------------------------------------------------------------------
#
#
HAL_restore_interrupts:	.global HAL_restore_interrupts
	msr PRIMASK, r0
	bx lr

.end
