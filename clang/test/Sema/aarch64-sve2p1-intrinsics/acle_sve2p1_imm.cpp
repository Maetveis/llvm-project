// RUN: %clang_cc1 -triple aarch64-none-linux-gnu -target-feature +sve -target-feature +sve2 -target-feature +sve2p1 -fsyntax-only -verify %s

// REQUIRES: aarch64-registered-target

#include <arm_sve.h>
void test_svpext_lane_imm_0_3(svcount_t c) {
  svpext_lane_c8(c, -1);  // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svpext_lane_c16(c, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svpext_lane_c32(c, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svpext_lane_c64(c, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}

  svpext_lane_c8(c, 4);  // expected-error {{argument value 4 is outside the valid range [0, 3]}}
  svpext_lane_c16(c, 4); // expected-error {{argument value 4 is outside the valid range [0, 3]}}
  svpext_lane_c32(c, 4); // expected-error {{argument value 4 is outside the valid range [0, 3]}}
  svpext_lane_c64(c, 4); // expected-error {{argument value 4 is outside the valid range [0, 3]}}
}

void test_svpext_lane_x2_imm_0_1(svcount_t c) {
  svpext_lane_c8_x2(c, -1);  // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svpext_lane_c16_x2(c, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svpext_lane_c32_x2(c, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svpext_lane_c64_x2(c, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}

  svpext_lane_c8_x2(c, 2);  // expected-error {{argument value 2 is outside the valid range [0, 1]}}
  svpext_lane_c16_x2(c, 2); // expected-error {{argument value 2 is outside the valid range [0, 1]}}
  svpext_lane_c32_x2(c, 2); // expected-error {{argument value 2 is outside the valid range [0, 1]}}
  svpext_lane_c64_x2(c, 2); // expected-error {{argument value 2 is outside the valid range [0, 1]}}
}

svcount_t test_svwhile_pn_signed(int64_t op1, int64_t op2) {
  svwhilege_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilege_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilege_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilege_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}

  svwhilege_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilege_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilege_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilege_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilele_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilele_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilele_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilele_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
}

svcount_t test_svwhile_pn_unsigned(uint64_t op1, uint64_t op2) {
  svwhilege_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilege_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilege_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilege_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilegt_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilele_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c8(op1, op2, 6);  // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c16(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c32(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}
  svwhilelt_c64(op1, op2, 6); // expected-error {{argument value 6 is outside the valid range [2, 4]}}

  svwhilege_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilege_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilege_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilege_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilegt_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilele_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilele_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilele_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilele_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c8(op1, op2, 3);  // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c16(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c32(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
  svwhilelt_c64(op1, op2, 3); // expected-error {{argument should be a multiple of 2}}
}

void test_cntp(svcount_t c) {
  svcntp_c8(c, 1);  // expected-error {{argument value 1 is outside the valid range [2, 4]}}
  svcntp_c16(c, 1); // expected-error {{argument value 1 is outside the valid range [2, 4]}}
  svcntp_c32(c, 1); // expected-error {{argument value 1 is outside the valid range [2, 4]}}
  svcntp_c64(c, 1); // expected-error {{argument value 1 is outside the valid range [2, 4]}}

  svcntp_c8(c, 3);  // expected-error {{argument should be a multiple of 2}}
  svcntp_c16(c, 3); // expected-error {{argument should be a multiple of 2}}
  svcntp_c32(c, 3); // expected-error {{argument should be a multiple of 2}}
  svcntp_c64(c, 3); // expected-error {{argument should be a multiple of 2}}
}


void test_svdot_lane_2way(svint32_t s32, svuint32_t u32, svint16_t s16, svuint16_t u16,
                          svfloat32_t f32, svfloat16_t f16) {
  svdot_lane_s32_s16(s32, s16, s16, 4); // expected-error {{argument value 4 is outside the valid range [0, 3]}}
  svdot_lane_u32_u16(u32, u16, u16, 4); // expected-error {{argument value 4 is outside the valid range [0, 3]}}
  svdot_lane_f32_f16(f32, f16, f16, 4); // expected-error {{argument value 4 is outside the valid range [0, 3]}}
}


__attribute__((target("+sve2p1+b16b16")))
void test_svbfml_lane(svbfloat16_t zda, svbfloat16_t zn, svbfloat16_t zm, uint64_t idx){
  svmla_lane_bf16(zda, zn, zm, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}
  svmla_lane_bf16(zda, zn, zm, 8);  // expected-error {{argument value 8 is outside the valid range [0, 7]}}
  svmls_lane_bf16(zda, zn, zm, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}
  svmls_lane_bf16(zda, zn, zm, 8);  // expected-error {{argument value 8 is outside the valid range [0, 7]}}
  svmla_lane_bf16(zda, zn, zm, idx); // expected-error {{argument to 'svmla_lane_bf16' must be a constant integer}}
  svmls_lane_bf16(zda, zn, zm, idx);  // expected-error {{argument to 'svmls_lane_bf16' must be a constant integer}}
}

__attribute__((target("+sve2p1+b16b16")))
void test_svbfmul_lane(svbfloat16_t zn, svbfloat16_t zm, uint64_t idx){
  svmul_lane_bf16(zn, zm, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}
  svmul_lane_bf16(zn, zm, 8);  // expected-error {{argument value 8 is outside the valid range [0, 7]}}
  svmul_lane_bf16(zn, zm, idx);  // expected-error {{argument to 'svmul_lane_bf16' must be a constant integer}}
}

__attribute__((target("+sve2p1")))
void test_svextq_8b_offset(svint8_t s8, svuint8_t u8){
  svextq_s8(s8, s8, -1);  // expected-error {{argument value -1 is outside the valid range [0, 15]}}
  svextq_u8(u8, u8, -1);  // expected-error {{argument value -1 is outside the valid range [0, 15]}}

  svextq_s8(s8, s8, 16);   // expected-error {{argument value 16 is outside the valid range [0, 15]}}
  svextq_u8(u8, u8, 16);   // expected-error {{argument value 16 is outside the valid range [0, 15]}}
}

__attribute__((target("+sve2p1")))
void test_svextq_16b_offset(svint16_t s16, svuint16_t u16, svfloat16_t f16, svbfloat16_t bf16){
  svextq_s16(s16, s16, -1);     // expected-error {{argument value -1 is outside the valid range [0, 7]}}
  svextq_u16(u16, u16, -1);     // expected-error {{argument value -1 is outside the valid range [0, 7]}}
  svextq_f16(f16, f16, -1);     // expected-error {{argument value -1 is outside the valid range [0, 7]}}
  svextq_bf16(bf16, bf16, -1);  // expected-error {{argument value -1 is outside the valid range [0, 7]}}

  svextq_s16(s16, s16, 8);     // expected-error {{argument value 8 is outside the valid range [0, 7]}}
  svextq_u16(u16, u16, 8);     // expected-error {{argument value 8 is outside the valid range [0, 7]}}
  svextq_f16(f16, f16, 8);     // expected-error {{argument value 8 is outside the valid range [0, 7]}}
  svextq_bf16(bf16, bf16, 8);  // expected-error {{argument value 8 is outside the valid range [0, 7]}}
}

__attribute__((target("+sve2p1")))
void test_svextq_32b_offset(svint32_t s32, svuint32_t u32, svfloat32_t f32){
  svextq_s32(s32, s32, -1);   // expected-error {{argument value -1 is outside the valid range [0, 3]}}
  svextq_u32(u32, u32, -1);   // expected-error {{argument value -1 is outside the valid range [0, 3]}}
  svextq_f32(f32, f32, -1);   // expected-error {{argument value -1 is outside the valid range [0, 3]}}

  svextq_s32(s32, s32, 4);   // expected-error {{argument value 4 is outside the valid range [0, 3]}}
  svextq_u32(u32, u32, 4);   // expected-error {{argument value 4 is outside the valid range [0, 3]}}
  svextq_f32(f32, f32, 4);   // expected-error {{argument value 4 is outside the valid range [0, 3]}}
}

__attribute__((target("+sve2p1")))
void test_svextq_64b_offset(svint64_t s64, svuint64_t u64, svfloat64_t f64){
  svextq_s64(s64, s64, -1);   // expected-error {{argument value -1 is outside the valid range [0, 1]}}
  svextq_u64(u64, u64, -1);   // expected-error {{argument value -1 is outside the valid range [0, 1]}}
  svextq_f64(f64, f64, -1);   // expected-error {{argument value -1 is outside the valid range [0, 1]}}

  svextq_s64(s64, s64, 2);   // expected-error {{argument value 2 is outside the valid range [0, 1]}}
  svextq_u64(u64, u64, 2);   // expected-error {{argument value 2 is outside the valid range [0, 1]}}
  svextq_f64(f64, f64, 2);   // expected-error {{argument value 2 is outside the valid range [0, 1]}}
}

__attribute__((target("+sve2p1")))
void test_svpmov_lane(){
  svuint8_t zn_u8;
  svuint16_t zn_u16;
  svuint32_t zn_u32;
  svuint64_t zn_u64;
  svbool_t pn;

  svpmov_lane_u8(zn_u8, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 0]}}
  svpmov_lane_u16(zn_u16, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svpmov_lane_u32(zn_u32, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svpmov_lane_u64(zn_u64, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}

  svpmov_lane_u8(zn_u8, 1); // expected-error {{argument value 1 is outside the valid range [0, 0]}}
  svpmov_lane_u16(zn_u16, 3); // expected-error {{argument value 3 is outside the valid range [0, 1]}}
  svpmov_lane_u32(zn_u32, 5); // expected-error {{argument value 5 is outside the valid range [0, 3]}}
  svpmov_lane_u64(zn_u64, 8); // expected-error {{argument value 8 is outside the valid range [0, 7]}}


  zn_u16 = svpmov_lane_u16_m(zn_u16, pn, 0); // expected-error {{argument value 0 is outside the valid range [1, 1]}}
  zn_u32 = svpmov_lane_u32_m(zn_u32, pn, 0); // expected-error {{argument value 0 is outside the valid range [1, 3]}}
  zn_u64 = svpmov_lane_u64_m(zn_u64, pn, 0); // expected-error {{argument value 0 is outside the valid range [1, 7]}}

  zn_u16 = svpmov_lane_u16_m(zn_u16, pn, 3); // expected-error {{argument value 3 is outside the valid range [1, 1]}}
  zn_u32 = svpmov_lane_u32_m(zn_u32, pn, 5); // expected-error {{argument value 5 is outside the valid range [1, 3]}}
  zn_u64 = svpmov_lane_u64_m(zn_u64, pn, 8); // expected-error {{argument value 8 is outside the valid range [1, 7]}}
}

__attribute__((target("+sve2p1")))
void test_svget_svset_b(uint64_t idx, svboolx2_t tuple2, svboolx4_t tuple4, svbool_t res){
  svset2_b(tuple2, -1, res); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svset2_b(tuple2, 2,  res); // expected-error {{argument value 2 is outside the valid range [0, 1]}}
  svset4_b(tuple4, -1, res); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svset4_b(tuple4, 4,  res); // expected-error {{argument value 4 is outside the valid range [0, 3]}}

  svget2_b(tuple2, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svget2_b(tuple2,  2); // expected-error {{argument value 2 is outside the valid range [0, 1]}}
  svget4_b(tuple4, -1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svget4_b(tuple4,  4); // expected-error {{argument value 4 is outside the valid range [0, 3]}}

  svset2_b(tuple2, idx, res); // expected-error {{argument to 'svset2_b' must be a constant integer}}
  svset4_b(tuple4, idx, res); // expected-error {{argument to 'svset4_b' must be a constant integer}}
  svget2_b(tuple2, idx); // expected-error {{argument to 'svget2_b' must be a constant integer}}
  svget4_b(tuple4, idx); // expected-error {{argument to 'svget4_b' must be a constant integer}}
}

__attribute__((target("+sve2p1")))
void test_svdup_laneq(){  
  svuint8_t zn_u8;
  svuint16_t zn_u16;
  svuint32_t zn_u32;
  svuint64_t zn_u64;
  svint8_t zn_s8;
  svint16_t zn_s16;
  svint32_t zn_s32;
  svint64_t zn_s64;
  svfloat16_t zn_f16;
  svfloat32_t zn_f32;
  svfloat64_t zn_f64;
  svbfloat16_t zn_bf16;

  svdup_laneq_u8(zn_u8,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 15]}}
  svdup_laneq_u16(zn_u16,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}
  svdup_laneq_u32(zn_u32,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svdup_laneq_u64(zn_u64,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svdup_laneq_s8(zn_s8,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 15]}}
  svdup_laneq_s16(zn_s16,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}
  svdup_laneq_s32(zn_s32,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svdup_laneq_s64(zn_s64,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svdup_laneq_f16(zn_f16,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}
  svdup_laneq_f32(zn_f32,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 3]}}
  svdup_laneq_f64(zn_f64,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 1]}}
  svdup_laneq_bf16(zn_bf16,-1); // expected-error {{argument value 18446744073709551615 is outside the valid range [0, 7]}}
}