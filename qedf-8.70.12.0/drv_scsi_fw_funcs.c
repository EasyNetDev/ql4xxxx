#include "drv_scsi_fw_funcs.h"
/*******************************************************************************
 * File name : scsi_init.c
 * Author    : Asaf Ravid
 *******************************************************************************
 *******************************************************************************
 * Description: 
 * ISCSI HSI functions
 *
 *******************************************************************************
 * Notes: This is the input to the auto generated file drv_scsi_fw_funcs.c
 * 
 *******************************************************************************/

#include "drv_scsi_fw_funcs.h"

#define SCSI_NUM_SGES_IN_CACHE 0x4 /* Defined in FW header \Fw\Main\MainFw\Scsi\Common\Src\ScsiCommon.h*/

bool scsi_is_slow_sgl(u16 num_sges, bool small_mid_sge)
{
	return (num_sges > SCSI_NUM_SGES_SLOW_SGL_THR && small_mid_sge);
}

void init_scsi_sgl_context(struct scsi_sgl_params      *ctx_sgl_params,
						         struct scsi_cached_sges     *ctx_data_desc,
                                 struct scsi_sgl_task_params *sgl_task_params)
{ /* no need to check for sgl_task_params->sgl validity */
    u8 sge_index;
    u8 num_sges_to_init = (u8)(sgl_task_params->num_sges > SCSI_NUM_SGES_IN_CACHE ? 
                             SCSI_NUM_SGES_IN_CACHE : sgl_task_params->num_sges);
    /* sgl params */
    ctx_sgl_params->sgl_addr.lo = 
	HWAL_CPU_TO_LE32(sgl_task_params->sgl_phys_addr.lo);
    ctx_sgl_params->sgl_addr.hi = 
	HWAL_CPU_TO_LE32(sgl_task_params->sgl_phys_addr.hi);
    ctx_sgl_params->sgl_total_length =
	HWAL_CPU_TO_LE32(sgl_task_params->total_buffer_size);
    ctx_sgl_params->sgl_num_sges     = HWAL_CPU_TO_LE16(sgl_task_params->num_sges);
        
    for (sge_index = 0; sge_index < num_sges_to_init; sge_index++) { /* set first cached SGEs in task context: */
        ctx_data_desc->sge[sge_index].sge_addr.lo =
		HWAL_CPU_TO_LE32(sgl_task_params->sgl[sge_index].sge_addr.lo);
        ctx_data_desc->sge[sge_index].sge_addr.hi =
		HWAL_CPU_TO_LE32(sgl_task_params->sgl[sge_index].sge_addr.hi);
        ctx_data_desc->sge[sge_index].sge_len  = HWAL_CPU_TO_LE32(sgl_task_params->sgl[sge_index].sge_len );
    }
}



