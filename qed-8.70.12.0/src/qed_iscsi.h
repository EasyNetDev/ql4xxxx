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

#ifndef _QED_ISCSI_H
#define _QED_ISCSI_H
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include "qed_mcp.h"
#include "qed_iscsi_if.h"
struct qed_iscsi_info {
	spinlock_t lock;
	struct list_head free_list;
	u16 max_num_outstanding_tasks;
	void *event_context;
	iscsi_event_cb_t event_cb;
};

#if IS_ENABLED(CONFIG_QEDI)
int qed_iscsi_alloc(struct qed_hwfn *p_hwfn);

void qed_iscsi_setup(struct qed_hwfn *p_hwfn);

void qed_iscsi_free(struct qed_hwfn *p_hwfn);
#else
static inline int qed_iscsi_alloc(struct qed_hwfn __maybe_unused * p_hwfn)
{
	return -EINVAL;
}

static inline void qed_iscsi_setup(struct qed_hwfn __maybe_unused * p_hwfn)
{
}

static inline void qed_iscsi_free(struct qed_hwfn __maybe_unused * p_hwfn)
{
}
#endif

#ifdef CONFIG_QED_LL2
extern const struct qed_common_ops qed_common_ops_pass;
extern const struct qed_ll2_ops qed_ll2_ops_pass;
#endif

#if IS_ENABLED(CONFIG_QEDI)
/**
 * @brief - Fills provided statistics struct with statistics.
 *
 * @param cdev
 * @param stats - points to struct that will be filled with statistics.
 */
void qed_get_protocol_stats_iscsi(struct qed_dev *cdev,
				  struct qed_mcp_iscsi_stats *stats);
#else
static inline void
qed_get_protocol_stats_iscsi(struct qed_dev __maybe_unused * cdev,
			     struct qed_mcp_iscsi_stats __maybe_unused * stats)
{
}
#endif
#endif
