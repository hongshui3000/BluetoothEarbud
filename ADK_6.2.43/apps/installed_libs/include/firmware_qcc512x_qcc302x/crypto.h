#ifndef __CRYPTO_H__
#define __CRYPTO_H__

/*! Encryption operation control flags */
#define CRYPTO_FLAGS_AES_BIG_ENDIAN_COUNTER     1u
        
#if TRAPSET_CRYPTO

/**
 *  \brief Encrypt data using the AES128-CTR algorithm.
 *  \param key Pointer to the 128-bit cryptographic key
 *  \param nonce Pointer to the 128-bit initial vector
 *  \param flags Provides control over how the cryptographic algorithm is run e.g. whether
 *  parameters are big or little endian
 *  \param counter Counter value to use for the first block of data. It will be incremented and
 *  applied to each block in turn.
 *  \param source_data Pointer to the data to feed into the algorithm
 *  \param source_data_len_bytes Length of the source data
 *  \param dest_data Pointer to the memory where the output data from the algorithm is to be written
 *  \param dest_data_len_bytes Length of the memory available at the dest_data pointer
 *  \return TRUE - Operation completed successfully, FALSE - Some input parameters were
 *  invalid.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_crypto
 */
bool CryptoAes128Ctr(const uint8 * key, const uint8 * nonce, uint16 flags, uint32 counter, const uint8 * source_data, uint16 source_data_len_bytes, uint8 * dest_data, uint16 dest_data_len_bytes);

/**
 *  \brief Encrypt, Decrypt or hash data using the AES128-CBC algorithm.
 *  \param encrypt TRUE for an encryption or FALSE for decryption
 *  \param key Pointer to the 128-bit cryptographic key
 *  \param nonce Pointer to the 128-bit number to be used as the initial vector
 *  \param flags Reserved for future use to control aspects of the cryptographic operation.
 *  \param source_data Pointer to the data to feed into the algorithm
 *  \param source_data_len_bytes Length of the source data
 *  \param dest_data Pointer to the memory where the output data from the algorithm is to be written
 *  \param dest_data_len_bytes Length of the memory available at the dest_data pointer
 *  \return TRUE - Operation completed successfully, FALSE - Some input parameters were
 *  invalid.
 * 
 * \note This trap may NOT be called from a high-priority task handler
 * 
 * \ingroup trapset_crypto
 */
bool CryptoAes128Cbc(bool encrypt, const uint8 * key, uint8 * nonce, uint16 flags, const uint8 * source_data, uint16 source_data_len_bytes, uint8 * dest_data, uint16 dest_data_len_bytes);
#endif /* TRAPSET_CRYPTO */
#endif
