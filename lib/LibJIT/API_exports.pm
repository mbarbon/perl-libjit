# THIS FILE IS GENERATED BY author_tools/make_libjit_wrappers.pl
# DO NOT EDIT

package LibJIT::API;

our @AutoFunctions = qw(
    jit_function_create
    jit_function_create_nested
    jit_function_abandon
    jit_function_get_context
    jit_function_get_signature
    jit_function_next
    jit_function_previous
    jit_function_get_entry
    jit_function_get_current
    jit_function_get_nested_parent
    jit_function_compile
    jit_function_is_compiled
    jit_function_set_recompilable
    jit_function_clear_recompilable
    jit_function_is_recompilable
    jit_function_set_optimization_level
    jit_function_get_optimization_level
    jit_function_get_max_optimization_level
    jit_function_reserve_label
    jit_function_labels_equal
    jit_insn_get_opcode
    jit_insn_get_dest
    jit_insn_get_value1
    jit_insn_get_value2
    jit_insn_get_label
    jit_insn_get_function
    jit_insn_get_signature
    jit_insn_dest_is_value
    jit_insn_new_block
    jit_insn_load
    jit_insn_dup
    jit_insn_load_small
    jit_insn_store
    jit_insn_load_relative
    jit_insn_store_relative
    jit_insn_add_relative
    jit_insn_load_elem
    jit_insn_load_elem_address
    jit_insn_store_elem
    jit_insn_check_null
    jit_insn_add
    jit_insn_add_ovf
    jit_insn_sub
    jit_insn_sub_ovf
    jit_insn_mul
    jit_insn_mul_ovf
    jit_insn_div
    jit_insn_rem
    jit_insn_rem_ieee
    jit_insn_neg
    jit_insn_and
    jit_insn_or
    jit_insn_xor
    jit_insn_not
    jit_insn_shl
    jit_insn_shr
    jit_insn_ushr
    jit_insn_sshr
    jit_insn_eq
    jit_insn_ne
    jit_insn_lt
    jit_insn_le
    jit_insn_gt
    jit_insn_ge
    jit_insn_cmpl
    jit_insn_cmpg
    jit_insn_to_bool
    jit_insn_to_not_bool
    jit_insn_acos
    jit_insn_asin
    jit_insn_atan
    jit_insn_atan2
    jit_insn_ceil
    jit_insn_cos
    jit_insn_cosh
    jit_insn_exp
    jit_insn_floor
    jit_insn_log
    jit_insn_log10
    jit_insn_pow
    jit_insn_rint
    jit_insn_round
    jit_insn_sin
    jit_insn_sinh
    jit_insn_sqrt
    jit_insn_tan
    jit_insn_tanh
    jit_insn_trunc
    jit_insn_is_nan
    jit_insn_is_finite
    jit_insn_is_inf
    jit_insn_abs
    jit_insn_min
    jit_insn_max
    jit_insn_sign
    jit_insn_address_of
    jit_insn_convert
    jit_insn_incoming_reg
    jit_insn_incoming_frame_posn
    jit_insn_outgoing_reg
    jit_insn_outgoing_frame_posn
    jit_insn_return_reg
    jit_insn_setup_for_nested
    jit_insn_flush_struct
    jit_insn_import
    jit_insn_push
    jit_insn_push_ptr
    jit_insn_set_param
    jit_insn_set_param_ptr
    jit_insn_push_return_area_ptr
    jit_insn_pop_stack
    jit_insn_defer_pop_stack
    jit_insn_flush_defer_pop
    jit_insn_return
    jit_insn_return_ptr
    jit_insn_default_return
    jit_insn_throw
    jit_insn_get_call_stack
    jit_insn_thrown_exception
    jit_insn_uses_catcher
    jit_insn_start_catcher
    jit_insn_rethrow_unhandled
    jit_insn_return_from_finally
    jit_insn_return_from_filter
    jit_insn_memcpy
    jit_insn_memmove
    jit_insn_memset
    jit_insn_alloca
    jit_insn_move_blocks_to_end
    jit_insn_move_blocks_to_start
    jit_insn_mark_offset
    jit_insn_mark_breakpoint
    jit_insn_mark_breakpoint_variable
    jit_type_copy
    jit_type_free
    jit_type_create_pointer
    jit_type_set_size_and_alignment
    jit_type_set_offset
    jit_type_get_kind
    jit_type_get_size
    jit_type_get_alignment
    jit_type_num_fields
    jit_type_get_field
    jit_type_get_offset
    jit_type_find_name
    jit_type_num_params
    jit_type_get_return
    jit_type_get_param
    jit_type_get_abi
    jit_type_get_ref
    jit_type_get_tagged_type
    jit_type_set_tagged_type
    jit_type_get_tagged_kind
    jit_type_is_primitive
    jit_type_is_struct
    jit_type_is_union
    jit_type_is_signature
    jit_type_is_pointer
    jit_type_is_tagged
    jit_type_best_alignment
    jit_type_normalize
    jit_type_remove_tags
    jit_type_promote_int
    jit_type_return_via_pointer
    jit_type_has_tag
    jit_value_create
    jit_value_create_nint_constant
    jit_value_create_long_constant
    jit_value_create_float32_constant
    jit_value_create_float64_constant
    jit_value_create_nfloat_constant
    jit_value_get_param
    jit_value_get_struct_pointer
    jit_value_is_temporary
    jit_value_is_local
    jit_value_is_constant
    jit_value_is_parameter
    jit_value_ref
    jit_value_set_volatile
    jit_value_is_volatile
    jit_value_set_addressable
    jit_value_is_addressable
    jit_value_get_type
    jit_value_get_function
    jit_value_get_block
    jit_value_get_context
    jit_value_get_nint_constant
    jit_value_get_long_constant
    jit_value_get_float32_constant
    jit_value_get_float64_constant
    jit_value_get_nfloat_constant
    jit_value_is_true
);

1;
