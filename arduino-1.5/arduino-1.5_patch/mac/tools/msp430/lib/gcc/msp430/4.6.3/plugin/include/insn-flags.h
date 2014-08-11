/* Generated automatically by the program `genflags'
   from the machine description file `md'.  */

#ifndef GCC_INSN_FLAGS_H
#define GCC_INSN_FLAGS_H

#define HAVE_nop 1
#define HAVE_dint 1
#define HAVE_eint 1
#define HAVE_read_status_register 1
#define HAVE_get_interrupt_state 1
#define HAVE_write_status_register 1
#define HAVE_set_interrupt_state 1
#define HAVE_bic_status_register 1
#define HAVE_bis_status_register 1
#define HAVE_bic_status_register_on_exit 1
#define HAVE_bis_status_register_on_exit 1
#define HAVE_read_stack_pointer 1
#define HAVE_write_stack_pointer 1
#define HAVE_delay_cycles_init 1
#define HAVE_delay_cycles_decr 1
#define HAVE_get_watchdog_clear_value 1
#define HAVE_set_watchdog_clear_value 1
#define HAVE_watchdog_clear 1
#define HAVE_extend8bithi1 1
#define HAVE_pushqi1 1
#define HAVE_loadqi2 1
#define HAVE_storeqi2 1
#define HAVE_pushmhi2 1
#define HAVE_popmhi2 1
#define HAVE_pushhi1 1
#define HAVE_pophi1 1
#define HAVE_pushsi1 1
#define HAVE_pushsf1 1
#define HAVE_pushdi1 1
#define HAVE_negsf2 1
#define HAVE_anddi3 1
#define HAVE_andsi3 1
#define HAVE_andhi3 1
#define HAVE_andqi3 1
#define HAVE_iordi3 1
#define HAVE_iorsi3 1
#define HAVE_iorhi3 1
#define HAVE_iorqi3 1
#define HAVE_xordi3 1
#define HAVE_xorsi3 1
#define HAVE_xorhi3 1
#define HAVE_xorqi3 1
#define HAVE_abssf2 1
#define HAVE_setcchi2 1
#define HAVE_setccqi2 1
#define HAVE_branchcc 1
#define HAVE_mpy_inhibit_intr 1
#define HAVE_mpy_restore_intr 1
#define HAVE_trunchiqi 1
#define HAVE_truncsihi 1
#define HAVE_truncsiqi 1
#define HAVE_truncdiqi 1
#define HAVE_truncdisi 1
#define HAVE_clrc 1
#define HAVE_mwshl8xor 1
#define HAVE_mwshr8xor 1
#define HAVE_ashrhi_15 1
#define HAVE_ashldi_1 1
#define HAVE_ashlsi_1 1
#define HAVE_ashlhi_1 1
#define HAVE_ashlqi_1 1
#define HAVE_rlahi1 1
#define HAVE_rlaqi1 1
#define HAVE_rlchi1 1
#define HAVE_rlcqi1 1
#define HAVE_ashrdi_1 1
#define HAVE_ashrsi_1 1
#define HAVE_ashrhi_1 1
#define HAVE_ashrqi_1 1
#define HAVE_rrahi1 1
#define HAVE_rraqi1 1
#define HAVE_rrchi1 1
#define HAVE_rrcqi1 1
#define HAVE_lshrdi_1 1
#define HAVE_lshrsi_1 1
#define HAVE_lshrhi_1 1
#define HAVE_lshrqi_1 1
#define HAVE_rruhi1 1
#define HAVE_rruqi1 1
#define HAVE_bittestforcarryhi2 1
#define HAVE_bittestforcarryqi2 1
#define HAVE_jump 1
#define HAVE_indirect_jump 1
#define HAVE_tablejump 1
#define HAVE_return ((reload_completed && cfun && cfun->machine \
    && (cfun->machine->frame_flags & MSP430_FF_ready_for_return)))
#define HAVE_extendsidi2 1
#define HAVE_extendhidi2 1
#define HAVE_extendhisi2 1
#define HAVE_extendqidi2 1
#define HAVE_extendqisi2 1
#define HAVE_extendqihi2 1
#define HAVE_zero_extendsidi2 1
#define HAVE_zero_extendhidi2 1
#define HAVE_zero_extendhisi2 1
#define HAVE_zero_extendqidi2 1
#define HAVE_zero_extendqisi2 1
#define HAVE_zero_extendqihi2 1
#define HAVE_bswaphi1 1
#define HAVE_movqi 1
#define HAVE_movhi 1
#define HAVE_movsi 1
#define HAVE_movsf 1
#define HAVE_movdi 1
#define HAVE_adddi3 1
#define HAVE_addsi3 1
#define HAVE_addhi3 1
#define HAVE_addqi3 1
#define HAVE_subdi3 1
#define HAVE_subsi3 1
#define HAVE_subhi3 1
#define HAVE_subqi3 1
#define HAVE_one_cmpldi2 1
#define HAVE_one_cmplsi2 1
#define HAVE_one_cmplhi2 1
#define HAVE_one_cmplqi2 1
#define HAVE_negdi2 1
#define HAVE_negsi2 1
#define HAVE_neghi2 1
#define HAVE_negqi2 1
#define HAVE_absdi2 1
#define HAVE_abssi2 1
#define HAVE_abshi2 1
#define HAVE_absqi2 1
#define HAVE_strlenhi 1
#define HAVE_cbranchdi4 1
#define HAVE_cbranchsi4 1
#define HAVE_cbranchhi4 1
#define HAVE_cbranchqi4 1
#define HAVE_call 1
#define HAVE_call_value 1
#define HAVE_muldi3 1
#define HAVE_mulsi3 1
#define HAVE_mulhi3 1
#define HAVE_mulqi3 1
#define HAVE_mulsidi3 1
#define HAVE_mulhisi3 1
#define HAVE_mulqihi3 1
#define HAVE_umulsidi3 1
#define HAVE_umulhisi3 1
#define HAVE_umulqihi3 1
#define HAVE_ashldi3 1
#define HAVE_ashlsi3 1
#define HAVE_ashlhi3 1
#define HAVE_ashlqi3 1
#define HAVE_ashrdi3 1
#define HAVE_ashrsi3 1
#define HAVE_ashrhi3 1
#define HAVE_ashrqi3 1
#define HAVE_lshrdi3 1
#define HAVE_lshrsi3 1
#define HAVE_lshrhi3 1
#define HAVE_lshrqi3 1
#define HAVE_ashlhi_m 1
#define HAVE_ashrhi_m 1
#define HAVE_lshrhi_m 1
#define HAVE_extzv 1
#define HAVE_prologue 1
#define HAVE_epilogue 1
extern rtx        gen_nop                         (void);
extern rtx        gen_dint                        (void);
extern rtx        gen_eint                        (void);
extern rtx        gen_read_status_register        (rtx);
extern rtx        gen_get_interrupt_state         (rtx);
extern rtx        gen_write_status_register       (rtx);
extern rtx        gen_set_interrupt_state         (rtx);
extern rtx        gen_bic_status_register         (rtx);
extern rtx        gen_bis_status_register         (rtx);
extern rtx        gen_bic_status_register_on_exit (rtx, rtx);
extern rtx        gen_bis_status_register_on_exit (rtx, rtx);
extern rtx        gen_read_stack_pointer          (rtx);
extern rtx        gen_write_stack_pointer         (rtx);
extern rtx        gen_delay_cycles_init           (rtx, rtx);
extern rtx        gen_delay_cycles_decr           (rtx);
extern rtx        gen_get_watchdog_clear_value    (rtx);
extern rtx        gen_set_watchdog_clear_value    (rtx);
extern rtx        gen_watchdog_clear              (void);
extern rtx        gen_extend8bithi1               (rtx);
extern rtx        gen_pushqi1                     (rtx);
extern rtx        gen_loadqi2                     (rtx, rtx);
extern rtx        gen_storeqi2                    (rtx, rtx);
extern rtx        gen_pushmhi2                    (rtx, rtx);
extern rtx        gen_popmhi2                     (rtx, rtx);
extern rtx        gen_pushhi1                     (rtx);
extern rtx        gen_pophi1                      (rtx);
extern rtx        gen_pushsi1                     (rtx);
extern rtx        gen_pushsf1                     (rtx);
extern rtx        gen_pushdi1                     (rtx);
extern rtx        gen_negsf2                      (rtx, rtx);
extern rtx        gen_anddi3                      (rtx, rtx, rtx);
extern rtx        gen_andsi3                      (rtx, rtx, rtx);
extern rtx        gen_andhi3                      (rtx, rtx, rtx);
extern rtx        gen_andqi3                      (rtx, rtx, rtx);
extern rtx        gen_iordi3                      (rtx, rtx, rtx);
extern rtx        gen_iorsi3                      (rtx, rtx, rtx);
extern rtx        gen_iorhi3                      (rtx, rtx, rtx);
extern rtx        gen_iorqi3                      (rtx, rtx, rtx);
extern rtx        gen_xordi3                      (rtx, rtx, rtx);
extern rtx        gen_xorsi3                      (rtx, rtx, rtx);
extern rtx        gen_xorhi3                      (rtx, rtx, rtx);
extern rtx        gen_xorqi3                      (rtx, rtx, rtx);
extern rtx        gen_abssf2                      (rtx, rtx);
extern rtx        gen_setcchi2                    (rtx, rtx);
extern rtx        gen_setccqi2                    (rtx, rtx);
extern rtx        gen_branchcc                    (rtx, rtx);
extern rtx        gen_mpy_inhibit_intr            (void);
extern rtx        gen_mpy_restore_intr            (void);
extern rtx        gen_trunchiqi                   (rtx, rtx);
extern rtx        gen_truncsihi                   (rtx, rtx);
extern rtx        gen_truncsiqi                   (rtx, rtx);
extern rtx        gen_truncdiqi                   (rtx, rtx);
extern rtx        gen_truncdisi                   (rtx, rtx);
extern rtx        gen_clrc                        (void);
extern rtx        gen_mwshl8xor                   (rtx, rtx);
extern rtx        gen_mwshr8xor                   (rtx, rtx);
extern rtx        gen_ashrhi_15                   (rtx, rtx);
extern rtx        gen_ashldi_1                    (rtx);
extern rtx        gen_ashlsi_1                    (rtx);
extern rtx        gen_ashlhi_1                    (rtx);
extern rtx        gen_ashlqi_1                    (rtx);
extern rtx        gen_rlahi1                      (rtx);
extern rtx        gen_rlaqi1                      (rtx);
extern rtx        gen_rlchi1                      (rtx);
extern rtx        gen_rlcqi1                      (rtx);
extern rtx        gen_ashrdi_1                    (rtx);
extern rtx        gen_ashrsi_1                    (rtx);
extern rtx        gen_ashrhi_1                    (rtx);
extern rtx        gen_ashrqi_1                    (rtx);
extern rtx        gen_rrahi1                      (rtx);
extern rtx        gen_rraqi1                      (rtx);
extern rtx        gen_rrchi1                      (rtx);
extern rtx        gen_rrcqi1                      (rtx);
extern rtx        gen_lshrdi_1                    (rtx);
extern rtx        gen_lshrsi_1                    (rtx);
extern rtx        gen_lshrhi_1                    (rtx);
extern rtx        gen_lshrqi_1                    (rtx);
extern rtx        gen_rruhi1                      (rtx);
extern rtx        gen_rruqi1                      (rtx);
extern rtx        gen_bittestforcarryhi2          (rtx, rtx);
extern rtx        gen_bittestforcarryqi2          (rtx, rtx);
extern rtx        gen_jump                        (rtx);
extern rtx        gen_indirect_jump               (rtx);
extern rtx        gen_tablejump                   (rtx, rtx);
extern rtx        gen_return                      (void);
extern rtx        gen_extendsidi2                 (rtx, rtx);
extern rtx        gen_extendhidi2                 (rtx, rtx);
extern rtx        gen_extendhisi2                 (rtx, rtx);
extern rtx        gen_extendqidi2                 (rtx, rtx);
extern rtx        gen_extendqisi2                 (rtx, rtx);
extern rtx        gen_extendqihi2                 (rtx, rtx);
extern rtx        gen_zero_extendsidi2            (rtx, rtx);
extern rtx        gen_zero_extendhidi2            (rtx, rtx);
extern rtx        gen_zero_extendhisi2            (rtx, rtx);
extern rtx        gen_zero_extendqidi2            (rtx, rtx);
extern rtx        gen_zero_extendqisi2            (rtx, rtx);
extern rtx        gen_zero_extendqihi2            (rtx, rtx);
extern rtx        gen_bswaphi1                    (rtx);
extern rtx        gen_movqi                       (rtx, rtx);
extern rtx        gen_movhi                       (rtx, rtx);
extern rtx        gen_movsi                       (rtx, rtx);
extern rtx        gen_movsf                       (rtx, rtx);
extern rtx        gen_movdi                       (rtx, rtx);
extern rtx        gen_adddi3                      (rtx, rtx, rtx);
extern rtx        gen_addsi3                      (rtx, rtx, rtx);
extern rtx        gen_addhi3                      (rtx, rtx, rtx);
extern rtx        gen_addqi3                      (rtx, rtx, rtx);
extern rtx        gen_subdi3                      (rtx, rtx, rtx);
extern rtx        gen_subsi3                      (rtx, rtx, rtx);
extern rtx        gen_subhi3                      (rtx, rtx, rtx);
extern rtx        gen_subqi3                      (rtx, rtx, rtx);
extern rtx        gen_one_cmpldi2                 (rtx, rtx);
extern rtx        gen_one_cmplsi2                 (rtx, rtx);
extern rtx        gen_one_cmplhi2                 (rtx, rtx);
extern rtx        gen_one_cmplqi2                 (rtx, rtx);
extern rtx        gen_negdi2                      (rtx, rtx);
extern rtx        gen_negsi2                      (rtx, rtx);
extern rtx        gen_neghi2                      (rtx, rtx);
extern rtx        gen_negqi2                      (rtx, rtx);
extern rtx        gen_absdi2                      (rtx, rtx);
extern rtx        gen_abssi2                      (rtx, rtx);
extern rtx        gen_abshi2                      (rtx, rtx);
extern rtx        gen_absqi2                      (rtx, rtx);
extern rtx        gen_strlenhi                    (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchdi4                  (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchsi4                  (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchhi4                  (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchqi4                  (rtx, rtx, rtx, rtx);
#define GEN_CALL(A, B, C, D) gen_call ((A), (B))
extern rtx        gen_call                        (rtx, rtx);
#define GEN_CALL_VALUE(A, B, C, D, E) gen_call_value ((A), (B), (C))
extern rtx        gen_call_value                  (rtx, rtx, rtx);
extern rtx        gen_muldi3                      (rtx, rtx, rtx);
extern rtx        gen_mulsi3                      (rtx, rtx, rtx);
extern rtx        gen_mulhi3                      (rtx, rtx, rtx);
extern rtx        gen_mulqi3                      (rtx, rtx, rtx);
extern rtx        gen_mulsidi3                    (rtx, rtx, rtx);
extern rtx        gen_mulhisi3                    (rtx, rtx, rtx);
extern rtx        gen_mulqihi3                    (rtx, rtx, rtx);
extern rtx        gen_umulsidi3                   (rtx, rtx, rtx);
extern rtx        gen_umulhisi3                   (rtx, rtx, rtx);
extern rtx        gen_umulqihi3                   (rtx, rtx, rtx);
extern rtx        gen_ashldi3                     (rtx, rtx, rtx);
extern rtx        gen_ashlsi3                     (rtx, rtx, rtx);
extern rtx        gen_ashlhi3                     (rtx, rtx, rtx);
extern rtx        gen_ashlqi3                     (rtx, rtx, rtx);
extern rtx        gen_ashrdi3                     (rtx, rtx, rtx);
extern rtx        gen_ashrsi3                     (rtx, rtx, rtx);
extern rtx        gen_ashrhi3                     (rtx, rtx, rtx);
extern rtx        gen_ashrqi3                     (rtx, rtx, rtx);
extern rtx        gen_lshrdi3                     (rtx, rtx, rtx);
extern rtx        gen_lshrsi3                     (rtx, rtx, rtx);
extern rtx        gen_lshrhi3                     (rtx, rtx, rtx);
extern rtx        gen_lshrqi3                     (rtx, rtx, rtx);
extern rtx        gen_ashlhi_m                    (rtx, rtx, rtx);
extern rtx        gen_ashrhi_m                    (rtx, rtx, rtx);
extern rtx        gen_lshrhi_m                    (rtx, rtx, rtx);
extern rtx        gen_extzv                       (rtx, rtx, rtx, rtx);
extern rtx        gen_prologue                    (void);
extern rtx        gen_epilogue                    (void);

#endif /* GCC_INSN_FLAGS_H */
