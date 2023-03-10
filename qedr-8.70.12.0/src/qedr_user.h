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

#ifndef __QEDR_USER_H__
#define __QEDR_USER_H__

#include <linux/types.h>

#define QEDR_ABI_VERSION		(8)

/* user kernel communication data structures. */
struct qedr_alloc_ucontext_req {
	__u32 context_flags;
#define	QEDR_ROCE_EDPM_MODE_V1_MASK	0x1
#define	QEDR_ROCE_EDPM_MODE_V1_SHIFT	0
	__u32 reserved[3];
};

struct qedr_alloc_ucontext_resp {
	__u64 db_pa;
	__u32 db_size;

	__u32 max_send_wr;
	__u32 max_recv_wr;
	__u32 max_srq_wr;
	__u32 sges_per_send_wr;
	__u32 sges_per_recv_wr;
	__u32 sges_per_srq_wr;
	__u32 max_cqes;
	__u8 dpm_enabled;
	__u8 wids_enabled;
	__u16 wid_count;
};

struct qedr_alloc_pd_ureq {
	__u64 rsvd1;
};

struct qedr_alloc_pd_uresp {
	__u32 pd_id;
};

struct qedr_create_cq_ureq {
	__u64 addr;	    /* user space virtual address of CQ buffer */
	__u64 len;	    /* size of CQ buffer */
	__u64 db_rec_addr;  /* address of CQ doorbell recovery user entry */
};

struct qedr_create_cq_uresp {
	__u32 db_offset;
	__u16 icid;
};

struct qedr_create_qp_ureq {
	__u32 qp_handle_hi;
	__u32 qp_handle_lo;

	/* user space virtual address of SQ buffer */
	__u64 sq_addr;

	/* length of SQ buffer */
	__u64 sq_len;

	/* user space virtual address of RQ buffer */
	__u64 rq_addr;

	/* length of RQ buffer */
	__u64 rq_len;

	/* address of SQ doorbell recovery user entry */
	__u64 sq_db_rec_addr;

	/* address of RQ doorbell recovery user entry */
	__u64 rq_db_rec_addr;
};

struct qedr_create_qp_uresp {
	__u32 qp_id;
	__u32 atomic_supported;

	/* SQ */
	__u32 sq_db_offset;
	__u16 sq_icid;

	/* RQ */
	__u32 rq_db_offset;
	__u16 rq_icid;

	__u32 rq_db2_offset;
};

struct qedr_create_srq_ureq {
	/* user space virtual address of producer pair */
	__u64 prod_pair_addr;

	/* user space virtual address of SRQ buffer */
	__u64 srq_addr;

	/* length of SRQ buffer */
	__u64 srq_len;
};

struct qedr_create_srq_uresp {
	__u16 srq_id;
};

/* doorbell recovery entry allocated and populated by userspace doorbelling
 * entities and mapped to kernel. Kernel uses this to register doorbell
 * information with doorbell drop recovery mechanism.
 */
struct qedr_user_db_rec {
	__u64 db_data; /* doorbell data */
};

#endif /* __QEDR_USER_H__ */
