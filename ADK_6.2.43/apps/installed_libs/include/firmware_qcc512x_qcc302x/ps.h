#ifndef __PS_H__
#define __PS_H__
#include <app/ps/ps_if.h>

/*! file 
@brief Access to persistent store.
** 
A user application running on the VM can access the persistent store on 
BlueCore which provides a small amount of non-volatile storage.
** 
Up to two hundred keys can be used, 0 to 199. There are no predefined
meanings for these keys; they can be used for any purpose required by the user
application. However, the total amount of data associated with all the keys
should be kept as small as possible.
** 
These keys are divided into three blocks of equal size and function
- User configuration data (0 to 49). 
- DSP configuration (50 to 99).
- VM Connection Library data (100 to 149) NOTE: Applications MUST NOT use these.
- User configuration data, 2nd block (150 to 199).
** 
The first twenty-five in both blocks, 0 to 24 and 50 to 74, are protected
against upload and modification via DFU (Device Firmware Upgrade) unless
accompanied by a valid signature. Hence, they should be used for sensitive data
such as cryptographic keys.
** 
The maximum length of persistent store key that is guaranteed to be supported by
the BlueCore firmware and its supporting tools is 64 words. Attempts to use
longer persistent store keys may return failure or may cause undefined firmware
behaviour.
** 
@par Notes
The persistent store is normally held in flash memory. This means that only a
finite number of changes can be made to values held in persistent store without
performing a defragment of the flash. The defragment is a time and memory
intensive operation that is only performed at boot time. Hence the number of
changes to the value of a key should be kept to a minimum. If the return code
from PsStore() indicates a failure then the chip needs to be reset for a
defragment to occur. To control when defragmentation occurs, use PsFlood() and
BootSetMode().  When persistent store is held in EEPROM, write operations to
EEPROM are time consuming. So during Bluetooth activity, avoid back-to-back
multiple PSKEY reads and writes since this may cause undefined firmware
behaviour.
** 
@par Notes
Doing error checks on a key's value costs code space and usually has no
effect.If the value of a key gives an error then it should be detected while the
module is being designed (or by the tools if you have simple keys) and so the
error checking is redundant in the field. The only exception is where it's
possible to set a key in such a way that the chip doesn't boot. In this case,
the user may be unable to recover the module once they've set the key. In this
case, you may want to consider issuing a fault and falling back to sane values
if the key is wrong.  It may be worth considering overriding certain settings
depending on other keys as a way of avoiding synchronised settings.  If you end
up overriding a whole key then the key is redundant and can be removed. If you
override part of a key, say a bit fielded key, then it costs little code to
simply apply a mask to the value read from persistent store and then set bits
later.
*/

#if TRAPSET_CORE

/**
 *  \brief Copy the specified memory buffer to persistent store. 
 *   A Persistent store user key can be deleted by calling the PsStore function
 *   with \e key to be deleted and \e words argument being zero.
 *   \note
 *   The key is interpreted as an index into PSKEY_USR0 ... PSKEY_USR49,
 *   PSKEY_DSP0 ... PSKEY_DSP49, PSKEY_CONNLIB0 ... PSKEY_CONNLIB49,
 *   PSKEY_USR50 ... PSKEY_USR99,   PSKEY_CUSTOMER0 ... PSKEY_CUSTOMER99
 *   No other keys can be written from an application.
 *  \param key The persistent store key to copy data to. 
 *  \param buff The memory buffer to copy data from,
 *             must be word aligned. 
 *  \param words The number of uint16 words to copy.
 *  \return The number of words actually written.
 *           Zero if either 'words' is zero or the store failed.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 */
uint16 PsStore(uint16 key, const void * buff, uint16 words);

/**
 *  \brief Copy to specified memory buffer from persistent store. 
 *   \note
 *   The key is interpreted as an index into PSKEY_USR0 ... PSKEY_USR49,
 *   PSKEY_DSP0 ... PSKEY_DSP49, PSKEY_CONNLIB0 ... PSKEY_CONNLIB49,
 *   PSKEY_USR50 ... PSKEY_USR99,   PSKEY_CUSTOMER0 ... PSKEY_CUSTOMER99..
 *   No other keys can be written from an application.
 *  \param key The persistent store key to copy data from. 
 *  \param buff The memory buffer to copy data to,
 *             must be word aligned.
 *  \param words The number of uint16 words to copy. 
 *  \return If \e buff is NULL and \e words is zero,
 *           returns the minimum length in words of the buffer necessary to hold
 *           the contents of \e key. Otherwise, returns the number of words
 *           actually read, or zero if the key does not exist or is longer
 *           than \e words.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 */
uint16 PsRetrieve(uint16 key, void * buff, uint16 words);

/**
 *  \brief Flood fill the store to force a defragment
 *   Subsequent attempts to write to persistent store will fail until then.
 * 
 * \note This trap may be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 */
void PsFlood(void );

/**
 *  \brief Return how many keys of this size in words we could write. 
 *  \param words The key size in words to use.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 */
uint16 PsFreeCount(uint16 words);

/**
 *  \brief Read any persistent store key which could be accessed from off-chip using
 *  BCCMD. 
 *   Note that this uses full CSR PS key numbering, not the
 *   0..149 space used by the other Ps functions which are restricted to
 *   accessing the VM, DSP and connection library specific keys.
 *   If a key is present (has a value) in any store (e.g., ROM, factory),
 *   stores layered on top of that (e.g., EEPROM, RAM) cannot cause the key
 *   to be absent. Therefore, making some behaviour conditional on the absence
 *   of a key is something to be avoided if at all possible. If the key has a
 *   default value in ROM, your behaviour will be impossible to access.
 *   Note that you can override any value with a zero-length value,
 *   so testing for a zero-length value does not have this problem.
 *   Usually this means you can obey this rule by allowing a zero length key
 *   to behave the same as an absent key.
 *   The correct way to read a key which may be a zero length pskey may 
 *   will look like below
 *       uint16 pio = 0xffff;
 *       if (PsFullRetrieve(PSKEY_*., &pio, sizeof(pio)) && pio <= 15)
 *   The initialisation of pio gives the value if the key isn't present,
 *   PsFullRetrieve returns FALSE if the key doesn't exist.
 *  \param key The persistent store key to copy data from. 
 *  \param buff The memory buffer to copy data to,
 *             must be word aligned. 
 *  \param words The number of uint16 words to copy.
 *  \return If \e buff is NULL and \e words is zero,
 *           returns the minimum length in words of the buffer necessary
 *           to hold the contents of \e key. Otherwise, returns the number
 *           of words actually read, or zero if the key does not exist or
 *           is longer than \e words.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 */
uint16 PsFullRetrieve(uint16 key, void * buff, uint16 words);

/**
 *  \brief Copy the specified memory buffer to audio persistent store. 
 *   A Persistent store audio key can be updated by the application by invoking
 *  this trap.
 *  \param key The persistent store audio key to copy data to. 
 *  \param buffer The memory buffer to copy data from. 
 *  \param words Number of word of key data to write.
 *  \param offset Number of words from the start of the key to write data.
 *  \param key_length Length of the key data in uint16 words.
 *  \return The number of words actually written.
 *           Zero if either 'words' is zero or the update failed.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 */
uint16 PsUpdateAudioKey(uint32 key, const uint16 * buffer, uint16 words, uint16 offset, uint16 key_length);

/**
 *  \brief Copy to specified memory buffer from audio persistent store. 
 *  
 *   A Persistent store audio key can be read by the application by invoking this
 *  trap.
 *  \param key The persistent store audio key to copy data from. 
 *  \param buffer The memory buffer to copy data to. 
 *  \param words The number of words to copy. 
 *  \param offset Number of words from the start of the key to read data.
 *  \param key_length_addr Pointer to return the length of the key in uint16 words.
 *  \return Returns 0 if input \e buffer is NULL.
 *           Otherwise, returns the number of words actually read, or zero if the
 *  key does not exist.
 *           
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 */
uint16 PsReadAudioKey(uint32 key, uint16 * buffer, uint16 words, uint16 offset, uint16 * key_length_addr);

/**
 *  \brief Set the PS Store that are used for subsequent PS operations 
 *   \note
 *   Any store in the BlueCore firmware can be set as default using this trap.
 *  \param store The PS store to be used
 * 
 * \note This trap may be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 * 
 * WARNING: This trap is UNIMPLEMENTED
 */
void PsSetStore(PsStores store);

/**
 *  \brief Returns the current PS store used for PS operations. 
 *  \return The PS store currently used.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_core
 * 
 * WARNING: This trap is UNIMPLEMENTED
 */
PsStores PsGetStore(void );
#endif /* TRAPSET_CORE */
#if TRAPSET_PARTITION

/**
 *  \brief Copy the specified memory buffer into PSKEY_FSTAB within the persistent store
 *   The function operates in a similar manner to \see PsStore but only operates
 *   on the PSKEY_FSTAB persistent store key. The parameter commit is used to set
 *   which store the value will be written to. The data to write must always be
 *   passed to the function.
 *   
 *  \param buff The memory buffer to copy data from. 
 *  \param words The number of words to copy. 
 *  \param commit Write the new FSTAB to non-volatile memory or not. If FALSE then the new FSTAB
 *  will only be stable across warm reboots; this feature can be used to enable
 *  the newly upgrade application to perform system checks before setting the
 *  FSTAB in the non-volatile store.
 *  \return TRUE if the operation succeeded or FALSE if it failed
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_partition
 * 
 * WARNING: This trap is UNIMPLEMENTED
 */
bool PsStoreFsTab(const void * buff, uint16 words, bool commit);
#endif /* TRAPSET_PARTITION */
#endif
