/**
 * @file sbc.h
 * @date 2020.7.28
 * @author zhiyuan,chen
 *
 * @defgroup
 * @ingroup
 * @brief 
 * @details
 *
 *
 * @{
 *
 *
 */

#ifndef __LIB_SBC_H__
#define __LIB_SBC_H__

#ifdef __cplusplus
extern "C"
{ /*}*/
#endif

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * EXTERN VARIABLES
 */


/*********************************************************************
 * EXTERN FUNCTIONS
 */

/**
 * @brief msbc codec_init
 *
 * @param[in] msbc, sbc or msbc
 * @param[in] bitpool, msbc must bt 26
 *
 * @return None
 **/
void codec_init(unsigned char msbc, unsigned char bitpool);

/**
 * @brief sbc encoder pcm
 *
 * @param[in] input pcm data
 * @param[in] input_len pcm len
 * @param[in] output output data, the sbc stream data
 * @param[in] outlen input the max out len
 *
 * @return outlen len
 **/
int codec_encode(const unsigned char *input, unsigned short input_len, void* output, unsigned short output_len);

#ifdef __cplusplus
/*{*/ }
#endif

#endif

/** @} */
