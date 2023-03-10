/* QLogic (R)NIC Driver/Library
 * Copyright (c) 2010-2017  Cavium, Inc.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __QED_HSI_RDMA__
#define __QED_HSI_RDMA__ 
/************************************************************************/
/* Add include to common rdma target for both eCore and protocol rdma driver */
/************************************************************************/
#include "rdma_common.h"

/* rdma completion notification queue element */
struct rdma_cnqe
{
	struct regpair cq_handle; 
};


struct rdma_cqe_responder
{
	struct regpair srq_wr_id; 
	struct regpair qp_handle; 
	__le32 imm_data_or_inv_r_Key; /* HSI_COMMENT: immediate data in case imm_flg is set, or invalidated r_key in case inv_flg is set */
	__le32 length; 
	__le32 imm_data_hi; /* HSI_COMMENT: High bytes of immediate data in case imm_flg is set in iWARP only */
	__le16 rq_cons_or_srq_id; /* HSI_COMMENT: When type is RDMA_CQE_TYPE_RESPONDER_RQ and status is WORK_REQUEST_FLUSHED_ERR it indicates an aggregative flush on all posted RQ WQEs until the reported rq_cons. When type is RDMA_CQE_TYPE_RESPONDER_XRC_SRQ it is the srq_id */
	u8 flags;
#define RDMA_CQE_RESPONDER_TOGGLE_BIT_MASK	0x1/* HSI_COMMENT: indicates a valid completion written by FW. FW toggle this bit each time it finishes producing all PBL entries */
#define RDMA_CQE_RESPONDER_TOGGLE_BIT_SHIFT	0
#define RDMA_CQE_RESPONDER_TYPE_MASK	0x3/* HSI_COMMENT:  (use enum rdma_cqe_type) */
#define RDMA_CQE_RESPONDER_TYPE_SHIFT	1
#define RDMA_CQE_RESPONDER_INV_FLG_MASK	0x1/* HSI_COMMENT: r_key invalidated indicator */
#define RDMA_CQE_RESPONDER_INV_FLG_SHIFT	3
#define RDMA_CQE_RESPONDER_IMM_FLG_MASK	0x1/* HSI_COMMENT: immediate data indicator */
#define RDMA_CQE_RESPONDER_IMM_FLG_SHIFT	4
#define RDMA_CQE_RESPONDER_RDMA_FLG_MASK	0x1/* HSI_COMMENT: 1=this CQE relates to an RDMA Write. 0=Send. */
#define RDMA_CQE_RESPONDER_RDMA_FLG_SHIFT	5
#define RDMA_CQE_RESPONDER_RESERVED2_MASK	0x3
#define RDMA_CQE_RESPONDER_RESERVED2_SHIFT	6
	u8 status; /* HSI_COMMENT:  (use enum rdma_cqe_responder_status_enum) */
};

struct rdma_cqe_requester
{
	__le16 sq_cons; 
	__le16 reserved0; 
	__le32 reserved1; 
	struct regpair qp_handle; 
	struct regpair reserved2; 
	__le32 reserved3; 
	__le16 reserved4; 
	u8 flags;
#define RDMA_CQE_REQUESTER_TOGGLE_BIT_MASK	0x1/* HSI_COMMENT: indicates a valid completion written by FW. FW toggle this bit each time it finishes producing all PBL entries */
#define RDMA_CQE_REQUESTER_TOGGLE_BIT_SHIFT	0
#define RDMA_CQE_REQUESTER_TYPE_MASK	0x3/* HSI_COMMENT:  (use enum rdma_cqe_type) */
#define RDMA_CQE_REQUESTER_TYPE_SHIFT	1
#define RDMA_CQE_REQUESTER_RESERVED5_MASK	0x1F
#define RDMA_CQE_REQUESTER_RESERVED5_SHIFT	3
	u8 status; /* HSI_COMMENT:  (use enum rdma_cqe_requester_status_enum) */
};

struct rdma_cqe_common
{
	struct regpair reserved0; 
	struct regpair qp_handle; 
	__le16 reserved1[7]; 
	u8 flags;
#define RDMA_CQE_COMMON_TOGGLE_BIT_MASK	0x1/* HSI_COMMENT: indicates a valid completion written by FW. FW toggle this bit each time it finishes producing all PBL entries */
#define RDMA_CQE_COMMON_TOGGLE_BIT_SHIFT	0
#define RDMA_CQE_COMMON_TYPE_MASK	0x3/* HSI_COMMENT:  (use enum rdma_cqe_type) */
#define RDMA_CQE_COMMON_TYPE_SHIFT	1
#define RDMA_CQE_COMMON_RESERVED2_MASK	0x1F
#define RDMA_CQE_COMMON_RESERVED2_SHIFT	3
	u8 status; 
};

/* rdma completion queue element */
union rdma_cqe
{
	struct rdma_cqe_responder resp; 
	struct rdma_cqe_requester req; 
	struct rdma_cqe_common cmn; 
};




/* CQE requester status enumeration */
enum rdma_cqe_requester_status_enum
{
	RDMA_CQE_REQ_STS_OK, 
	RDMA_CQE_REQ_STS_BAD_RESPONSE_ERR, 
	RDMA_CQE_REQ_STS_LOCAL_LENGTH_ERR, 
	RDMA_CQE_REQ_STS_LOCAL_QP_OPERATION_ERR, 
	RDMA_CQE_REQ_STS_LOCAL_PROTECTION_ERR, 
	RDMA_CQE_REQ_STS_MEMORY_MGT_OPERATION_ERR, 
	RDMA_CQE_REQ_STS_REMOTE_INVALID_REQUEST_ERR, 
	RDMA_CQE_REQ_STS_REMOTE_ACCESS_ERR, 
	RDMA_CQE_REQ_STS_REMOTE_OPERATION_ERR, 
	RDMA_CQE_REQ_STS_RNR_NAK_RETRY_CNT_ERR, 
	RDMA_CQE_REQ_STS_TRANSPORT_RETRY_CNT_ERR, 
	RDMA_CQE_REQ_STS_WORK_REQUEST_FLUSHED_ERR, 
	RDMA_CQE_REQ_STS_XRC_VIOLATION_ERR, 
	RDMA_CQE_REQ_STS_SIG_ERR, 
	MAX_RDMA_CQE_REQUESTER_STATUS_ENUM
};



/* CQE responder status enumeration */
enum rdma_cqe_responder_status_enum
{
	RDMA_CQE_RESP_STS_OK, 
	RDMA_CQE_RESP_STS_LOCAL_ACCESS_ERR, 
	RDMA_CQE_RESP_STS_LOCAL_LENGTH_ERR, 
	RDMA_CQE_RESP_STS_LOCAL_QP_OPERATION_ERR, 
	RDMA_CQE_RESP_STS_LOCAL_PROTECTION_ERR, 
	RDMA_CQE_RESP_STS_MEMORY_MGT_OPERATION_ERR, 
	RDMA_CQE_RESP_STS_REMOTE_INVALID_REQUEST_ERR, 
	RDMA_CQE_RESP_STS_WORK_REQUEST_FLUSHED_ERR, 
	MAX_RDMA_CQE_RESPONDER_STATUS_ENUM
};


/* CQE type enumeration */
enum rdma_cqe_type
{
	RDMA_CQE_TYPE_REQUESTER, 
	RDMA_CQE_TYPE_RESPONDER_RQ, 
	RDMA_CQE_TYPE_RESPONDER_SRQ, 
	RDMA_CQE_TYPE_RESPONDER_XRC_SRQ, 
	RDMA_CQE_TYPE_INVALID, 
	MAX_RDMA_CQE_TYPE
};


/* DIF Block size options */
enum rdma_dif_block_size
{
	RDMA_DIF_BLOCK_512=0, 
	RDMA_DIF_BLOCK_4096=1, 
	MAX_RDMA_DIF_BLOCK_SIZE
};


/* DIF CRC initial value */
enum rdma_dif_crc_seed
{
	RDMA_DIF_CRC_SEED_0000=0, 
	RDMA_DIF_CRC_SEED_FFFF=1, 
	MAX_RDMA_DIF_CRC_SEED
};


/* RDMA DIF Error Result Structure */
struct rdma_dif_error_result
{
	__le32 error_intervals; /* HSI_COMMENT: Total number of error intervals in the IO. */
	__le32 dif_error_1st_interval; /* HSI_COMMENT: Number of the first interval that contained error. Set to 0xFFFFFFFF if error occurred in the Runt Block. */
	u8 flags;
#define RDMA_DIF_ERROR_RESULT_DIF_ERROR_TYPE_CRC_MASK	0x1/* HSI_COMMENT: CRC error occurred. */
#define RDMA_DIF_ERROR_RESULT_DIF_ERROR_TYPE_CRC_SHIFT	0
#define RDMA_DIF_ERROR_RESULT_DIF_ERROR_TYPE_APP_TAG_MASK	0x1/* HSI_COMMENT: App Tag error occurred. */
#define RDMA_DIF_ERROR_RESULT_DIF_ERROR_TYPE_APP_TAG_SHIFT	1
#define RDMA_DIF_ERROR_RESULT_DIF_ERROR_TYPE_REF_TAG_MASK	0x1/* HSI_COMMENT: Ref Tag error occurred. */
#define RDMA_DIF_ERROR_RESULT_DIF_ERROR_TYPE_REF_TAG_SHIFT	2
#define RDMA_DIF_ERROR_RESULT_RESERVED0_MASK	0xF
#define RDMA_DIF_ERROR_RESULT_RESERVED0_SHIFT	3
#define RDMA_DIF_ERROR_RESULT_TOGGLE_BIT_MASK	0x1/* HSI_COMMENT: Used to indicate the structure is valid. Toggles each time an invalidate region is performed. */
#define RDMA_DIF_ERROR_RESULT_TOGGLE_BIT_SHIFT	7
	u8 reserved1[55]; /* HSI_COMMENT: Pad to 64 bytes to ensure efficient word line writing. */
};


/* DIF IO direction */
enum rdma_dif_io_direction_flg
{
	RDMA_DIF_DIR_RX=0, 
	RDMA_DIF_DIR_TX=1, 
	MAX_RDMA_DIF_IO_DIRECTION_FLG
};


struct rdma_dif_params
{
	__le32 base_ref_tag; /* HSI_COMMENT: Ref tag of the first DIF Block. */
	__le16 app_tag; /* HSI_COMMENT: App tag of all DIF Blocks. */
	__le16 app_tag_mask; /* HSI_COMMENT: Bitmask for verifying dif_app_tag. */
	__le16 runt_crc_value; /* HSI_COMMENT: In TX IO, in case the runt_valid_flg is set, this value is used to validate the last Block in the IO. */
	__le16 flags;
#define RDMA_DIF_PARAMS_IO_DIRECTION_FLG_MASK	0x1/* HSI_COMMENT: 0=RX, 1=TX (use enum rdma_dif_io_direction_flg) */
#define RDMA_DIF_PARAMS_IO_DIRECTION_FLG_SHIFT	0
#define RDMA_DIF_PARAMS_BLOCK_SIZE_MASK	0x1/* HSI_COMMENT: DIF block size. 0=512B 1=4096B (use enum rdma_dif_block_size) */
#define RDMA_DIF_PARAMS_BLOCK_SIZE_SHIFT	1
#define RDMA_DIF_PARAMS_RUNT_VALID_FLG_MASK	0x1/* HSI_COMMENT: In TX IO, indicates the runt_value field is valid. In RX IO, indicates the calculated runt value is to be placed on host buffer. */
#define RDMA_DIF_PARAMS_RUNT_VALID_FLG_SHIFT	2
#define RDMA_DIF_PARAMS_VALIDATE_CRC_GUARD_MASK	0x1/* HSI_COMMENT: In TX IO, indicates CRC of each DIF guard tag is checked. */
#define RDMA_DIF_PARAMS_VALIDATE_CRC_GUARD_SHIFT	3
#define RDMA_DIF_PARAMS_VALIDATE_REF_TAG_MASK	0x1/* HSI_COMMENT: In TX IO, indicates Ref tag of each DIF guard tag is checked. */
#define RDMA_DIF_PARAMS_VALIDATE_REF_TAG_SHIFT	4
#define RDMA_DIF_PARAMS_VALIDATE_APP_TAG_MASK	0x1/* HSI_COMMENT: In TX IO, indicates App tag of each DIF guard tag is checked. */
#define RDMA_DIF_PARAMS_VALIDATE_APP_TAG_SHIFT	5
#define RDMA_DIF_PARAMS_CRC_SEED_MASK	0x1/* HSI_COMMENT: DIF CRC Seed to use. 0=0x000 1=0xFFFF (use enum rdma_dif_crc_seed) */
#define RDMA_DIF_PARAMS_CRC_SEED_SHIFT	6
#define RDMA_DIF_PARAMS_RX_REF_TAG_CONST_MASK	0x1/* HSI_COMMENT: In RX IO, Ref Tag will remain at constant value of dif_base_ref_tag */
#define RDMA_DIF_PARAMS_RX_REF_TAG_CONST_SHIFT	7
#define RDMA_DIF_PARAMS_BLOCK_GUARD_TYPE_MASK	0x1/* HSI_COMMENT: 0 = checksum, 1 = CRC */
#define RDMA_DIF_PARAMS_BLOCK_GUARD_TYPE_SHIFT	8
#define RDMA_DIF_PARAMS_APP_ESCAPE_MASK	0x1/* HSI_COMMENT: Indicate to skip block check if apptag=0xffff */
#define RDMA_DIF_PARAMS_APP_ESCAPE_SHIFT	9
#define RDMA_DIF_PARAMS_REF_ESCAPE_MASK	0x1/* HSI_COMMENT: Indicate to skip block check if reftag=0xffffffff */
#define RDMA_DIF_PARAMS_REF_ESCAPE_SHIFT	10
#define RDMA_DIF_PARAMS_RESERVED4_MASK	0x1F
#define RDMA_DIF_PARAMS_RESERVED4_SHIFT	11
	__le32 Reserved5; 
};


/* rdma dirty page trace queue element */
struct rdma_dptqe
{
	__le32 flags;
#define RDMA_DPTQE_LENGTH_MASK	0xFFFFFF/* HSI_COMMENT: The length in bytes of the reported remote write (at most 64k) */
#define RDMA_DPTQE_LENGTH_SHIFT	0
#define RDMA_DPTQE_NAME_SPACE_MASK	0xFF/* HSI_COMMENT: The namespace to which this trace element belongs */
#define RDMA_DPTQE_NAME_SPACE_SHIFT	24
	__le32 rkey; /* HSI_COMMENT: The remote key of the MR */
	struct regpair va; /* HSI_COMMENT: Virtual address withing the MR */
};


struct rdma_rq_sge
{
	struct regpair addr; 
	__le32 length; 
	__le32 flags;
#define RDMA_RQ_SGE_L_KEY_LO_MASK	0x3FFFFFF/* HSI_COMMENT: 26 low bits of key of memory relating to this RQ */
#define RDMA_RQ_SGE_L_KEY_LO_SHIFT	0
#define RDMA_RQ_SGE_NUM_SGES_MASK	0x7/* HSI_COMMENT: first SGE - number of SGEs in this RQ WQE. Other SGEs - should be set to 0 */
#define RDMA_RQ_SGE_NUM_SGES_SHIFT	26
#define RDMA_RQ_SGE_L_KEY_HI_MASK	0x7/* HSI_COMMENT: 3 high bits of key of memory relating to this RQ */
#define RDMA_RQ_SGE_L_KEY_HI_SHIFT	29
};


struct rdma_sq_atomic_wqe
{
	__le32 reserved1; 
	__le32 length; /* HSI_COMMENT: Total data length (8 bytes for Atomic) */
	__le32 xrc_srq; /* HSI_COMMENT: Valid only when XRC is set for the QP */
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_ATOMIC_WQE_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_ATOMIC_WQE_COMP_FLG_SHIFT	0
#define RDMA_SQ_ATOMIC_WQE_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_ATOMIC_WQE_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_ATOMIC_WQE_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_ATOMIC_WQE_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_ATOMIC_WQE_SE_FLG_MASK	0x1/* HSI_COMMENT: Dont care for atomic wqe */
#define RDMA_SQ_ATOMIC_WQE_SE_FLG_SHIFT	3
#define RDMA_SQ_ATOMIC_WQE_INLINE_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for atomic wqe */
#define RDMA_SQ_ATOMIC_WQE_INLINE_FLG_SHIFT	4
#define RDMA_SQ_ATOMIC_WQE_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for atomic wqe */
#define RDMA_SQ_ATOMIC_WQE_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_ATOMIC_WQE_RESERVED0_MASK	0x3
#define RDMA_SQ_ATOMIC_WQE_RESERVED0_SHIFT	6
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks including SGE */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
	struct regpair remote_va; /* HSI_COMMENT: remote virtual address */
	__le32 r_key; /* HSI_COMMENT: Remote key */
	__le32 reserved2; 
	struct regpair cmp_data; /* HSI_COMMENT: Data to compare in case of ATOMIC_CMP_AND_SWAP */
	struct regpair swap_data; /* HSI_COMMENT: Swap or add data */
};


/* First element (16 bytes) of atomic wqe */
struct rdma_sq_atomic_wqe_1st
{
	__le32 reserved1; 
	__le32 length; /* HSI_COMMENT: Total data length (8 bytes for Atomic) */
	__le32 xrc_srq; /* HSI_COMMENT: Valid only when XRC is set for the QP */
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_ATOMIC_WQE_1ST_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_ATOMIC_WQE_1ST_COMP_FLG_SHIFT	0
#define RDMA_SQ_ATOMIC_WQE_1ST_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_ATOMIC_WQE_1ST_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_ATOMIC_WQE_1ST_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_ATOMIC_WQE_1ST_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_ATOMIC_WQE_1ST_SE_FLG_MASK	0x1/* HSI_COMMENT: Dont care for atomic wqe */
#define RDMA_SQ_ATOMIC_WQE_1ST_SE_FLG_SHIFT	3
#define RDMA_SQ_ATOMIC_WQE_1ST_INLINE_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for atomic wqe */
#define RDMA_SQ_ATOMIC_WQE_1ST_INLINE_FLG_SHIFT	4
#define RDMA_SQ_ATOMIC_WQE_1ST_RESERVED0_MASK	0x7
#define RDMA_SQ_ATOMIC_WQE_1ST_RESERVED0_SHIFT	5
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks including all SGEs. Set to number of SGEs + 1. */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
};


/* Second element (16 bytes) of atomic wqe */
struct rdma_sq_atomic_wqe_2nd
{
	struct regpair remote_va; /* HSI_COMMENT: remote virtual address */
	__le32 r_key; /* HSI_COMMENT: Remote key */
	__le32 reserved2; 
};


/* Third element (16 bytes) of atomic wqe */
struct rdma_sq_atomic_wqe_3rd
{
	struct regpair cmp_data; /* HSI_COMMENT: Data to compare in case of ATOMIC_CMP_AND_SWAP */
	struct regpair swap_data; /* HSI_COMMENT: Swap or add data */
};


struct rdma_sq_bind_wqe
{
	struct regpair addr; 
	__le32 l_key; 
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_BIND_WQE_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_BIND_WQE_COMP_FLG_SHIFT	0
#define RDMA_SQ_BIND_WQE_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_BIND_WQE_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_BIND_WQE_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_BIND_WQE_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_BIND_WQE_SE_FLG_MASK	0x1/* HSI_COMMENT: Dont care for bind wqe */
#define RDMA_SQ_BIND_WQE_SE_FLG_SHIFT	3
#define RDMA_SQ_BIND_WQE_INLINE_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for bind wqe */
#define RDMA_SQ_BIND_WQE_INLINE_FLG_SHIFT	4
#define RDMA_SQ_BIND_WQE_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: Bit indicating that this MW has DIF protection enabled. */
#define RDMA_SQ_BIND_WQE_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_BIND_WQE_RESERVED0_MASK	0x3
#define RDMA_SQ_BIND_WQE_RESERVED0_SHIFT	6
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
	u8 bind_ctrl;
#define RDMA_SQ_BIND_WQE_ZERO_BASED_MASK	0x1/* HSI_COMMENT: zero based indication */
#define RDMA_SQ_BIND_WQE_ZERO_BASED_SHIFT	0
#define RDMA_SQ_BIND_WQE_RESERVED1_MASK	0x7F
#define RDMA_SQ_BIND_WQE_RESERVED1_SHIFT	1
	u8 access_ctrl;
#define RDMA_SQ_BIND_WQE_REMOTE_READ_MASK	0x1
#define RDMA_SQ_BIND_WQE_REMOTE_READ_SHIFT	0
#define RDMA_SQ_BIND_WQE_REMOTE_WRITE_MASK	0x1
#define RDMA_SQ_BIND_WQE_REMOTE_WRITE_SHIFT	1
#define RDMA_SQ_BIND_WQE_ENABLE_ATOMIC_MASK	0x1
#define RDMA_SQ_BIND_WQE_ENABLE_ATOMIC_SHIFT	2
#define RDMA_SQ_BIND_WQE_LOCAL_READ_MASK	0x1
#define RDMA_SQ_BIND_WQE_LOCAL_READ_SHIFT	3
#define RDMA_SQ_BIND_WQE_LOCAL_WRITE_MASK	0x1
#define RDMA_SQ_BIND_WQE_LOCAL_WRITE_SHIFT	4
#define RDMA_SQ_BIND_WQE_RESERVED2_MASK	0x7
#define RDMA_SQ_BIND_WQE_RESERVED2_SHIFT	5
	u8 reserved3; 
	u8 length_hi; /* HSI_COMMENT: upper 8 bits of the registered MW length */
	__le32 length_lo; /* HSI_COMMENT: lower 32 bits of the registered MW length */
	__le32 parent_l_key; /* HSI_COMMENT: l_key of the parent MR */
	__le32 reserved4; 
	struct rdma_dif_params dif_params; 
};


/* First element (16 bytes) of bind wqe */
struct rdma_sq_bind_wqe_1st
{
	struct regpair addr; 
	__le32 l_key; 
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_BIND_WQE_1ST_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_BIND_WQE_1ST_COMP_FLG_SHIFT	0
#define RDMA_SQ_BIND_WQE_1ST_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_BIND_WQE_1ST_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_BIND_WQE_1ST_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_BIND_WQE_1ST_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_BIND_WQE_1ST_SE_FLG_MASK	0x1/* HSI_COMMENT: Dont care for bind wqe */
#define RDMA_SQ_BIND_WQE_1ST_SE_FLG_SHIFT	3
#define RDMA_SQ_BIND_WQE_1ST_INLINE_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for bind wqe */
#define RDMA_SQ_BIND_WQE_1ST_INLINE_FLG_SHIFT	4
#define RDMA_SQ_BIND_WQE_1ST_RESERVED0_MASK	0x7
#define RDMA_SQ_BIND_WQE_1ST_RESERVED0_SHIFT	5
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
};


/* Second element (16 bytes) of bind wqe */
struct rdma_sq_bind_wqe_2nd
{
	u8 bind_ctrl;
#define RDMA_SQ_BIND_WQE_2ND_ZERO_BASED_MASK	0x1/* HSI_COMMENT: zero based indication */
#define RDMA_SQ_BIND_WQE_2ND_ZERO_BASED_SHIFT	0
#define RDMA_SQ_BIND_WQE_2ND_RESERVED1_MASK	0x7F
#define RDMA_SQ_BIND_WQE_2ND_RESERVED1_SHIFT	1
	u8 access_ctrl;
#define RDMA_SQ_BIND_WQE_2ND_REMOTE_READ_MASK	0x1
#define RDMA_SQ_BIND_WQE_2ND_REMOTE_READ_SHIFT	0
#define RDMA_SQ_BIND_WQE_2ND_REMOTE_WRITE_MASK	0x1
#define RDMA_SQ_BIND_WQE_2ND_REMOTE_WRITE_SHIFT	1
#define RDMA_SQ_BIND_WQE_2ND_ENABLE_ATOMIC_MASK	0x1
#define RDMA_SQ_BIND_WQE_2ND_ENABLE_ATOMIC_SHIFT	2
#define RDMA_SQ_BIND_WQE_2ND_LOCAL_READ_MASK	0x1
#define RDMA_SQ_BIND_WQE_2ND_LOCAL_READ_SHIFT	3
#define RDMA_SQ_BIND_WQE_2ND_LOCAL_WRITE_MASK	0x1
#define RDMA_SQ_BIND_WQE_2ND_LOCAL_WRITE_SHIFT	4
#define RDMA_SQ_BIND_WQE_2ND_RESERVED2_MASK	0x7
#define RDMA_SQ_BIND_WQE_2ND_RESERVED2_SHIFT	5
	u8 reserved3; 
	u8 length_hi; /* HSI_COMMENT: upper 8 bits of the registered MW length */
	__le32 length_lo; /* HSI_COMMENT: lower 32 bits of the registered MW length */
	__le32 parent_l_key; /* HSI_COMMENT: l_key of the parent MR */
	__le32 reserved4; 
};


/* Third element (16 bytes) of bind wqe */
struct rdma_sq_bind_wqe_3rd
{
	struct rdma_dif_params dif_params; 
};


/* Structure with only the SQ WQE common fields. Size is of one SQ element (16B) */
struct rdma_sq_common_wqe
{
	__le32 reserved1[3]; 
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_COMMON_WQE_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_COMMON_WQE_COMP_FLG_SHIFT	0
#define RDMA_SQ_COMMON_WQE_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_COMMON_WQE_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_COMMON_WQE_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_COMMON_WQE_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_COMMON_WQE_SE_FLG_MASK	0x1/* HSI_COMMENT: If set, signal the responder to generate a solicited event on this WQE (only relevant in SENDs and RDMA write with Imm) */
#define RDMA_SQ_COMMON_WQE_SE_FLG_SHIFT	3
#define RDMA_SQ_COMMON_WQE_INLINE_FLG_MASK	0x1/* HSI_COMMENT: if set, indicates inline data is following this WQE instead of SGEs (only relevant in SENDs and RDMA writes) */
#define RDMA_SQ_COMMON_WQE_INLINE_FLG_SHIFT	4
#define RDMA_SQ_COMMON_WQE_RESERVED0_MASK	0x7
#define RDMA_SQ_COMMON_WQE_RESERVED0_SHIFT	5
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks including all SGEs or inline data. In case there are SGEs: set to number of SGEs + 1. In case of inline data: set to the whole number of 16B which contain the inline data + 1. */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
};


struct rdma_sq_fmr_wqe
{
	struct regpair addr; 
	__le32 l_key; 
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_FMR_WQE_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_FMR_WQE_COMP_FLG_SHIFT	0
#define RDMA_SQ_FMR_WQE_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_FMR_WQE_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_FMR_WQE_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_FMR_WQE_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_FMR_WQE_SE_FLG_MASK	0x1/* HSI_COMMENT: Dont care for FMR wqe */
#define RDMA_SQ_FMR_WQE_SE_FLG_SHIFT	3
#define RDMA_SQ_FMR_WQE_INLINE_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for FMR wqe */
#define RDMA_SQ_FMR_WQE_INLINE_FLG_SHIFT	4
#define RDMA_SQ_FMR_WQE_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: If set, indicated host memory of this WQE is DIF protected. */
#define RDMA_SQ_FMR_WQE_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_FMR_WQE_RESERVED0_MASK	0x3
#define RDMA_SQ_FMR_WQE_RESERVED0_SHIFT	6
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
	u8 fmr_ctrl;
#define RDMA_SQ_FMR_WQE_PAGE_SIZE_LOG_MASK	0x1F/* HSI_COMMENT: 0 is 4k, 1 is 8k... */
#define RDMA_SQ_FMR_WQE_PAGE_SIZE_LOG_SHIFT	0
#define RDMA_SQ_FMR_WQE_ZERO_BASED_MASK	0x1/* HSI_COMMENT: zero based indication */
#define RDMA_SQ_FMR_WQE_ZERO_BASED_SHIFT	5
#define RDMA_SQ_FMR_WQE_BIND_EN_MASK	0x1/* HSI_COMMENT: indication whether bind is enabled for this MR */
#define RDMA_SQ_FMR_WQE_BIND_EN_SHIFT	6
#define RDMA_SQ_FMR_WQE_RESERVED1_MASK	0x1
#define RDMA_SQ_FMR_WQE_RESERVED1_SHIFT	7
	u8 access_ctrl;
#define RDMA_SQ_FMR_WQE_REMOTE_READ_MASK	0x1
#define RDMA_SQ_FMR_WQE_REMOTE_READ_SHIFT	0
#define RDMA_SQ_FMR_WQE_REMOTE_WRITE_MASK	0x1
#define RDMA_SQ_FMR_WQE_REMOTE_WRITE_SHIFT	1
#define RDMA_SQ_FMR_WQE_ENABLE_ATOMIC_MASK	0x1
#define RDMA_SQ_FMR_WQE_ENABLE_ATOMIC_SHIFT	2
#define RDMA_SQ_FMR_WQE_LOCAL_READ_MASK	0x1
#define RDMA_SQ_FMR_WQE_LOCAL_READ_SHIFT	3
#define RDMA_SQ_FMR_WQE_LOCAL_WRITE_MASK	0x1
#define RDMA_SQ_FMR_WQE_LOCAL_WRITE_SHIFT	4
#define RDMA_SQ_FMR_WQE_RESERVED2_MASK	0x7
#define RDMA_SQ_FMR_WQE_RESERVED2_SHIFT	5
	u8 reserved3; 
	u8 length_hi; /* HSI_COMMENT: upper 8 bits of the registered MR length */
	__le32 length_lo; /* HSI_COMMENT: lower 32 bits of the registered MR length. In case of DIF the length is specified including the DIF guards. */
	struct regpair pbl_addr; /* HSI_COMMENT: Address of PBL */
};


/* First element (16 bytes) of fmr wqe */
struct rdma_sq_fmr_wqe_1st
{
	struct regpair addr; 
	__le32 l_key; 
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_FMR_WQE_1ST_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_FMR_WQE_1ST_COMP_FLG_SHIFT	0
#define RDMA_SQ_FMR_WQE_1ST_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_FMR_WQE_1ST_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_FMR_WQE_1ST_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_FMR_WQE_1ST_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_FMR_WQE_1ST_SE_FLG_MASK	0x1/* HSI_COMMENT: Dont care for FMR wqe */
#define RDMA_SQ_FMR_WQE_1ST_SE_FLG_SHIFT	3
#define RDMA_SQ_FMR_WQE_1ST_INLINE_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for FMR wqe */
#define RDMA_SQ_FMR_WQE_1ST_INLINE_FLG_SHIFT	4
#define RDMA_SQ_FMR_WQE_1ST_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: If set, indicated host memory of this WQE is DIF protected. */
#define RDMA_SQ_FMR_WQE_1ST_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_FMR_WQE_1ST_RESERVED0_MASK	0x3
#define RDMA_SQ_FMR_WQE_1ST_RESERVED0_SHIFT	6
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
};


/* Second element (16 bytes) of fmr wqe */
struct rdma_sq_fmr_wqe_2nd
{
	u8 fmr_ctrl;
#define RDMA_SQ_FMR_WQE_2ND_PAGE_SIZE_LOG_MASK	0x1F/* HSI_COMMENT: 0 is 4k, 1 is 8k... */
#define RDMA_SQ_FMR_WQE_2ND_PAGE_SIZE_LOG_SHIFT	0
#define RDMA_SQ_FMR_WQE_2ND_ZERO_BASED_MASK	0x1/* HSI_COMMENT: zero based indication */
#define RDMA_SQ_FMR_WQE_2ND_ZERO_BASED_SHIFT	5
#define RDMA_SQ_FMR_WQE_2ND_BIND_EN_MASK	0x1/* HSI_COMMENT: indication whether bind is enabled for this MR */
#define RDMA_SQ_FMR_WQE_2ND_BIND_EN_SHIFT	6
#define RDMA_SQ_FMR_WQE_2ND_RESERVED1_MASK	0x1
#define RDMA_SQ_FMR_WQE_2ND_RESERVED1_SHIFT	7
	u8 access_ctrl;
#define RDMA_SQ_FMR_WQE_2ND_REMOTE_READ_MASK	0x1
#define RDMA_SQ_FMR_WQE_2ND_REMOTE_READ_SHIFT	0
#define RDMA_SQ_FMR_WQE_2ND_REMOTE_WRITE_MASK	0x1
#define RDMA_SQ_FMR_WQE_2ND_REMOTE_WRITE_SHIFT	1
#define RDMA_SQ_FMR_WQE_2ND_ENABLE_ATOMIC_MASK	0x1
#define RDMA_SQ_FMR_WQE_2ND_ENABLE_ATOMIC_SHIFT	2
#define RDMA_SQ_FMR_WQE_2ND_LOCAL_READ_MASK	0x1
#define RDMA_SQ_FMR_WQE_2ND_LOCAL_READ_SHIFT	3
#define RDMA_SQ_FMR_WQE_2ND_LOCAL_WRITE_MASK	0x1
#define RDMA_SQ_FMR_WQE_2ND_LOCAL_WRITE_SHIFT	4
#define RDMA_SQ_FMR_WQE_2ND_RESERVED2_MASK	0x7
#define RDMA_SQ_FMR_WQE_2ND_RESERVED2_SHIFT	5
	u8 reserved3; 
	u8 length_hi; /* HSI_COMMENT: upper 8 bits of the registered MR length */
	__le32 length_lo; /* HSI_COMMENT: lower 32 bits of the registered MR length. */
	struct regpair pbl_addr; /* HSI_COMMENT: Address of PBL */
};


struct rdma_sq_local_inv_wqe
{
	struct regpair reserved; 
	__le32 inv_l_key; /* HSI_COMMENT: The invalidate local key */
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_LOCAL_INV_WQE_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_LOCAL_INV_WQE_COMP_FLG_SHIFT	0
#define RDMA_SQ_LOCAL_INV_WQE_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_LOCAL_INV_WQE_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_LOCAL_INV_WQE_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_LOCAL_INV_WQE_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_LOCAL_INV_WQE_SE_FLG_MASK	0x1/* HSI_COMMENT: Dont care for local invalidate wqe */
#define RDMA_SQ_LOCAL_INV_WQE_SE_FLG_SHIFT	3
#define RDMA_SQ_LOCAL_INV_WQE_INLINE_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for local invalidate wqe */
#define RDMA_SQ_LOCAL_INV_WQE_INLINE_FLG_SHIFT	4
#define RDMA_SQ_LOCAL_INV_WQE_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: If set, indicated host memory of this WQE is DIF protected. */
#define RDMA_SQ_LOCAL_INV_WQE_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_LOCAL_INV_WQE_RESERVED0_MASK	0x3
#define RDMA_SQ_LOCAL_INV_WQE_RESERVED0_SHIFT	6
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
};


struct rdma_sq_rdma_wqe
{
	__le32 imm_data; /* HSI_COMMENT: The immediate data in case of RDMA_WITH_IMM */
	__le32 length; /* HSI_COMMENT: Total data length. If DIF on host is enabled, length does NOT include DIF guards. */
	__le32 xrc_srq; /* HSI_COMMENT: Valid only when XRC is set for the QP */
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_RDMA_WQE_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_RDMA_WQE_COMP_FLG_SHIFT	0
#define RDMA_SQ_RDMA_WQE_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_RDMA_WQE_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_RDMA_WQE_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_RDMA_WQE_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_RDMA_WQE_SE_FLG_MASK	0x1/* HSI_COMMENT: If set, signal the responder to generate a solicited event on this WQE */
#define RDMA_SQ_RDMA_WQE_SE_FLG_SHIFT	3
#define RDMA_SQ_RDMA_WQE_INLINE_FLG_MASK	0x1/* HSI_COMMENT: if set, indicates inline data is following this WQE instead of SGEs. Applicable for RDMA_WR or RDMA_WR_WITH_IMM. Should be 0 for RDMA_RD */
#define RDMA_SQ_RDMA_WQE_INLINE_FLG_SHIFT	4
#define RDMA_SQ_RDMA_WQE_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: If set, indicated host memory of this WQE is DIF protected. */
#define RDMA_SQ_RDMA_WQE_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_RDMA_WQE_READ_INV_FLG_MASK	0x1/* HSI_COMMENT: If set, indicated read with invalidate WQE. iWARP only */
#define RDMA_SQ_RDMA_WQE_READ_INV_FLG_SHIFT	6
#define RDMA_SQ_RDMA_WQE_RESERVED0_MASK	0x1
#define RDMA_SQ_RDMA_WQE_RESERVED0_SHIFT	7
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks including all SGEs or inline data. In case there are SGEs: set to number of SGEs + 1. In case of inline data: set to the whole number of 16B which contain the inline data + 1. */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
	struct regpair remote_va; /* HSI_COMMENT: Remote virtual address */
	__le32 r_key; /* HSI_COMMENT: Remote key */
	u8 dif_flags;
#define RDMA_SQ_RDMA_WQE_DIF_BLOCK_SIZE_MASK	0x1/* HSI_COMMENT: if dif_on_host_flg set: DIF block size. 0=512B 1=4096B (use enum rdma_dif_block_size) */
#define RDMA_SQ_RDMA_WQE_DIF_BLOCK_SIZE_SHIFT	0
#define RDMA_SQ_RDMA_WQE_RESERVED1_MASK	0x7F
#define RDMA_SQ_RDMA_WQE_RESERVED1_SHIFT	1
	u8 reserved2[3]; 
};


/* First element (16 bytes) of rdma wqe */
struct rdma_sq_rdma_wqe_1st
{
	__le32 imm_data; /* HSI_COMMENT: The immediate data in case of RDMA_WITH_IMM */
	__le32 length; /* HSI_COMMENT: Total data length */
	__le32 xrc_srq; /* HSI_COMMENT: Valid only when XRC is set for the QP */
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_RDMA_WQE_1ST_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_RDMA_WQE_1ST_COMP_FLG_SHIFT	0
#define RDMA_SQ_RDMA_WQE_1ST_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_RDMA_WQE_1ST_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_RDMA_WQE_1ST_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_RDMA_WQE_1ST_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_RDMA_WQE_1ST_SE_FLG_MASK	0x1/* HSI_COMMENT: If set, signal the responder to generate a solicited event on this WQE */
#define RDMA_SQ_RDMA_WQE_1ST_SE_FLG_SHIFT	3
#define RDMA_SQ_RDMA_WQE_1ST_INLINE_FLG_MASK	0x1/* HSI_COMMENT: if set, indicates inline data is following this WQE instead of SGEs. Applicable for RDMA_WR or RDMA_WR_WITH_IMM. Should be 0 for RDMA_RD */
#define RDMA_SQ_RDMA_WQE_1ST_INLINE_FLG_SHIFT	4
#define RDMA_SQ_RDMA_WQE_1ST_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: If set, indicated host memory of this WQE is DIF protected. */
#define RDMA_SQ_RDMA_WQE_1ST_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_RDMA_WQE_1ST_READ_INV_FLG_MASK	0x1/* HSI_COMMENT: If set, indicated read with invalidate WQE. iWARP only */
#define RDMA_SQ_RDMA_WQE_1ST_READ_INV_FLG_SHIFT	6
#define RDMA_SQ_RDMA_WQE_1ST_RESERVED0_MASK	0x1
#define RDMA_SQ_RDMA_WQE_1ST_RESERVED0_SHIFT	7
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks including all SGEs or inline data. In case there are SGEs: set to number of SGEs + 1. In case of inline data: set to the whole number of 16B which contain the inline data + 1. */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
};


/* Second element (16 bytes) of rdma wqe */
struct rdma_sq_rdma_wqe_2nd
{
	struct regpair remote_va; /* HSI_COMMENT: Remote virtual address */
	__le32 r_key; /* HSI_COMMENT: Remote key */
	u8 dif_flags;
#define RDMA_SQ_RDMA_WQE_2ND_DIF_BLOCK_SIZE_MASK	0x1/* HSI_COMMENT: if dif_on_host_flg set: DIF block size. 0=512B 1=4096B (use enum rdma_dif_block_size) */
#define RDMA_SQ_RDMA_WQE_2ND_DIF_BLOCK_SIZE_SHIFT	0
#define RDMA_SQ_RDMA_WQE_2ND_DIF_FIRST_SEGMENT_FLG_MASK	0x1/* HSI_COMMENT: if dif_on_host_flg set: WQE executes first DIF on related MR. */
#define RDMA_SQ_RDMA_WQE_2ND_DIF_FIRST_SEGMENT_FLG_SHIFT	1
#define RDMA_SQ_RDMA_WQE_2ND_DIF_LAST_SEGMENT_FLG_MASK	0x1/* HSI_COMMENT: if dif_on_host_flg set: WQE executes last DIF on related MR. */
#define RDMA_SQ_RDMA_WQE_2ND_DIF_LAST_SEGMENT_FLG_SHIFT	2
#define RDMA_SQ_RDMA_WQE_2ND_RESERVED1_MASK	0x1F
#define RDMA_SQ_RDMA_WQE_2ND_RESERVED1_SHIFT	3
	u8 reserved2[3]; 
};


/* SQ WQE req type enumeration */
enum rdma_sq_req_type
{
	RDMA_SQ_REQ_TYPE_SEND, 
	RDMA_SQ_REQ_TYPE_SEND_WITH_IMM, 
	RDMA_SQ_REQ_TYPE_SEND_WITH_INVALIDATE, 
	RDMA_SQ_REQ_TYPE_RDMA_WR, 
	RDMA_SQ_REQ_TYPE_RDMA_WR_WITH_IMM, 
	RDMA_SQ_REQ_TYPE_RDMA_RD, 
	RDMA_SQ_REQ_TYPE_ATOMIC_CMP_AND_SWAP, 
	RDMA_SQ_REQ_TYPE_ATOMIC_ADD, 
	RDMA_SQ_REQ_TYPE_LOCAL_INVALIDATE, 
	RDMA_SQ_REQ_TYPE_FAST_MR, 
	RDMA_SQ_REQ_TYPE_BIND, 
	RDMA_SQ_REQ_TYPE_INVALID, 
	MAX_RDMA_SQ_REQ_TYPE
};


struct rdma_sq_send_wqe
{
	__le32 inv_key_or_imm_data; /* HSI_COMMENT: the r_key to invalidate in case of SEND_WITH_INVALIDATE, or the immediate data in case of SEND_WITH_IMM */
	__le32 length; /* HSI_COMMENT: Total data length */
	__le32 xrc_srq; /* HSI_COMMENT: Valid only when XRC is set for the QP */
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_SEND_WQE_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_SEND_WQE_COMP_FLG_SHIFT	0
#define RDMA_SQ_SEND_WQE_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_SEND_WQE_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_SEND_WQE_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_SEND_WQE_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_SEND_WQE_SE_FLG_MASK	0x1/* HSI_COMMENT: If set, signal the responder to generate a solicited event on this WQE */
#define RDMA_SQ_SEND_WQE_SE_FLG_SHIFT	3
#define RDMA_SQ_SEND_WQE_INLINE_FLG_MASK	0x1/* HSI_COMMENT: if set, indicates inline data is following this WQE instead of SGEs */
#define RDMA_SQ_SEND_WQE_INLINE_FLG_SHIFT	4
#define RDMA_SQ_SEND_WQE_DIF_ON_HOST_FLG_MASK	0x1/* HSI_COMMENT: Should be 0 for send wqe */
#define RDMA_SQ_SEND_WQE_DIF_ON_HOST_FLG_SHIFT	5
#define RDMA_SQ_SEND_WQE_RESERVED0_MASK	0x3
#define RDMA_SQ_SEND_WQE_RESERVED0_SHIFT	6
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks including all SGEs or inline data. In case there are SGEs: set to number of SGEs + 1. In case of inline data: set to the whole number of 16B which contain the inline data + 1. */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
	__le32 reserved1[4]; 
};


struct rdma_sq_send_wqe_1st
{
	__le32 inv_key_or_imm_data; /* HSI_COMMENT: the r_key to invalidate in case of SEND_WITH_INVALIDATE, or the immediate data in case of SEND_WITH_IMM */
	__le32 length; /* HSI_COMMENT: Total data length */
	__le32 xrc_srq; /* HSI_COMMENT: Valid only when XRC is set for the QP */
	u8 req_type; /* HSI_COMMENT: Type of WQE (use enum rdma_sq_req_type) */
	u8 flags;
#define RDMA_SQ_SEND_WQE_1ST_COMP_FLG_MASK	0x1/* HSI_COMMENT: If set, completion will be generated when the WQE is completed */
#define RDMA_SQ_SEND_WQE_1ST_COMP_FLG_SHIFT	0
#define RDMA_SQ_SEND_WQE_1ST_RD_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending RDMA read or Atomic operations will be completed before start processing this WQE */
#define RDMA_SQ_SEND_WQE_1ST_RD_FENCE_FLG_SHIFT	1
#define RDMA_SQ_SEND_WQE_1ST_INV_FENCE_FLG_MASK	0x1/* HSI_COMMENT: If set, all pending operations will be completed before start processing this WQE */
#define RDMA_SQ_SEND_WQE_1ST_INV_FENCE_FLG_SHIFT	2
#define RDMA_SQ_SEND_WQE_1ST_SE_FLG_MASK	0x1/* HSI_COMMENT: If set, signal the responder to generate a solicited event on this WQE */
#define RDMA_SQ_SEND_WQE_1ST_SE_FLG_SHIFT	3
#define RDMA_SQ_SEND_WQE_1ST_INLINE_FLG_MASK	0x1/* HSI_COMMENT: if set, indicates inline data is following this WQE instead of SGEs */
#define RDMA_SQ_SEND_WQE_1ST_INLINE_FLG_SHIFT	4
#define RDMA_SQ_SEND_WQE_1ST_RESERVED0_MASK	0x7
#define RDMA_SQ_SEND_WQE_1ST_RESERVED0_SHIFT	5
	u8 wqe_size; /* HSI_COMMENT: Size of WQE in 16B chunks including all SGEs or inline data. In case there are SGEs: set to number of SGEs + 1. In case of inline data: set to the whole number of 16B which contain the inline data + 1. */
	u8 prev_wqe_size; /* HSI_COMMENT: Previous WQE size in 16B chunks */
};


struct rdma_sq_send_wqe_2st
{
	__le32 reserved1[4]; 
};


struct rdma_sq_sge
{
	__le32 length; /* HSI_COMMENT: Total length of the send. If DIF on host is enabled, SGE length includes the DIF guards. */
	struct regpair addr; 
	__le32 l_key; 
};


struct rdma_srq_wqe_header
{
	struct regpair wr_id; 
	u8 num_sges; /* HSI_COMMENT: number of SGEs in WQE */
	u8 reserved2[7]; 
};

struct rdma_srq_sge
{
	struct regpair addr; 
	__le32 length; 
	__le32 l_key; 
};

/* rdma srq sge */
union rdma_srq_elm
{
	struct rdma_srq_wqe_header header; 
	struct rdma_srq_sge sge; 
};




/* Rdma doorbell data for CQ, in 24b icid mode */
struct rdma_pwm_24b_icid_data
{
	__le16 icid; /* HSI_COMMENT: internal CID */
	__le16 value_lo; /* HSI_COMMENT: Low 16b of aggregated value to update */
	__le16 value_hi; /* HSI_COMMENT: High 16b of aggregated value to update */
	u8 icid_ext; /* HSI_COMMENT: 8 MSb of icid */
	u8 params;
#define RDMA_PWM_24B_ICID_DATA_AGG_FLAGS_MASK	0x7F/* HSI_COMMENT: bit for every DQ counter flags in CM context that DQ can increment shifted by *cm_flg_24_icid_shift */
#define RDMA_PWM_24B_ICID_DATA_AGG_FLAGS_SHIFT	0
#define RDMA_PWM_24B_ICID_DATA_CONN_TYPE_IS_IWARP_MASK	0x1/* HSI_COMMENT: Connection type is iWARP */
#define RDMA_PWM_24B_ICID_DATA_CONN_TYPE_IS_IWARP_SHIFT	7
};


/* Rdma doorbell data for flags update */
struct rdma_pwm_flags_data
{
	__le16 icid; /* HSI_COMMENT: internal CID */
	u8 agg_flags; /* HSI_COMMENT: aggregative flags */
	u8 reserved; 
};


/* Rdma doorbell data for SQ and RQ */
struct rdma_pwm_val16_data
{
	__le16 icid; /* HSI_COMMENT: internal CID */
	__le16 value; /* HSI_COMMENT: aggregated value to update */
};


union rdma_pwm_val16_data_union
{
	struct rdma_pwm_val16_data as_struct; /* HSI_COMMENT: Parameters field */
	__le32 as_dword; 
};


/* Rdma doorbell data for CQ */
struct rdma_pwm_val32_data
{
	__le16 icid; /* HSI_COMMENT: internal CID */
	u8 agg_flags; /* HSI_COMMENT: bit for every DQ counter flags in CM context that DQ can increment */
	u8 params;
#define RDMA_PWM_VAL32_DATA_AGG_CMD_MASK	0x3/* HSI_COMMENT: aggregative command to CM (use enum db_agg_cmd_sel) */
#define RDMA_PWM_VAL32_DATA_AGG_CMD_SHIFT	0
#define RDMA_PWM_VAL32_DATA_BYPASS_EN_MASK	0x1/* HSI_COMMENT: enable QM bypass */
#define RDMA_PWM_VAL32_DATA_BYPASS_EN_SHIFT	2
#define RDMA_PWM_VAL32_DATA_CONN_TYPE_IS_IWARP_MASK	0x1/* HSI_COMMENT: Connection type is iWARP */
#define RDMA_PWM_VAL32_DATA_CONN_TYPE_IS_IWARP_SHIFT	3
#define RDMA_PWM_VAL32_DATA_SET_16B_VAL_MASK	0x1/* HSI_COMMENT: Flag indicating 16b variable should be updated. Should be used when conn_type_is_iwarp is used */
#define RDMA_PWM_VAL32_DATA_SET_16B_VAL_SHIFT	4
#define RDMA_PWM_VAL32_DATA_RESERVED_MASK	0x7
#define RDMA_PWM_VAL32_DATA_RESERVED_SHIFT	5
	__le32 value; /* HSI_COMMENT: aggregated value to update */
};


union rdma_pwm_val32_data_union
{
	struct rdma_pwm_val32_data as_struct; /* HSI_COMMENT: Parameters field */
	struct regpair as_repair; 
};

#endif /* __QED_HSI_RDMA__ */
