/*             ----> DO NOT REMOVE THE FOLLOWING NOTICE <----

                  Copyright (c) 2014-2022 Tuxera US Inc.
                      All Rights Reserved Worldwide.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; use version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but "AS-IS," WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/*  Businesses and individuals that for commercial or other reasons cannot
    comply with the terms of the GPLv2 license must obtain a commercial
    license before incorporating Reliance Edge into proprietary software
    for distribution in any form.

    Visit https://www.tuxera.com/products/reliance-edge/ for more information.
*/
/** @file
    @brief Compile-time validity checks for the REDCONF macros.
*/
#ifndef REDCONFIGCHK_H
#define REDCONFIGCHK_H

#ifdef RED_CONFIG_MINCOMPAT_VER
  #if (RED_VERSION_VAL & 0xFFFFFF00U) < (RED_CONFIG_MINCOMPAT_VER & 0xFFFFFF00U)
    /*  This indicates that your configuration files were generated by a
        version of the Reliance Edge Configuration Utility that is designed
        for a more recent version of Reliance Edge and is no longer compatible
        with this version.  You can update to the most recent version of
        Reliance Edge or contact support@tuxera.com to obtain the correct legacy
        version of the Configuration Utility.
    */
    #error "Your configuration is not compatible with this version of Reliance Edge. Please download the latest version of Reliance Edge or recreate your configuration with an older version of the Configuration Utility."
  #endif
#endif

#ifndef REDCONF_READ_ONLY
  #error "Configuration error: REDCONF_READ_ONLY must be defined."
#endif
#ifndef REDCONF_API_POSIX
  #error "Configuration error: REDCONF_API_POSIX must be defined."
#endif
#ifndef REDCONF_API_FSE
  #error "Configuration error: REDCONF_API_FSE must be defined."
#endif

#if REDCONF_API_POSIX == 1
  #ifndef REDCONF_API_POSIX_FORMAT
    #error "Configuration error: REDCONF_API_POSIX_FORMAT must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_UNLINK
    #error "Configuration error: REDCONF_API_POSIX_UNLINK must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_MKDIR
    #error "Configuration error: REDCONF_API_POSIX_MKDIR must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_RMDIR
    #error "Configuration error: REDCONF_API_POSIX_RMDIR must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_RENAME
    #error "Configuration error: REDCONF_API_POSIX_RENAME must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_LINK
    #error "Configuration error: REDCONF_API_POSIX_LINK must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_FTRUNCATE
    #error "Configuration error: REDCONF_API_POSIX_FTRUNCATE must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_FRESERVE
    #error "Configuration error: REDCONF_API_POSIX_FRESERVE must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_READDIR
    #error "Configuration error: REDCONF_API_POSIX_READDIR must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_CWD
    #error "Configuration error: REDCONF_API_POSIX_CWD must be defined."
  #endif
  #ifndef REDCONF_API_POSIX_FSTRIM
    #error "Configuration error: REDCONF_API_POSIX_FSTRIM must be defined."
  #endif
  #ifndef REDCONF_NAME_MAX
    #error "Configuration error: REDCONF_NAME_MAX must be defined."
  #endif
  #ifndef REDCONF_PATH_SEPARATOR
    #error "Configuration error: REDCONF_PATH_SEPARATOR must be defined."
  #endif
  #ifndef REDCONF_RENAME_ATOMIC
    #error "Configuration error: REDCONF_RENAME_ATOMIC must be defined."
  #endif
  #ifndef REDCONF_HANDLE_COUNT
    #error "Configuration error: REDCONF_HANDLE_COUNT must be defined."
  #endif
#endif
#if REDCONF_API_FSE == 1
  #ifndef REDCONF_API_FSE_FORMAT
    #error "Configuration error: REDCONF_API_FSE_FORMAT must be defined."
  #endif
  #ifndef REDCONF_API_FSE_TRUNCATE
    #error "Configuration error: REDCONF_API_FSE_TRUNCATE must be defined."
  #endif
  #ifndef REDCONF_API_FSE_TRANSMASKSET
    #error "Configuration error: REDCONF_API_FSE_TRANSMASKSET must be defined."
  #endif
  #ifndef REDCONF_API_FSE_TRANSMASKGET
    #error "Configuration error: REDCONF_API_FSE_TRANSMASKGET must be defined."
  #endif
#endif

/*  Because of how these are used in rednodes.h, they need to be defined in all
    configurations (not just POSIX configurations).
*/
#ifndef REDCONF_API_POSIX_SYMLINK
  #error "Configuration error: REDCONF_API_POSIX_SYMLINK must be defined."
#endif
#ifndef REDCONF_POSIX_OWNER_PERM
  #error "Configuration error: REDCONF_POSIX_OWNER_PERM must be defined."
#endif
#ifndef REDCONF_DELETE_OPEN
  #error "Configuration error: REDCONF_DELETE_OPEN must be defined."
#endif

#ifndef REDCONF_TASK_COUNT
  #error "Configuration error: REDCONF_TASK_COUNT must be defined."
#endif
#ifndef REDCONF_ENDIAN_BIG
  #error "Configuration error: REDCONF_ENDIAN_BIG must be defined."
#endif
#ifndef REDCONF_ALIGNMENT_SIZE
  #error "Configuration error: REDCONF_ALIGNMENT_SIZE must be defined."
#endif
#ifndef REDCONF_CRC_ALGORITHM
  #error "Configuration error: REDCONF_CRC_ALGORITHM must be defined."
#endif
#ifndef REDCONF_INODE_TIMESTAMPS
  #error "Configuration error: REDCONF_INODE_TIMESTAMPS must be defined."
#endif
#ifndef REDCONF_ATIME
  #error "Configuration error: REDCONF_ATIME must be defined."
#endif
#ifndef REDCONF_DIRECT_POINTERS
  #error "Configuration error: REDCONF_DIRECT_POINTERS must be defined."
#endif
#ifndef REDCONF_INDIRECT_POINTERS
  #error "Configuration error: REDCONF_INDIRECT_POINTERS must be defined."
#endif
#ifndef REDCONF_INODE_BLOCKS
  #error "Configuration error: REDCONF_INODE_BLOCKS must be defined."
#endif
#ifndef REDCONF_IMAP_EXTERNAL
  #error "Configuration error: REDCONF_IMAP_EXTERNAL must be defined."
#endif
#ifndef REDCONF_IMAP_INLINE
  #error "Configuration error: REDCONF_IMAP_INLINE must be defined."
#endif
#ifndef REDCONF_OUTPUT
  #error "Configuration error: REDCONF_OUTPUT must be defined."
#endif
#ifndef REDCONF_ASSERTS
  #error "Configuration error: REDCONF_ASSERTS must be defined."
#endif
#ifndef REDCONF_TRANSACT_DEFAULT
  #error "Configuration error: REDCONF_TRANSACT_DEFAULT must be defined."
#endif
#ifndef REDCONF_BUFFER_COUNT
  #error "Configuration error: REDCONF_BUFFER_COUNT must be defined."
#endif
#ifndef REDCONF_BUFFER_ALIGNMENT
  #error "Configuration error: REDCONF_BUFFER_ALIGNMENT must be defined."
#endif
#ifndef REDCONF_BUFFER_WRITE_GATHER_SIZE_KB
  #error "Configuration error: REDCONF_BUFFER_WRITE_GATHER_SIZE_KB must be defined."
#endif
#ifndef REDCONF_BLOCK_SIZE
  #error "Configuration error: REDCONF_BLOCK_SIZE must be defined."
#endif
#ifndef REDCONF_VOLUME_COUNT
  #error "Configuration error: REDCONF_VOLUME_COUNT must be defined."
#endif
#ifndef REDCONF_DISCARDS
  #error "Configuration error: REDCONF_DISCARDS must be defined."
#endif
#ifndef REDCONF_IMAGE_BUILDER
  #error "Configuration error: REDCONF_IMAGE_BUILDER must be defined."
#endif
#ifndef REDCONF_CHECKER
  #error "Configuration error: REDCONF_CHECKER must be defined."
#endif

#if (REDCONF_READ_ONLY != 0) && (REDCONF_READ_ONLY != 1)
  #error "Configuration error: REDCONF_READ_ONLY must be either 0 or 1"
#endif

#if (REDCONF_API_POSIX != 0) && (REDCONF_API_POSIX != 1)
  #error "Configuration error: REDCONF_API_POSIX must be either 0 or 1."
#endif
#if (REDCONF_API_FSE != 0) && (REDCONF_API_FSE != 1)
  #error "Configuration error: REDCONF_API_FSE must be either 0 or 1."
#endif

#if (REDCONF_API_FSE == 0) && (REDCONF_API_POSIX == 0)
  #error "Configuration error: either REDCONF_API_FSE or REDCONF_API_POSIX must be set to 1."
#endif

#if REDCONF_API_POSIX == 1
  #if REDCONF_API_FSE != 0
    #error "Configuration error: REDCONF_API_FSE must be 0 if REDCONF_API_POSIX is 1"
  #endif

  #if (REDCONF_API_POSIX_FORMAT != 0) && (REDCONF_API_POSIX_FORMAT != 1)
    #error "Configuration error: REDCONF_API_POSIX_FORMAT must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_UNLINK != 0) && (REDCONF_API_POSIX_UNLINK != 1)
    #error "Configuration error: REDCONF_API_POSIX_UNLINK must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_MKDIR != 0) && (REDCONF_API_POSIX_MKDIR != 1)
    #error "Configuration error: REDCONF_API_POSIX_MKDIR must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_RMDIR != 0) && (REDCONF_API_POSIX_RMDIR != 1)
    #error "Configuration error: REDCONF_API_POSIX_RMDIR must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_RENAME != 0) && (REDCONF_API_POSIX_RENAME != 1)
    #error "Configuration error: REDCONF_API_POSIX_RENAME must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_LINK != 0) && (REDCONF_API_POSIX_LINK != 1)
    #error "Configuration error: REDCONF_API_POSIX_LINK must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_FTRUNCATE != 0) && (REDCONF_API_POSIX_FTRUNCATE != 1)
    #error "Configuration error: REDCONF_API_POSIX_FTRUNCATE must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_FRESERVE != 0) && (REDCONF_API_POSIX_FRESERVE != 1)
    #error "Configuration error: REDCONF_API_POSIX_FRESERVE must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_READDIR != 0) && (REDCONF_API_POSIX_READDIR != 1)
    #error "Configuration error: REDCONF_API_POSIX_READDIR must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_CWD != 0) && (REDCONF_API_POSIX_CWD != 1)
    #error "Configuration error: REDCONF_API_POSIX_CWD must be either 0 or 1."
  #endif

  #if (REDCONF_API_POSIX_FSTRIM != 0) && (REDCONF_API_POSIX_FSTRIM != 1)
    #error "Configuration error: REDCONF_API_POSIX_FSTRIM must be either 0 or 1."
  #endif
  #if (REDCONF_API_POSIX_FSTRIM == 1) && (RED_KIT == RED_KIT_GPL)
    #error "REDCONF_API_POSIX_FSTRIM not supported in Reliance Edge under GPL. Contact sales@tuxera.com to upgrade."
  #endif

  /*  Maximum is either block size minus 4 or block size minus 20, depending on
      the on-disk layout version.  Since we don't know the layout version, check
      the looser limit.
  */
  #if (REDCONF_NAME_MAX < 1U) || (REDCONF_NAME_MAX > (REDCONF_BLOCK_SIZE - 4U))
    #error "Configuration error: invalid value of REDCONF_NAME_MAX"
  #endif

  #if (REDCONF_PATH_SEPARATOR < 1) || (REDCONF_PATH_SEPARATOR > 127)
    #error "Configuration error: invalid value of REDCONF_PATH_SEPARATOR"
  #endif
  #if (REDCONF_API_POSIX_CWD == 1) && (REDCONF_PATH_SEPARATOR == '.')
    #error "Configuration error: REDCONF_PATH_SEPARATOR cannot be '.' when REDCONF_API_POSIX_CWD is 1."
  #endif

  #if (REDCONF_RENAME_ATOMIC != 0) && (REDCONF_RENAME_ATOMIC != 1)
    #error "Configuration error: REDCONF_RENAME_ATOMIC must be either 0 or 1."
  #endif

  #if (REDCONF_HANDLE_COUNT < 1U) || (REDCONF_HANDLE_COUNT > 4096U)
    #error "Configuration error: invalid value of REDCONF_HANDLE_COUNT"
  #endif
#endif
#if REDCONF_API_FSE == 1
  #if (REDCONF_API_FSE_FORMAT != 0) && (REDCONF_API_FSE_FORMAT != 1)
    #error "Configuration error: REDCONF_API_FSE_FORMAT must be either 0 or 1."
  #endif

  #if (REDCONF_API_FSE_TRUNCATE != 0) && (REDCONF_API_FSE_TRUNCATE != 1)
    #error "Configuration error: REDCONF_API_FSE_TRUNCATE must be either 0 or 1."
  #endif

  #if (REDCONF_API_FSE_TRANSMASKSET != 0) && (REDCONF_API_FSE_TRANSMASKSET != 1)
    #error "Configuration error: REDCONF_API_FSE_TRANSMASKSET must be either 0 or 1."
  #endif

  #if (REDCONF_API_FSE_TRANSMASKGET != 0) && (REDCONF_API_FSE_TRANSMASKGET != 1)
    #error "Configuration error: REDCONF_API_FSE_TRANSMASKGET must be either 0 or 1."
  #endif
#endif

#if (REDCONF_API_POSIX_SYMLINK != 0) && (REDCONF_API_POSIX_SYMLINK != 1)
  #error "Configuration error: REDCONF_API_POSIX_SYMLINK must be either 0 or 1."
#endif

#if (REDCONF_POSIX_OWNER_PERM != 0) && (REDCONF_POSIX_OWNER_PERM != 1)
  #error "Configuration error: REDCONF_POSIX_OWNER_PERM must be either 0 or 1."
#endif

#if (REDCONF_DELETE_OPEN != 0) && (REDCONF_DELETE_OPEN != 1)
  #error "Configuration error: REDCONF_DELETE_OPEN must be either 0 or 1."
#endif

#if REDCONF_TASK_COUNT < 1U
  #error "Configuration error: invalid value of REDCONF_TASK_COUNT"
#endif

#if (REDCONF_ENDIAN_BIG != 0) && (REDCONF_ENDIAN_BIG != 1)
  #error "Configuration error: REDCONF_ENDIAN_BIG must be either 0 or 1."
#endif

#if (REDCONF_ALIGNMENT_SIZE != 1U) && (REDCONF_ALIGNMENT_SIZE != 2U) && (REDCONF_ALIGNMENT_SIZE != 4U) && (REDCONF_ALIGNMENT_SIZE != 8U)
  #error "Configuration error: invalid value REDCONF_ALIGNMENT_SIZE"
#endif

/*  REDCONF_CRC_ALGORITHM checked in crc.c
*/

#if (REDCONF_INODE_TIMESTAMPS != 0) && (REDCONF_INODE_TIMESTAMPS != 1)
  #error "Configuration error: REDCONF_INODE_TIMESTAMPS must be either 0 or 1."
#endif

#if (REDCONF_ATIME != 0) && (REDCONF_ATIME != 1)
  #error "Configuration error: REDCONF_ATIME must be either 0 or 1."
#endif

#if (REDCONF_INODE_TIMESTAMPS == 0) && (REDCONF_ATIME == 1)
  #error "Configuration error: REDCONF_ATIME must be 0 when REDCONF_INODE_TIMESTAMPS is 0."
#endif

/*  REDCONF_DIRECT_POINTERS and REDCONF_INDIRECT_POINTERS checked in rednodes.h
*/

#if (REDCONF_INODE_BLOCKS != 0) && (REDCONF_INODE_BLOCKS != 1)
  #error "Configuration error: REDCONF_INODE_BLOCKS must be either 0 or 1."
#endif

/*  Further validity checking of imap specs done in RelCoreInit()
*/
#if (REDCONF_IMAP_EXTERNAL != 0) && (REDCONF_IMAP_EXTERNAL != 1)
  #error "Configuration error: REDCONF_IMAP_EXTERNAL must be either 0 or 1."
#endif
#if (REDCONF_IMAP_INLINE != 0) && (REDCONF_IMAP_INLINE != 1)
  #error "Configuration error: REDCONF_IMAP_INLINE must be either 0 or 1."
#endif
#if (REDCONF_IMAP_INLINE == 0) && (REDCONF_IMAP_EXTERNAL == 0)
  #error "Configuration error: At least one of REDCONF_IMAP_INLINE and REDCONF_IMAP_EXTERNAL must be set"
#endif

#if (REDCONF_OUTPUT != 0) && (REDCONF_OUTPUT != 1)
  #error "Configuration error: REDCONF_OUTPUT must be either 0 or 1."
#endif

#if (REDCONF_ASSERTS != 0) && (REDCONF_ASSERTS != 1)
  #error "Configuration error: REDCONF_ASSERTS must be either 0 or 1."
#endif

/*  REDCONF_BLOCK_SIZE checked in redmacs.h
*/

#if (REDCONF_VOLUME_COUNT < 1U) || (REDCONF_VOLUME_COUNT > 255U)
  #error "REDCONF_VOLUME_COUNT must be an integer between 1 and 255"
#endif

#if (REDCONF_DISCARDS != 0) && (REDCONF_DISCARDS != 1)
  #error "Configuration error: REDCONF_DISCARDS must be either 0 or 1."
#endif

#if (REDCONF_DISCARDS == 1) && (RED_KIT == RED_KIT_GPL)
  #error "REDCONF_DISCARDS not supported in Reliance Edge under GPL. Contact sales@tuxera.com to upgrade."
#endif

#if (REDCONF_IMAGE_BUILDER != 0) && (REDCONF_IMAGE_BUILDER != 1)
  #error "Configuration error: REDCONF_IMAGE_BUILDER must be either 0 or 1."
#endif

#if (REDCONF_CHECKER != 0) && (REDCONF_CHECKER != 1)
  #error "Configuration error: REDCONF_CHECKER must be either 0 or 1."
#endif


#endif
