// RUN: %clang_cc1 -ffreestanding -fms-extensions -fms-compatibility -fms-compatibility-version=17.00 \
// RUN:         -triple i686--windows -Oz -emit-llvm %s -o - \
// RUN:         | FileCheck %s -check-prefixes CHECK,CHECK-I386,CHECK-INTEL
// RUN: %clang_cc1 -ffreestanding -fms-extensions -fms-compatibility -fms-compatibility-version=17.00 \
// RUN:         -triple thumbv7--windows -Oz -emit-llvm %s -o - \
// RUN:         | FileCheck %s --check-prefixes CHECK,CHECK-ARM,CHECK-ARM-ARM64,CHECK-ARM-X64
// RUN: %clang_cc1 -ffreestanding -fms-extensions -fms-compatibility -fms-compatibility-version=17.00 \
// RUN:         -triple x86_64--windows -Oz -emit-llvm -target-feature +cx16 %s -o - \
// RUN:         | FileCheck %s --check-prefixes CHECK,CHECK-X64,CHECK-ARM-X64,CHECK-INTEL,CHECK-64
// RUN: %clang_cc1 -ffreestanding -fms-extensions -fms-compatibility -fms-compatibility-version=17.00 \
// RUN:         -triple aarch64-windows -Oz -emit-llvm %s -o - \
// RUN:         | FileCheck %s --check-prefixes CHECK-ARM-ARM64,CHECK-ARM-X64,CHECK-ARM64,CHECK-64

// intrin.h needs size_t, but -ffreestanding prevents us from getting it from
// stddef.h.  Work around it with this typedef.
typedef __SIZE_TYPE__ size_t;

#include <intrin.h>

#if defined(__i386__) || defined(__x86_64__)
void test__stosb(unsigned char *Dest, unsigned char Data, size_t Count) {
  return __stosb(Dest, Data, Count);
}

// CHECK-I386: define{{.*}}void @test__stosb
// CHECK-I386:   tail call void @llvm.memset.p0.i32(ptr align 1 %Dest, i8 %Data, i32 %Count, i1 true)
// CHECK-I386:   ret void
// CHECK-I386: }

// CHECK-X64: define{{.*}}void @test__stosb
// CHECK-X64:   tail call void @llvm.memset.p0.i64(ptr align 1 %Dest, i8 %Data, i64 %Count, i1 true)
// CHECK-X64:   ret void
// CHECK-X64: }

void test__movsb(unsigned char *Dest, unsigned char *Src, size_t Count) {
  return __movsb(Dest, Src, Count);
}
// CHECK-I386-LABEL: define{{.*}} void @test__movsb
// CHECK-I386:   tail call { ptr, ptr, i32 } asm sideeffect "xchg $(%esi, $1$|$1, esi$)\0Arep movsb\0Axchg $(%esi, $1$|$1, esi$)", "={di},=r,={cx},0,1,2,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr %Dest, ptr %Src, i32 %Count)
// CHECK-I386:   ret void
// CHECK-I386: }

// CHECK-X64-LABEL: define{{.*}} void @test__movsb
// CHECK-X64:   call { ptr, ptr, i64 } asm sideeffect "rep movsb", "={di},={si},={cx},0,1,2,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr %Dest, ptr %Src, i64 %Count)
// CHECK-X64:   ret void
// CHECK-X64: }

void test__stosw(unsigned short *Dest, unsigned short Data, size_t Count) {
  return __stosw(Dest, Data, Count);
}
// CHECK-I386-LABEL: define{{.*}} void @test__stosw
// CHECK-I386:   call { ptr, i32 } asm sideeffect "rep stosw", "={di},={cx},{ax},0,1,~{memory},~{dirflag},~{fpsr},~{flags}"(i16 %Data, ptr %Dest, i32 %Count)
// CHECK-I386:   ret void
// CHECK-I386: }

// CHECK-X64-LABEL: define{{.*}} void @test__stosw
// CHECK-X64:   call { ptr, i64 } asm sideeffect "rep stosw", "={di},={cx},{ax},0,1,~{memory},~{dirflag},~{fpsr},~{flags}"(i16 %Data, ptr %Dest, i64 %Count)
// CHECK-X64:   ret void
// CHECK-X64: }

void test__movsw(unsigned short *Dest, unsigned short *Src, size_t Count) {
  return __movsw(Dest, Src, Count);
}
// CHECK-I386-LABEL: define{{.*}} void @test__movsw
// CHECK-I386:   tail call { ptr, ptr, i32 } asm sideeffect "xchg $(%esi, $1$|$1, esi$)\0Arep movsw\0Axchg $(%esi, $1$|$1, esi$)", "={di},=r,={cx},0,1,2,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr %Dest, ptr %Src, i32 %Count)
// CHECK-I386:   ret void
// CHECK-I386: }

// CHECK-X64-LABEL: define{{.*}} void @test__movsw
// CHECK-X64:   call { ptr, ptr, i64 } asm sideeffect "rep movsw", "={di},={si},={cx},0,1,2,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr %Dest, ptr %Src, i64 %Count)
// CHECK-X64:   ret void
// CHECK-X64: }

void test__stosd(unsigned long *Dest, unsigned long Data, size_t Count) {
  return __stosd(Dest, Data, Count);
}
// CHECK-I386-LABEL: define{{.*}} void @test__stosd
// CHECK-I386:   call { ptr, i32 } asm sideeffect "rep stos$(l$|d$)", "={di},={cx},{ax},0,1,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %Data, ptr %Dest, i32 %Count)
// CHECK-I386:   ret void
// CHECK-I386: }

// CHECK-X64-LABEL: define{{.*}} void @test__stosd
// CHECK-X64:   call { ptr, i64 } asm sideeffect "rep stos$(l$|d$)", "={di},={cx},{ax},0,1,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %Data, ptr %Dest, i64 %Count)
// CHECK-X64:   ret void
// CHECK-X64: }

void test__movsd(unsigned long *Dest, unsigned long *Src, size_t Count) {
  return __movsd(Dest, Src, Count);
}
// CHECK-I386-LABEL: define{{.*}} void @test__movsd
// CHECK-I386:   tail call { ptr, ptr, i32 } asm sideeffect "xchg $(%esi, $1$|$1, esi$)\0Arep movs$(l$|d$)\0Axchg $(%esi, $1$|$1, esi$)", "={di},=r,={cx},0,1,2,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr %Dest, ptr %Src, i32 %Count)
// CHECK-I386:   ret void
// CHECK-I386: }

// CHECK-X64-LABEL: define{{.*}} void @test__movsd
// CHECK-X64:   call { ptr, ptr, i64 } asm sideeffect "rep movs$(l$|d$)", "={di},={si},={cx},0,1,2,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr %Dest, ptr %Src, i64 %Count)
// CHECK-X64:   ret void
// CHECK-X64: }

#ifdef __x86_64__
void test__stosq(unsigned __int64 *Dest, unsigned __int64 Data, size_t Count) {
  return __stosq(Dest, Data, Count);
}
// CHECK-X64-LABEL: define{{.*}} void @test__stosq
// CHECK-X64:   call { ptr, i64 } asm sideeffect "rep stosq", "={di},={cx},{ax},0,1,~{memory},~{dirflag},~{fpsr},~{flags}"(i64 %Data, ptr %Dest, i64 %Count)
// CHECK-X64:   ret void
// CHECK-X64: }

void test__movsq(unsigned __int64 *Dest, unsigned __int64 *Src, size_t Count) {
  return __movsq(Dest, Src, Count);
}
// CHECK-X64-LABEL: define{{.*}} void @test__movsq
// CHECK-X64:   call { ptr, ptr, i64 } asm sideeffect "rep movsq", "={di},={si},={cx},0,1,2,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr %Dest, ptr %Src, i64 %Count)
// CHECK-X64:   ret void
// CHECK-X64: }
#endif

void test__ud2(void) {
  __ud2();
}
// CHECK-INTEL-LABEL: define{{.*}} void @test__ud2()
// CHECK-INTEL: call void @llvm.trap()

void test__int2c(void) {
  __int2c();
}
// CHECK-INTEL-LABEL: define{{.*}} void @test__int2c()
// CHECK-INTEL: call void asm sideeffect "int $$0x2c", ""() #[[NORETURN:[0-9]+]]


#endif

void *test_ReturnAddress(void) {
  return _ReturnAddress();
}
// CHECK-LABEL: define{{.*}}ptr @test_ReturnAddress()
// CHECK: = tail call ptr @llvm.returnaddress(i32 0)
// CHECK: ret ptr

#if defined(__i386__) || defined(__x86_64__) || defined (__aarch64__)
void *test_AddressOfReturnAddress(void) {
  return _AddressOfReturnAddress();
}
// CHECK-INTEL-LABEL: define dso_local ptr @test_AddressOfReturnAddress()
// CHECK-INTEL: = tail call ptr @llvm.addressofreturnaddress.p0()
// CHECK-INTEL: ret ptr
#endif

unsigned char test_BitScanForward(unsigned long *Index, unsigned long Mask) {
  return _BitScanForward(++Index, Mask);
}
// CHECK: define{{.*}}i8 @test_BitScanForward(ptr {{.*}}%Index, i32 {{[a-z_ ]*}}%Mask){{.*}}{
// CHECK:   [[ISNOTZERO:%[a-z0-9._]+]] = icmp eq i32 %Mask, 0
// CHECK:   br i1 [[ISNOTZERO]], label %[[END_LABEL:[a-z0-9._]+]], label %[[ISNOTZERO_LABEL:[a-z0-9._]+]]
// CHECK:   [[END_LABEL]]:
// CHECK:   [[RESULT:%[a-z0-9._]+]] = phi i8 [ 0, %[[ISZERO_LABEL:[a-z0-9._]+]] ], [ 1, %[[ISNOTZERO_LABEL]] ]
// CHECK:   ret i8 [[RESULT]]
// CHECK:   [[ISNOTZERO_LABEL]]:
// CHECK:   [[IDXGEP:%[a-z0-9._]+]] = getelementptr inbounds nuw i8, ptr %Index, {{i64|i32}} 4
// CHECK:   [[INDEX:%[0-9]+]] = tail call range(i32 0, 33) i32 @llvm.cttz.i32(i32 %Mask, i1 true)
// CHECK:   store i32 [[INDEX]], ptr [[IDXGEP]], align 4
// CHECK:   br label %[[END_LABEL]]

unsigned char test_BitScanReverse(unsigned long *Index, unsigned long Mask) {
  return _BitScanReverse(++Index, Mask);
}
// CHECK: define{{.*}}i8 @test_BitScanReverse(ptr {{.*}}%Index, i32 {{[a-z_ ]*}}%Mask){{.*}}{
// CHECK:   [[ISNOTZERO:%[0-9]+]] = icmp eq i32 %Mask, 0
// CHECK:   br i1 [[ISNOTZERO]], label %[[END_LABEL:[a-z0-9._]+]], label %[[ISNOTZERO_LABEL:[a-z0-9._]+]]
// CHECK:   [[END_LABEL]]:
// CHECK:   [[RESULT:%[a-z0-9._]+]] = phi i8 [ 0, %[[ISZERO_LABEL:[a-z0-9._]+]] ], [ 1, %[[ISNOTZERO_LABEL]] ]
// CHECK:   ret i8 [[RESULT]]
// CHECK:   [[ISNOTZERO_LABEL]]:
// CHECK:   [[IDXGEP:%[a-z0-9._]+]] = getelementptr inbounds nuw i8, ptr %Index, {{i64|i32}} 4
// CHECK:   [[REVINDEX:%[0-9]+]] = tail call range(i32 0, 33) i32 @llvm.ctlz.i32(i32 %Mask, i1 true)
// CHECK:   [[INDEX:%[0-9]+]] = xor i32 [[REVINDEX]], 31
// CHECK:   store i32 [[INDEX]], ptr [[IDXGEP]], align 4
// CHECK:   br label %[[END_LABEL]]

#if defined(__x86_64__) || defined(__arm__) || defined(__aarch64__)
unsigned char test_BitScanForward64(unsigned long *Index, unsigned __int64 Mask) {
  return _BitScanForward64(Index, Mask);
}
// CHECK-ARM-X64: define{{.*}}i8 @test_BitScanForward64(ptr {{.*}}%Index, i64 {{[a-z_ ]*}}%Mask){{.*}}{
// CHECK-ARM-X64:   [[ISNOTZERO:%[a-z0-9._]+]] = icmp eq i64 %Mask, 0
// CHECK-ARM-X64:   br i1 [[ISNOTZERO]], label %[[END_LABEL:[a-z0-9._]+]], label %[[ISNOTZERO_LABEL:[a-z0-9._]+]]
// CHECK-ARM-X64:   [[END_LABEL]]:
// CHECK-ARM-X64:   [[RESULT:%[a-z0-9._]+]] = phi i8 [ 0, %[[ISZERO_LABEL:[a-z0-9._]+]] ], [ 1, %[[ISNOTZERO_LABEL]] ]
// CHECK-ARM-X64:   ret i8 [[RESULT]]
// CHECK-ARM-X64:   [[ISNOTZERO_LABEL]]:
// CHECK-ARM-X64:   [[INDEX:%[0-9]+]] = tail call range(i64 0, 65) i64 @llvm.cttz.i64(i64 %Mask, i1 true)
// CHECK-ARM-X64:   [[TRUNC_INDEX:%[0-9]+]] = trunc nuw nsw i64 [[INDEX]] to i32
// CHECK-ARM-X64:   store i32 [[TRUNC_INDEX]], ptr %Index, align 4
// CHECK-ARM-X64:   br label %[[END_LABEL]]

unsigned char test_BitScanReverse64(unsigned long *Index, unsigned __int64 Mask) {
  return _BitScanReverse64(Index, Mask);
}
// CHECK-ARM-X64: define{{.*}}i8 @test_BitScanReverse64(ptr {{.*}}%Index, i64 {{[a-z_ ]*}}%Mask){{.*}}{
// CHECK-ARM-X64:   [[ISNOTZERO:%[0-9]+]] = icmp eq i64 %Mask, 0
// CHECK-ARM-X64:   br i1 [[ISNOTZERO]], label %[[END_LABEL:[a-z0-9._]+]], label %[[ISNOTZERO_LABEL:[a-z0-9._]+]]
// CHECK-ARM-X64:   [[END_LABEL]]:
// CHECK-ARM-X64:   [[RESULT:%[a-z0-9._]+]] = phi i8 [ 0, %[[ISZERO_LABEL:[a-z0-9._]+]] ], [ 1, %[[ISNOTZERO_LABEL]] ]
// CHECK-ARM-X64:   ret i8 [[RESULT]]
// CHECK-ARM-X64:   [[ISNOTZERO_LABEL]]:
// CHECK-ARM-X64:   [[REVINDEX:%[0-9]+]] = tail call range(i64 0, 65) i64 @llvm.ctlz.i64(i64 %Mask, i1 true)
// CHECK-ARM-X64:   [[TRUNC_REVINDEX:%[0-9]+]] = trunc nuw nsw i64 [[REVINDEX]] to i32
// CHECK-ARM-X64:   [[INDEX:%[0-9]+]] = xor i32 [[TRUNC_REVINDEX]], 63
// CHECK-ARM-X64:   store i32 [[INDEX]], ptr %Index, align 4
// CHECK-ARM-X64:   br label %[[END_LABEL]]
#endif

void *test_InterlockedExchangePointer(void * volatile *Target, void *Value) {
  return _InterlockedExchangePointer(Target, Value);
}

// CHECK: define{{.*}}ptr @test_InterlockedExchangePointer(ptr {{.*}}%Target, ptr {{[a-z_ ]*}}%Value){{.*}}{
// CHECK:   %[[VALUE:[0-9]+]] = ptrtoint ptr %Value to [[iPTR:i[0-9]+]]
// CHECK:   %[[EXCHANGE:[0-9]+]] = atomicrmw xchg ptr %Target, [[iPTR]] %[[VALUE]] seq_cst, align {{4|8}}
// CHECK:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXCHANGE]] to ptr
// CHECK:   ret ptr %[[RESULT]]
// CHECK: }

#if defined(__arm__) || defined(__aarch64__)
void *test_InterlockedExchangePointer_acq(void * volatile *Target, void *Value) {
  return _InterlockedExchangePointer_acq(Target, Value);
}

// CHECK-ARM-ARM64: define{{.*}}ptr @test_InterlockedExchangePointer_acq(ptr {{.*}}%Target, ptr {{[a-z_ ]*}}%Value){{.*}}{
// CHECK-ARM-ARM64:   %[[VALUE:[0-9]+]] = ptrtoint ptr %Value to [[iPTR:i[0-9]+]]
// CHECK-ARM-ARM64:   %[[EXCHANGE:[0-9]+]] = atomicrmw xchg ptr %Target, [[iPTR]] %[[VALUE]] acquire, align {{4|8}}
// CHECK-ARM-ARM64:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXCHANGE]] to ptr
// CHECK-ARM-ARM64:   ret ptr %[[RESULT]]
// CHECK-ARM-ARM64: }

void *test_InterlockedExchangePointer_nf(void * volatile *Target, void *Value) {
  return _InterlockedExchangePointer_nf(Target, Value);
}

// CHECK-ARM-ARM64: define{{.*}}ptr @test_InterlockedExchangePointer_nf(ptr {{.*}}%Target, ptr {{[a-z_ ]*}}%Value){{.*}}{
// CHECK-ARM-ARM64:   %[[VALUE:[0-9]+]] = ptrtoint ptr %Value to [[iPTR]]
// CHECK-ARM-ARM64:   %[[EXCHANGE:[0-9]+]] = atomicrmw xchg ptr %Target, [[iPTR]] %[[VALUE]] monotonic, align {{4|8}}
// CHECK-ARM-ARM64:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXCHANGE]] to ptr
// CHECK-ARM-ARM64:   ret ptr %[[RESULT]]
// CHECK-ARM-ARM64: }

void *test_InterlockedExchangePointer_rel(void * volatile *Target, void *Value) {
  return _InterlockedExchangePointer_rel(Target, Value);
}

// CHECK-ARM-ARM64: define{{.*}}ptr @test_InterlockedExchangePointer_rel(ptr {{.*}}%Target, ptr {{[a-z_ ]*}}%Value){{.*}}{
// CHECK-ARM-ARM64:   %[[VALUE:[0-9]+]] = ptrtoint ptr %Value to [[iPTR]]
// CHECK-ARM-ARM64:   %[[EXCHANGE:[0-9]+]] = atomicrmw xchg ptr %Target, [[iPTR]] %[[VALUE]] release, align {{4|8}}
// CHECK-ARM-ARM64:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXCHANGE]] to ptr
// CHECK-ARM-ARM64:   ret ptr %[[RESULT]]
// CHECK-ARM-ARM64: }
#endif

void *test_InterlockedCompareExchangePointer(void * volatile *Destination,
                                             void *Exchange, void *Comparand) {
  return _InterlockedCompareExchangePointer(Destination, Exchange, Comparand);
}

// CHECK: define{{.*}}ptr @test_InterlockedCompareExchangePointer(ptr {{.*}}%Destination, ptr {{[a-z_ ]*}}%Exchange, ptr {{[a-z_ ]*}}%Comparand){{.*}}{
// CHECK:   %[[EXCHANGE:[0-9]+]] = ptrtoint ptr %Exchange to [[iPTR]]
// CHECK:   %[[COMPARAND:[0-9]+]] = ptrtoint ptr %Comparand to [[iPTR]]
// CHECK:   %[[XCHG:[0-9]+]] = cmpxchg volatile ptr %[[DEST:.+]], [[iPTR]] %[[COMPARAND:[0-9]+]], [[iPTR]] %[[EXCHANGE:[0-9]+]] seq_cst seq_cst, align {{4|8}}
// CHECK:   %[[EXTRACT:[0-9]+]] = extractvalue { [[iPTR]], i1 } %[[XCHG]], 0
// CHECK:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXTRACT]] to ptr
// CHECK:   ret ptr %[[RESULT:[0-9]+]]
// CHECK: }

void *test_InterlockedCompareExchangePointer_nf(void * volatile *Destination,
                                             void *Exchange, void *Comparand) {
  return _InterlockedCompareExchangePointer_nf(Destination, Exchange, Comparand);
}

// CHECK: define{{.*}}ptr @test_InterlockedCompareExchangePointer_nf(ptr {{.*}}%Destination, ptr {{[a-z_ ]*}}%Exchange, ptr {{[a-z_ ]*}}%Comparand){{.*}}{
// CHECK:   %[[EXCHANGE:[0-9]+]] = ptrtoint ptr %Exchange to [[iPTR]]
// CHECK:   %[[COMPARAND:[0-9]+]] = ptrtoint ptr %Comparand to [[iPTR]]
// CHECK:   %[[XCHG:[0-9]+]] = cmpxchg volatile ptr %[[DEST:.+]], [[iPTR]] %[[COMPARAND:[0-9]+]], [[iPTR]] %[[EXCHANGE:[0-9]+]] monotonic monotonic, align {{4|8}}
// CHECK:   %[[EXTRACT:[0-9]+]] = extractvalue { [[iPTR]], i1 } %[[XCHG]], 0
// CHECK:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXTRACT]] to ptr
// CHECK:   ret ptr %[[RESULT:[0-9]+]]
// CHECK: }

#if defined(__arm__) || defined(__aarch64__)
void *test_InterlockedCompareExchangePointer_acq(void * volatile *Destination,
                                             void *Exchange, void *Comparand) {
  return _InterlockedCompareExchangePointer_acq(Destination, Exchange, Comparand);
}

// CHECK-ARM-ARM64: define{{.*}}ptr @test_InterlockedCompareExchangePointer_acq(ptr {{.*}}%Destination, ptr {{[a-z_ ]*}}%Exchange, ptr {{[a-z_ ]*}}%Comparand){{.*}}{
// CHECK-ARM-ARM64:   %[[EXCHANGE:[0-9]+]] = ptrtoint ptr %Exchange to [[iPTR]]
// CHECK-ARM-ARM64:   %[[COMPARAND:[0-9]+]] = ptrtoint ptr %Comparand to [[iPTR]]
// CHECK-ARM-ARM64:   %[[XCHG:[0-9]+]] = cmpxchg volatile ptr %[[DEST:.+]], [[iPTR]] %[[COMPARAND:[0-9]+]], [[iPTR]] %[[EXCHANGE:[0-9]+]] acquire acquire, align {{4|8}}
// CHECK-ARM-ARM64:   %[[EXTRACT:[0-9]+]] = extractvalue { [[iPTR]], i1 } %[[XCHG]], 0
// CHECK-ARM-ARM64:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXTRACT]] to ptr
// CHECK-ARM-ARM64:   ret ptr %[[RESULT:[0-9]+]]
// CHECK-ARM-ARM64: }


void *test_InterlockedCompareExchangePointer_rel(void * volatile *Destination,
                                             void *Exchange, void *Comparand) {
  return _InterlockedCompareExchangePointer_rel(Destination, Exchange, Comparand);
}

// CHECK-ARM-ARM64: define{{.*}}ptr @test_InterlockedCompareExchangePointer_rel(ptr {{.*}}%Destination, ptr {{[a-z_ ]*}}%Exchange, ptr {{[a-z_ ]*}}%Comparand){{.*}}{
// CHECK-ARM-ARM64:   %[[EXCHANGE:[0-9]+]] = ptrtoint ptr %Exchange to [[iPTR]]
// CHECK-ARM-ARM64:   %[[COMPARAND:[0-9]+]] = ptrtoint ptr %Comparand to [[iPTR]]
// CHECK-ARM-ARM64:   %[[XCHG:[0-9]+]] = cmpxchg volatile ptr %[[DEST:.+]], [[iPTR]] %[[COMPARAND:[0-9]+]], [[iPTR]] %[[EXCHANGE:[0-9]+]] release monotonic, align {{4|8}}
// CHECK-ARM-ARM64:   %[[EXTRACT:[0-9]+]] = extractvalue { [[iPTR]], i1 } %[[XCHG]], 0
// CHECK-ARM-ARM64:   %[[RESULT:[0-9]+]] = inttoptr [[iPTR]] %[[EXTRACT]] to ptr
// CHECK-ARM-ARM64:   ret ptr %[[RESULT:[0-9]+]]
// CHECK-ARM-ARM64: }
#endif

char test_InterlockedExchange8(char volatile *value, char mask) {
  return _InterlockedExchange8(value, mask);
}
// CHECK: define{{.*}}i8 @test_InterlockedExchange8(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i8 %mask seq_cst, align 1
// CHECK:   ret i8 [[RESULT:%[0-9]+]]
// CHECK: }

short test_InterlockedExchange16(short volatile *value, short mask) {
  return _InterlockedExchange16(value, mask);
}
// CHECK: define{{.*}}i16 @test_InterlockedExchange16(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i16 %mask seq_cst, align 2
// CHECK:   ret i16 [[RESULT:%[0-9]+]]
// CHECK: }

long test_InterlockedExchange(long volatile *value, long mask) {
  return _InterlockedExchange(value, mask);
}
// CHECK: define{{.*}}i32 @test_InterlockedExchange(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i32 %mask seq_cst, align 4
// CHECK:   ret i32 [[RESULT:%[0-9]+]]
// CHECK: }

char test_InterlockedExchangeAdd8(char volatile *value, char mask) {
  return _InterlockedExchangeAdd8(value, mask);
}
// CHECK: define{{.*}}i8 @test_InterlockedExchangeAdd8(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i8 %mask seq_cst, align 1
// CHECK:   ret i8 [[RESULT:%[0-9]+]]
// CHECK: }

short test_InterlockedExchangeAdd16(short volatile *value, short mask) {
  return _InterlockedExchangeAdd16(value, mask);
}
// CHECK: define{{.*}}i16 @test_InterlockedExchangeAdd16(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i16 %mask seq_cst, align 2
// CHECK:   ret i16 [[RESULT:%[0-9]+]]
// CHECK: }

long test_InterlockedExchangeAdd(long volatile *value, long mask) {
  return _InterlockedExchangeAdd(value, mask);
}
// CHECK: define{{.*}}i32 @test_InterlockedExchangeAdd(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i32 %mask seq_cst, align 4
// CHECK:   ret i32 [[RESULT:%[0-9]+]]
// CHECK: }

char test_InterlockedExchangeSub8(char volatile *value, char mask) {
  return _InterlockedExchangeSub8(value, mask);
}
// CHECK: define{{.*}}i8 @test_InterlockedExchangeSub8(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw sub ptr %value, i8 %mask seq_cst, align 1
// CHECK:   ret i8 [[RESULT:%[0-9]+]]
// CHECK: }

short test_InterlockedExchangeSub16(short volatile *value, short mask) {
  return _InterlockedExchangeSub16(value, mask);
}
// CHECK: define{{.*}}i16 @test_InterlockedExchangeSub16(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw sub ptr %value, i16 %mask seq_cst, align 2
// CHECK:   ret i16 [[RESULT:%[0-9]+]]
// CHECK: }

long test_InterlockedExchangeSub(long volatile *value, long mask) {
  return _InterlockedExchangeSub(value, mask);
}
// CHECK: define{{.*}}i32 @test_InterlockedExchangeSub(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw sub ptr %value, i32 %mask seq_cst, align 4
// CHECK:   ret i32 [[RESULT:%[0-9]+]]
// CHECK: }

char test_InterlockedOr8(char volatile *value, char mask) {
  return _InterlockedOr8(value, mask);
}
// CHECK: define{{.*}}i8 @test_InterlockedOr8(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i8 %mask seq_cst, align 1
// CHECK:   ret i8 [[RESULT:%[0-9]+]]
// CHECK: }

short test_InterlockedOr16(short volatile *value, short mask) {
  return _InterlockedOr16(value, mask);
}
// CHECK: define{{.*}}i16 @test_InterlockedOr16(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i16 %mask seq_cst, align 2
// CHECK:   ret i16 [[RESULT:%[0-9]+]]
// CHECK: }

long test_InterlockedOr(long volatile *value, long mask) {
  return _InterlockedOr(value, mask);
}
// CHECK: define{{.*}}i32 @test_InterlockedOr(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i32 %mask seq_cst, align 4
// CHECK:   ret i32 [[RESULT:%[0-9]+]]
// CHECK: }

char test_InterlockedXor8(char volatile *value, char mask) {
  return _InterlockedXor8(value, mask);
}
// CHECK: define{{.*}}i8 @test_InterlockedXor8(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i8 %mask seq_cst, align 1
// CHECK:   ret i8 [[RESULT:%[0-9]+]]
// CHECK: }

short test_InterlockedXor16(short volatile *value, short mask) {
  return _InterlockedXor16(value, mask);
}
// CHECK: define{{.*}}i16 @test_InterlockedXor16(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i16 %mask seq_cst, align 2
// CHECK:   ret i16 [[RESULT:%[0-9]+]]
// CHECK: }

long test_InterlockedXor(long volatile *value, long mask) {
  return _InterlockedXor(value, mask);
}
// CHECK: define{{.*}}i32 @test_InterlockedXor(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i32 %mask seq_cst, align 4
// CHECK:   ret i32 [[RESULT:%[0-9]+]]
// CHECK: }

char test_InterlockedAnd8(char volatile *value, char mask) {
  return _InterlockedAnd8(value, mask);
}
// CHECK: define{{.*}}i8 @test_InterlockedAnd8(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i8 %mask seq_cst, align 1
// CHECK:   ret i8 [[RESULT:%[0-9]+]]
// CHECK: }

short test_InterlockedAnd16(short volatile *value, short mask) {
  return _InterlockedAnd16(value, mask);
}
// CHECK: define{{.*}}i16 @test_InterlockedAnd16(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i16 %mask seq_cst, align 2
// CHECK:   ret i16 [[RESULT:%[0-9]+]]
// CHECK: }

long test_InterlockedAnd(long volatile *value, long mask) {
  return _InterlockedAnd(value, mask);
}
// CHECK: define{{.*}}i32 @test_InterlockedAnd(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i32 %mask seq_cst, align 4
// CHECK:   ret i32 [[RESULT:%[0-9]+]]
// CHECK: }

char test_InterlockedCompareExchange8(char volatile *Destination, char Exchange, char Comperand) {
  return _InterlockedCompareExchange8(Destination, Exchange, Comperand);
}
// CHECK: define{{.*}}i8 @test_InterlockedCompareExchange8(ptr{{.*}}%Destination, i8{{[a-z_ ]*}}%Exchange, i8{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i8 %Comperand, i8 %Exchange seq_cst seq_cst, align 1
// CHECK: [[RESULT:%[0-9]+]] = extractvalue { i8, i1 } [[TMP]], 0
// CHECK: ret i8 [[RESULT]]
// CHECK: }

short test_InterlockedCompareExchange16(short volatile *Destination, short Exchange, short Comperand) {
  return _InterlockedCompareExchange16(Destination, Exchange, Comperand);
}
// CHECK: define{{.*}}i16 @test_InterlockedCompareExchange16(ptr{{.*}}%Destination, i16{{[a-z_ ]*}}%Exchange, i16{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i16 %Comperand, i16 %Exchange seq_cst seq_cst, align 2
// CHECK: [[RESULT:%[0-9]+]] = extractvalue { i16, i1 } [[TMP]], 0
// CHECK: ret i16 [[RESULT]]
// CHECK: }

long test_InterlockedCompareExchange(long volatile *Destination, long Exchange, long Comperand) {
  return _InterlockedCompareExchange(Destination, Exchange, Comperand);
}
// CHECK: define{{.*}}i32 @test_InterlockedCompareExchange(ptr{{.*}}%Destination, i32{{[a-z_ ]*}}%Exchange, i32{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i32 %Comperand, i32 %Exchange seq_cst seq_cst, align 4
// CHECK: [[RESULT:%[0-9]+]] = extractvalue { i32, i1 } [[TMP]], 0
// CHECK: ret i32 [[RESULT]]
// CHECK: }

__int64 test_InterlockedCompareExchange64(__int64 volatile *Destination, __int64 Exchange, __int64 Comperand) {
  return _InterlockedCompareExchange64(Destination, Exchange, Comperand);
}
// CHECK: define{{.*}}i64 @test_InterlockedCompareExchange64(ptr{{.*}}%Destination, i64{{[a-z_ ]*}}%Exchange, i64{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i64 %Comperand, i64 %Exchange seq_cst seq_cst, align 8
// CHECK: [[RESULT:%[0-9]+]] = extractvalue { i64, i1 } [[TMP]], 0
// CHECK: ret i64 [[RESULT]]
// CHECK: }

#if defined(__x86_64__) || defined(__aarch64__)
unsigned char test_InterlockedCompareExchange128(
    __int64 volatile *Destination, __int64 ExchangeHigh,
    __int64 ExchangeLow, __int64 *ComparandResult) {
  return _InterlockedCompareExchange128(++Destination, ++ExchangeHigh,
                                        ++ExchangeLow, ++ComparandResult);
}
// CHECK-64: define{{.*}}i8 @test_InterlockedCompareExchange128(ptr{{.*}}%Destination, i64{{[a-z_ ]*}}%ExchangeHigh, i64{{[a-z_ ]*}}%ExchangeLow, ptr{{.*}}%ComparandResult){{.*}}{
// CHECK-64: %incdec.ptr = getelementptr inbounds nuw i8, ptr %Destination, i64 8
// CHECK-64: %inc = add nsw i64 %ExchangeHigh, 1
// CHECK-64: %inc1 = add nsw i64 %ExchangeLow, 1
// CHECK-64: %incdec.ptr2 = getelementptr inbounds nuw i8, ptr %ComparandResult, i64 8
// CHECK-64: [[EH:%[0-9]+]] = zext i64 %inc to i128
// CHECK-64: [[EL:%[0-9]+]] = zext i64 %inc1 to i128
// CHECK-64: [[EHS:%[0-9]+]] = shl nuw i128 [[EH]], 64
// CHECK-64: [[EXP:%[0-9]+]] = or disjoint i128 [[EHS]], [[EL]]
// CHECK-64: [[ORG:%[0-9]+]] = load i128, ptr %incdec.ptr2, align 8
// CHECK-64: [[RES:%[0-9]+]] = cmpxchg volatile ptr %incdec.ptr, i128 [[ORG]], i128 [[EXP]] seq_cst seq_cst, align 16
// CHECK-64: [[OLD:%[0-9]+]] = extractvalue { i128, i1 } [[RES]], 0
// CHECK-64: store i128 [[OLD]], ptr %incdec.ptr2, align 8
// CHECK-64: [[SUC1:%[0-9]+]] = extractvalue { i128, i1 } [[RES]], 1
// CHECK-64: [[SUC8:%[0-9]+]] = zext i1 [[SUC1]] to i8
// CHECK-64: ret i8 [[SUC8]]
// CHECK-64: }
#endif

#if defined(__aarch64__)
unsigned char test_InterlockedCompareExchange128_acq(
    __int64 volatile *Destination, __int64 ExchangeHigh,
    __int64 ExchangeLow, __int64 *ComparandResult) {
  return _InterlockedCompareExchange128_acq(Destination, ExchangeHigh,
                                            ExchangeLow, ComparandResult);
}
unsigned char test_InterlockedCompareExchange128_nf(
    __int64 volatile *Destination, __int64 ExchangeHigh,
    __int64 ExchangeLow, __int64 *ComparandResult) {
  return _InterlockedCompareExchange128_nf(Destination, ExchangeHigh,
                                           ExchangeLow, ComparandResult);
}
unsigned char test_InterlockedCompareExchange128_rel(
    __int64 volatile *Destination, __int64 ExchangeHigh,
    __int64 ExchangeLow, __int64 *ComparandResult) {
  return _InterlockedCompareExchange128_rel(Destination, ExchangeHigh,
                                            ExchangeLow, ComparandResult);
}
// CHECK-ARM64: define{{.*}}i8 @test_InterlockedCompareExchange128_acq({{.*}})
// CHECK-ARM64: cmpxchg volatile ptr %{{.*}}, i128 %{{.*}}, i128 %{{.*}} acquire acquire, align 16
// CHECK-ARM64: define{{.*}}i8 @test_InterlockedCompareExchange128_nf({{.*}})
// CHECK-ARM64: cmpxchg volatile ptr %{{.*}}, i128 %{{.*}}, i128 %{{.*}} monotonic monotonic, align 16
// CHECK-ARM64: define{{.*}}i8 @test_InterlockedCompareExchange128_rel({{.*}})
// CHECK-ARM64: cmpxchg volatile ptr %{{.*}}, i128 %{{.*}}, i128 %{{.*}} release monotonic, align 16
#endif

short test_InterlockedIncrement16(short volatile *Addend) {
  return _InterlockedIncrement16(++Addend);
}
// CHECK: define{{.*}}i16 @test_InterlockedIncrement16(ptr{{.*}}%Addend){{.*}}{
// CHECK: %incdec.ptr = getelementptr inbounds nuw i8, ptr %Addend, {{i64|i32}} 2
// CHECK: [[TMP:%[0-9]+]] = atomicrmw add ptr %incdec.ptr, i16 1 seq_cst, align 2
// CHECK: [[RESULT:%[0-9]+]] = add i16 [[TMP]], 1
// CHECK: ret i16 [[RESULT]]
// CHECK: }

long test_InterlockedIncrement(long volatile *Addend) {
  return _InterlockedIncrement(++Addend);
}
// CHECK: define{{.*}}i32 @test_InterlockedIncrement(ptr{{.*}}%Addend){{.*}}{
// CHECK: %incdec.ptr = getelementptr inbounds nuw i8, ptr %Addend, {{i64|i32}} 4
// CHECK: [[TMP:%[0-9]+]] = atomicrmw add ptr %incdec.ptr, i32 1 seq_cst, align 4
// CHECK: [[RESULT:%[0-9]+]] = add i32 [[TMP]], 1
// CHECK: ret i32 [[RESULT]]
// CHECK: }

short test_InterlockedDecrement16(short volatile *Addend) {
  return _InterlockedDecrement16(Addend);
}
// CHECK: define{{.*}}i16 @test_InterlockedDecrement16(ptr{{.*}}%Addend){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i16 1 seq_cst, align 2
// CHECK: [[RESULT:%[0-9]+]] = add i16 [[TMP]], -1
// CHECK: ret i16 [[RESULT]]
// CHECK: }

long test_InterlockedDecrement(long volatile *Addend) {
  return _InterlockedDecrement(Addend);
}
// CHECK: define{{.*}}i32 @test_InterlockedDecrement(ptr{{.*}}%Addend){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i32 1 seq_cst, align 4
// CHECK: [[RESULT:%[0-9]+]] = add i32 [[TMP]], -1
// CHECK: ret i32 [[RESULT]]
// CHECK: }

char test_iso_volatile_load8(char volatile *p) { return __iso_volatile_load8(p); }
short test_iso_volatile_load16(short volatile *p) { return __iso_volatile_load16(p); }
int test_iso_volatile_load32(int volatile *p) { return __iso_volatile_load32(p); }
__int64 test_iso_volatile_load64(__int64 volatile *p) { return __iso_volatile_load64(p); }

// CHECK: define{{.*}}i8 @test_iso_volatile_load8(ptr{{.*}}%p)
// CHECK: = load volatile i8, ptr %p
// CHECK: define{{.*}}i16 @test_iso_volatile_load16(ptr{{.*}}%p)
// CHECK: = load volatile i16, ptr %p
// CHECK: define{{.*}}i32 @test_iso_volatile_load32(ptr{{.*}}%p)
// CHECK: = load volatile i32, ptr %p
// CHECK: define{{.*}}i64 @test_iso_volatile_load64(ptr{{.*}}%p)
// CHECK: = load volatile i64, ptr %p

void test_iso_volatile_store8(char volatile *p, char v) { __iso_volatile_store8(p, v); }
void test_iso_volatile_store16(short volatile *p, short v) { __iso_volatile_store16(p, v); }
void test_iso_volatile_store32(int volatile *p, int v) { __iso_volatile_store32(p, v); }
void test_iso_volatile_store64(__int64 volatile *p, __int64 v) { __iso_volatile_store64(p, v); }

// CHECK: define{{.*}}void @test_iso_volatile_store8(ptr{{.*}}%p, i8 {{[a-z_ ]*}}%v)
// CHECK: store volatile i8 %v, ptr %p
// CHECK: define{{.*}}void @test_iso_volatile_store16(ptr{{.*}}%p, i16 {{[a-z_ ]*}}%v)
// CHECK: store volatile i16 %v, ptr %p
// CHECK: define{{.*}}void @test_iso_volatile_store32(ptr{{.*}}%p, i32 {{[a-z_ ]*}}%v)
// CHECK: store volatile i32 %v, ptr %p
// CHECK: define{{.*}}void @test_iso_volatile_store64(ptr{{.*}}%p, i64 {{[a-z_ ]*}}%v)
// CHECK: store volatile i64 %v, ptr %p


#if defined(__i386__) || defined(__x86_64__) || defined(__arm__) || defined(__aarch64__)
__int64 test_InterlockedExchange64(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchange64(value, mask);
}
// CHECK: define{{.*}}i64 @test_InterlockedExchange64(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i64 %mask seq_cst, align 8
// CHECK:   ret i64 [[RESULT:%[0-9]+]]
// CHECK: }

__int64 test_InterlockedExchangeAdd64(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchangeAdd64(value, mask);
}
// CHECK: define{{.*}}i64 @test_InterlockedExchangeAdd64(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i64 %mask seq_cst, align 8
// CHECK:   ret i64 [[RESULT:%[0-9]+]]
// CHECK: }

__int64 test_InterlockedExchangeSub64(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchangeSub64(value, mask);
}
// CHECK: define{{.*}}i64 @test_InterlockedExchangeSub64(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw sub ptr %value, i64 %mask seq_cst, align 8
// CHECK:   ret i64 [[RESULT:%[0-9]+]]
// CHECK: }

__int64 test_InterlockedOr64(__int64 volatile *value, __int64 mask) {
  return _InterlockedOr64(value, mask);
}
// CHECK: define{{.*}}i64 @test_InterlockedOr64(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i64 %mask seq_cst, align 8
// CHECK:   ret i64 [[RESULT:%[0-9]+]]
// CHECK: }

__int64 test_InterlockedXor64(__int64 volatile *value, __int64 mask) {
  return _InterlockedXor64(value, mask);
}
// CHECK: define{{.*}}i64 @test_InterlockedXor64(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i64 %mask seq_cst, align 8
// CHECK:   ret i64 [[RESULT:%[0-9]+]]
// CHECK: }

__int64 test_InterlockedAnd64(__int64 volatile *value, __int64 mask) {
  return _InterlockedAnd64(value, mask);
}
// CHECK: define{{.*}}i64 @test_InterlockedAnd64(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i64 %mask seq_cst, align 8
// CHECK:   ret i64 [[RESULT:%[0-9]+]]
// CHECK: }

__int64 test_InterlockedIncrement64(__int64 volatile *Addend) {
  return _InterlockedIncrement64(Addend);
}
// CHECK: define{{.*}}i64 @test_InterlockedIncrement64(ptr{{.*}}%Addend){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i64 1 seq_cst, align 8
// CHECK: [[RESULT:%[0-9]+]] = add i64 [[TMP]], 1
// CHECK: ret i64 [[RESULT]]
// CHECK: }

__int64 test_InterlockedDecrement64(__int64 volatile *Addend) {
  return _InterlockedDecrement64(Addend);
}
// CHECK: define{{.*}}i64 @test_InterlockedDecrement64(ptr{{.*}}%Addend){{.*}}{
// CHECK: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i64 1 seq_cst, align 8
// CHECK: [[RESULT:%[0-9]+]] = add i64 [[TMP]], -1
// CHECK: ret i64 [[RESULT]]
// CHECK: }

#endif

#if defined(__i386__) || defined(__x86_64__)
long test_InterlockedExchange_HLEAcquire(long volatile *Target, long Value) {
// CHECK-INTEL: define{{.*}} i32 @test_InterlockedExchange_HLEAcquire(ptr{{.*}}%Target, i32{{[a-z_ ]*}}%Value)
// CHECK-INTEL: call i32 asm sideeffect ".byte 0xf2 ; lock ; xchg $($0, $1$|$1, $0$)", "=r,=*m,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i32) %Target, i32 %Value, ptr elementtype(i32) %Target)
  return _InterlockedExchange_HLEAcquire(Target, Value);
}
long test_InterlockedExchange_HLERelease(long volatile *Target, long Value) {
// CHECK-INTEL: define{{.*}} i32 @test_InterlockedExchange_HLERelease(ptr{{.*}}%Target, i32{{[a-z_ ]*}}%Value)
// CHECK-INTEL: call i32 asm sideeffect ".byte 0xf3 ; lock ; xchg $($0, $1$|$1, $0$)", "=r,=*m,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i32) %Target, i32 %Value, ptr elementtype(i32) %Target)
  return _InterlockedExchange_HLERelease(Target, Value);
}
long test_InterlockedCompareExchange_HLEAcquire(long volatile *Destination,
                                                long Exchange, long Comparand) {
// CHECK-INTEL: define{{.*}} i32 @test_InterlockedCompareExchange_HLEAcquire(ptr{{.*}}%Destination, i32{{[a-z_ ]*}}%Exchange, i32{{[a-z_ ]*}}%Comparand)
// CHECK-INTEL: call i32 asm sideeffect ".byte 0xf2 ; lock ; cmpxchg $($2, $1$|$1, $2$)", "={ax},=*m,r,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i32) %Destination, i32 %Exchange, i32 %Comparand, ptr elementtype(i32) %Destination)
  return _InterlockedCompareExchange_HLEAcquire(Destination, Exchange, Comparand);
}
long test_InterlockedCompareExchange_HLERelease(long volatile *Destination,
                                            long Exchange, long Comparand) {
// CHECK-INTEL: define{{.*}} i32 @test_InterlockedCompareExchange_HLERelease(ptr{{.*}}%Destination, i32{{[a-z_ ]*}}%Exchange, i32{{[a-z_ ]*}}%Comparand)
// CHECK-INTEL: call i32 asm sideeffect ".byte 0xf3 ; lock ; cmpxchg $($2, $1$|$1, $2$)", "={ax},=*m,r,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i32) %Destination, i32 %Exchange, i32 %Comparand, ptr elementtype(i32) %Destination)
  return _InterlockedCompareExchange_HLERelease(Destination, Exchange, Comparand);
}
#endif
#if defined(__x86_64__)
__int64 test_InterlockedExchange64_HLEAcquire(__int64 volatile *Target, __int64 Value) {
// CHECK-X64: define{{.*}} i64 @test_InterlockedExchange64_HLEAcquire(ptr{{.*}}%Target, i64{{[a-z_ ]*}}%Value)
// CHECK-X64: call i64 asm sideeffect ".byte 0xf2 ; lock ; xchg $($0, $1$|$1, $0$)", "=r,=*m,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i64) %Target, i64 %Value, ptr elementtype(i64) %Target)
  return _InterlockedExchange64_HLEAcquire(Target, Value);
}
__int64 test_InterlockedExchange64_HLERelease(__int64 volatile *Target, __int64 Value) {
// CHECK-X64: define{{.*}} i64 @test_InterlockedExchange64_HLERelease(ptr{{.*}}%Target, i64{{[a-z_ ]*}}%Value)
// CHECK-X64: call i64 asm sideeffect ".byte 0xf3 ; lock ; xchg $($0, $1$|$1, $0$)", "=r,=*m,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i64) %Target, i64 %Value, ptr elementtype(i64) %Target)
  return _InterlockedExchange64_HLERelease(Target, Value);
}
__int64 test_InterlockedCompareExchange64_HLEAcquire(__int64 volatile *Destination,
                                                     __int64 Exchange, __int64 Comparand) {
// CHECK-X64: define{{.*}} i64 @test_InterlockedCompareExchange64_HLEAcquire(ptr{{.*}}%Destination, i64{{[a-z_ ]*}}%Exchange, i64{{[a-z_ ]*}}%Comparand)
// CHECK-X64: call i64 asm sideeffect ".byte 0xf2 ; lock ; cmpxchg $($2, $1$|$1, $2$)", "={ax},=*m,r,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i64) %Destination, i64 %Exchange, i64 %Comparand, ptr elementtype(i64) %Destination)
  return _InterlockedCompareExchange64_HLEAcquire(Destination, Exchange, Comparand);
}
__int64 test_InterlockedCompareExchange64_HLERelease(__int64 volatile *Destination,
                                                     __int64 Exchange, __int64 Comparand) {
// CHECK-X64: define{{.*}} i64 @test_InterlockedCompareExchange64_HLERelease(ptr{{.*}}%Destination, i64{{[a-z_ ]*}}%Exchange, i64{{[a-z_ ]*}}%Comparand)
// CHECK-X64: call i64 asm sideeffect ".byte 0xf3 ; lock ; cmpxchg $($2, $1$|$1, $2$)", "={ax},=*m,r,0,*m,~{memory},~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i64) %Destination, i64 %Exchange, i64 %Comparand, ptr elementtype(i64) %Destination)
  return _InterlockedCompareExchange64_HLERelease(Destination, Exchange, Comparand);
}
#endif

#if defined(__arm__) || defined(__aarch64__)
char test_InterlockedExchangeAdd8_acq(char volatile *value, char mask) {
  return _InterlockedExchangeAdd8_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedExchangeAdd8_acq(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i8 %mask acquire, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
char test_InterlockedExchangeAdd8_rel(char volatile *value, char mask) {
  return _InterlockedExchangeAdd8_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedExchangeAdd8_rel(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i8 %mask release, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
char test_InterlockedExchangeAdd8_nf(char volatile *value, char mask) {
  return _InterlockedExchangeAdd8_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedExchangeAdd8_nf(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i8 %mask monotonic, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
short test_InterlockedExchangeAdd16_acq(short volatile *value, short mask) {
  return _InterlockedExchangeAdd16_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedExchangeAdd16_acq(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i16 %mask acquire, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
short test_InterlockedExchangeAdd16_rel(short volatile *value, short mask) {
  return _InterlockedExchangeAdd16_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedExchangeAdd16_rel(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i16 %mask release, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
short test_InterlockedExchangeAdd16_nf(short volatile *value, short mask) {
  return _InterlockedExchangeAdd16_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedExchangeAdd16_nf(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i16 %mask monotonic, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
long test_InterlockedExchangeAdd_acq(long volatile *value, long mask) {
  return _InterlockedExchangeAdd_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedExchangeAdd_acq(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i32 %mask acquire, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
long test_InterlockedExchangeAdd_rel(long volatile *value, long mask) {
  return _InterlockedExchangeAdd_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedExchangeAdd_rel(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i32 %mask release, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
long test_InterlockedExchangeAdd_nf(long volatile *value, long mask) {
  return _InterlockedExchangeAdd_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedExchangeAdd_nf(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i32 %mask monotonic, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
__int64 test_InterlockedExchangeAdd64_acq(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchangeAdd64_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedExchangeAdd64_acq(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i64 %mask acquire, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
__int64 test_InterlockedExchangeAdd64_rel(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchangeAdd64_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedExchangeAdd64_rel(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i64 %mask release, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
__int64 test_InterlockedExchangeAdd64_nf(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchangeAdd64_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedExchangeAdd64_nf(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw add ptr %value, i64 %mask monotonic, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedExchange8_acq(char volatile *value, char mask) {
  return _InterlockedExchange8_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedExchange8_acq(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i8 %mask acquire, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
char test_InterlockedExchange8_rel(char volatile *value, char mask) {
  return _InterlockedExchange8_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedExchange8_rel(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i8 %mask release, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
char test_InterlockedExchange8_nf(char volatile *value, char mask) {
  return _InterlockedExchange8_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedExchange8_nf(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i8 %mask monotonic, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
short test_InterlockedExchange16_acq(short volatile *value, short mask) {
  return _InterlockedExchange16_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedExchange16_acq(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i16 %mask acquire, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
short test_InterlockedExchange16_rel(short volatile *value, short mask) {
  return _InterlockedExchange16_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedExchange16_rel(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i16 %mask release, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
short test_InterlockedExchange16_nf(short volatile *value, short mask) {
  return _InterlockedExchange16_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedExchange16_nf(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i16 %mask monotonic, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
long test_InterlockedExchange_acq(long volatile *value, long mask) {
  return _InterlockedExchange_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedExchange_acq(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i32 %mask acquire, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
long test_InterlockedExchange_rel(long volatile *value, long mask) {
  return _InterlockedExchange_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedExchange_rel(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i32 %mask release, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
long test_InterlockedExchange_nf(long volatile *value, long mask) {
  return _InterlockedExchange_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedExchange_nf(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i32 %mask monotonic, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
__int64 test_InterlockedExchange64_acq(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchange64_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedExchange64_acq(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i64 %mask acquire, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
__int64 test_InterlockedExchange64_rel(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchange64_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedExchange64_rel(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i64 %mask release, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }
__int64 test_InterlockedExchange64_nf(__int64 volatile *value, __int64 mask) {
  return _InterlockedExchange64_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedExchange64_nf(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xchg ptr %value, i64 %mask monotonic, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedCompareExchange8_acq(char volatile *Destination, char Exchange, char Comperand) {
  return _InterlockedCompareExchange8_acq(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedCompareExchange8_acq(ptr{{.*}}%Destination, i8{{[a-z_ ]*}}%Exchange, i8{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i8 %Comperand, i8 %Exchange acquire acquire, align 1
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i8, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i8 [[RESULT]]
// CHECK-ARM-ARM64: }

char test_InterlockedCompareExchange8_rel(char volatile *Destination, char Exchange, char Comperand) {
  return _InterlockedCompareExchange8_rel(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedCompareExchange8_rel(ptr{{.*}}%Destination, i8{{[a-z_ ]*}}%Exchange, i8{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i8 %Comperand, i8 %Exchange release monotonic, align 1
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i8, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i8 [[RESULT]]
// CHECK-ARM-ARM64: }

char test_InterlockedCompareExchange8_nf(char volatile *Destination, char Exchange, char Comperand) {
  return _InterlockedCompareExchange8_nf(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedCompareExchange8_nf(ptr{{.*}}%Destination, i8{{[a-z_ ]*}}%Exchange, i8{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i8 %Comperand, i8 %Exchange monotonic monotonic, align 1
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i8, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i8 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedCompareExchange16_acq(short volatile *Destination, short Exchange, short Comperand) {
  return _InterlockedCompareExchange16_acq(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedCompareExchange16_acq(ptr{{.*}}%Destination, i16{{[a-z_ ]*}}%Exchange, i16{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i16 %Comperand, i16 %Exchange acquire acquire, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i16, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedCompareExchange16_rel(short volatile *Destination, short Exchange, short Comperand) {
  return _InterlockedCompareExchange16_rel(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedCompareExchange16_rel(ptr{{.*}}%Destination, i16{{[a-z_ ]*}}%Exchange, i16{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i16 %Comperand, i16 %Exchange release monotonic, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i16, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedCompareExchange16_nf(short volatile *Destination, short Exchange, short Comperand) {
  return _InterlockedCompareExchange16_nf(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedCompareExchange16_nf(ptr{{.*}}%Destination, i16{{[a-z_ ]*}}%Exchange, i16{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i16 %Comperand, i16 %Exchange monotonic monotonic, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i16, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedCompareExchange_acq(long volatile *Destination, long Exchange, long Comperand) {
  return _InterlockedCompareExchange_acq(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedCompareExchange_acq(ptr{{.*}}%Destination, i32{{[a-z_ ]*}}%Exchange, i32{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i32 %Comperand, i32 %Exchange acquire acquire, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i32, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedCompareExchange_rel(long volatile *Destination, long Exchange, long Comperand) {
  return _InterlockedCompareExchange_rel(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedCompareExchange_rel(ptr{{.*}}%Destination, i32{{[a-z_ ]*}}%Exchange, i32{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i32 %Comperand, i32 %Exchange release monotonic, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i32, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedCompareExchange_nf(long volatile *Destination, long Exchange, long Comperand) {
  return _InterlockedCompareExchange_nf(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedCompareExchange_nf(ptr{{.*}}%Destination, i32{{[a-z_ ]*}}%Exchange, i32{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i32 %Comperand, i32 %Exchange monotonic monotonic, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i32, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedCompareExchange64_acq(__int64 volatile *Destination, __int64 Exchange, __int64 Comperand) {
  return _InterlockedCompareExchange64_acq(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedCompareExchange64_acq(ptr{{.*}}%Destination, i64{{[a-z_ ]*}}%Exchange, i64{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i64 %Comperand, i64 %Exchange acquire acquire, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i64, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedCompareExchange64_rel(__int64 volatile *Destination, __int64 Exchange, __int64 Comperand) {
  return _InterlockedCompareExchange64_rel(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedCompareExchange64_rel(ptr{{.*}}%Destination, i64{{[a-z_ ]*}}%Exchange, i64{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i64 %Comperand, i64 %Exchange release monotonic, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i64, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedCompareExchange64_nf(__int64 volatile *Destination, __int64 Exchange, __int64 Comperand) {
  return _InterlockedCompareExchange64_nf(Destination, Exchange, Comperand);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedCompareExchange64_nf(ptr{{.*}}%Destination, i64{{[a-z_ ]*}}%Exchange, i64{{[a-z_ ]*}}%Comperand){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = cmpxchg volatile ptr %Destination, i64 %Comperand, i64 %Exchange monotonic monotonic, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = extractvalue { i64, i1 } [[TMP]], 0
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

char test_InterlockedOr8_acq(char volatile *value, char mask) {
  return _InterlockedOr8_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedOr8_acq(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i8 %mask acquire, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedOr8_rel(char volatile *value, char mask) {
  return _InterlockedOr8_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedOr8_rel(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i8 %mask release, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedOr8_nf(char volatile *value, char mask) {
  return _InterlockedOr8_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedOr8_nf(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i8 %mask monotonic, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedOr16_acq(short volatile *value, short mask) {
  return _InterlockedOr16_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedOr16_acq(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i16 %mask acquire, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedOr16_rel(short volatile *value, short mask) {
  return _InterlockedOr16_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedOr16_rel(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i16 %mask release, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedOr16_nf(short volatile *value, short mask) {
  return _InterlockedOr16_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedOr16_nf(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i16 %mask monotonic, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedOr_acq(long volatile *value, long mask) {
  return _InterlockedOr_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedOr_acq(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i32 %mask acquire, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedOr_rel(long volatile *value, long mask) {
  return _InterlockedOr_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedOr_rel(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i32 %mask release, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedOr_nf(long volatile *value, long mask) {
  return _InterlockedOr_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedOr_nf(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i32 %mask monotonic, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedOr64_acq(__int64 volatile *value, __int64 mask) {
  return _InterlockedOr64_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedOr64_acq(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i64 %mask acquire, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedOr64_rel(__int64 volatile *value, __int64 mask) {
  return _InterlockedOr64_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedOr64_rel(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i64 %mask release, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedOr64_nf(__int64 volatile *value, __int64 mask) {
  return _InterlockedOr64_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedOr64_nf(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw or ptr %value, i64 %mask monotonic, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedXor8_acq(char volatile *value, char mask) {
  return _InterlockedXor8_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedXor8_acq(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i8 %mask acquire, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedXor8_rel(char volatile *value, char mask) {
  return _InterlockedXor8_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedXor8_rel(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i8 %mask release, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedXor8_nf(char volatile *value, char mask) {
  return _InterlockedXor8_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedXor8_nf(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i8 %mask monotonic, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedXor16_acq(short volatile *value, short mask) {
  return _InterlockedXor16_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedXor16_acq(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i16 %mask acquire, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedXor16_rel(short volatile *value, short mask) {
  return _InterlockedXor16_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedXor16_rel(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i16 %mask release, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedXor16_nf(short volatile *value, short mask) {
  return _InterlockedXor16_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedXor16_nf(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i16 %mask monotonic, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedXor_acq(long volatile *value, long mask) {
  return _InterlockedXor_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedXor_acq(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i32 %mask acquire, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedXor_rel(long volatile *value, long mask) {
  return _InterlockedXor_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedXor_rel(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i32 %mask release, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedXor_nf(long volatile *value, long mask) {
  return _InterlockedXor_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedXor_nf(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i32 %mask monotonic, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedXor64_acq(__int64 volatile *value, __int64 mask) {
  return _InterlockedXor64_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedXor64_acq(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i64 %mask acquire, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedXor64_rel(__int64 volatile *value, __int64 mask) {
  return _InterlockedXor64_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedXor64_rel(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i64 %mask release, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedXor64_nf(__int64 volatile *value, __int64 mask) {
  return _InterlockedXor64_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedXor64_nf(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw xor ptr %value, i64 %mask monotonic, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedAnd8_acq(char volatile *value, char mask) {
  return _InterlockedAnd8_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedAnd8_acq(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i8 %mask acquire, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedAnd8_rel(char volatile *value, char mask) {
  return _InterlockedAnd8_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedAnd8_rel(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i8 %mask release, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

char test_InterlockedAnd8_nf(char volatile *value, char mask) {
  return _InterlockedAnd8_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i8 @test_InterlockedAnd8_nf(ptr{{.*}}%value, i8{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i8 %mask monotonic, align 1
// CHECK-ARM-ARM64:   ret i8 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedAnd16_acq(short volatile *value, short mask) {
  return _InterlockedAnd16_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedAnd16_acq(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i16 %mask acquire, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedAnd16_rel(short volatile *value, short mask) {
  return _InterlockedAnd16_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedAnd16_rel(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i16 %mask release, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedAnd16_nf(short volatile *value, short mask) {
  return _InterlockedAnd16_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedAnd16_nf(ptr{{.*}}%value, i16{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i16 %mask monotonic, align 2
// CHECK-ARM-ARM64:   ret i16 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedAnd_acq(long volatile *value, long mask) {
  return _InterlockedAnd_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedAnd_acq(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i32 %mask acquire, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedAnd_rel(long volatile *value, long mask) {
  return _InterlockedAnd_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedAnd_rel(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i32 %mask release, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

long test_InterlockedAnd_nf(long volatile *value, long mask) {
  return _InterlockedAnd_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedAnd_nf(ptr{{.*}}%value, i32{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i32 %mask monotonic, align 4
// CHECK-ARM-ARM64:   ret i32 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedAnd64_acq(__int64 volatile *value, __int64 mask) {
  return _InterlockedAnd64_acq(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedAnd64_acq(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i64 %mask acquire, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedAnd64_rel(__int64 volatile *value, __int64 mask) {
  return _InterlockedAnd64_rel(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedAnd64_rel(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i64 %mask release, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedAnd64_nf(__int64 volatile *value, __int64 mask) {
  return _InterlockedAnd64_nf(value, mask);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedAnd64_nf(ptr{{.*}}%value, i64{{[a-z_ ]*}}%mask){{.*}}{
// CHECK-ARM-ARM64:   [[RESULT:%[0-9]+]] = atomicrmw and ptr %value, i64 %mask monotonic, align 8
// CHECK-ARM-ARM64:   ret i64 [[RESULT:%[0-9]+]]
// CHECK-ARM-ARM64: }

short test_InterlockedIncrement16_acq(short volatile *Addend) {
  return _InterlockedIncrement16_acq(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedIncrement16_acq(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i16 1 acquire, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i16 [[TMP]], 1
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedIncrement16_rel(short volatile *Addend) {
  return _InterlockedIncrement16_rel(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedIncrement16_rel(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i16 1 release, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i16 [[TMP]], 1
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedIncrement16_nf(short volatile *Addend) {
  return _InterlockedIncrement16_nf(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedIncrement16_nf(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i16 1 monotonic, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i16 [[TMP]], 1
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedIncrement_acq(long volatile *Addend) {
  return _InterlockedIncrement_acq(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedIncrement_acq(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i32 1 acquire, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i32 [[TMP]], 1
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedIncrement_rel(long volatile *Addend) {
  return _InterlockedIncrement_rel(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedIncrement_rel(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i32 1 release, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i32 [[TMP]], 1
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedIncrement_nf(long volatile *Addend) {
  return _InterlockedIncrement_nf(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedIncrement_nf(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i32 1 monotonic, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i32 [[TMP]], 1
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedIncrement64_acq(__int64 volatile *Addend) {
  return _InterlockedIncrement64_acq(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedIncrement64_acq(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i64 1 acquire, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i64 [[TMP]], 1
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedIncrement64_rel(__int64 volatile *Addend) {
  return _InterlockedIncrement64_rel(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedIncrement64_rel(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i64 1 release, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i64 [[TMP]], 1
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedIncrement64_nf(__int64 volatile *Addend) {
  return _InterlockedIncrement64_nf(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedIncrement64_nf(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw add ptr %Addend, i64 1 monotonic, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i64 [[TMP]], 1
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedDecrement16_acq(short volatile *Addend) {
  return _InterlockedDecrement16_acq(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedDecrement16_acq(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i16 1 acquire, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i16 [[TMP]], -1
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedDecrement16_rel(short volatile *Addend) {
  return _InterlockedDecrement16_rel(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedDecrement16_rel(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i16 1 release, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i16 [[TMP]], -1
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

short test_InterlockedDecrement16_nf(short volatile *Addend) {
  return _InterlockedDecrement16_nf(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i16 @test_InterlockedDecrement16_nf(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i16 1 monotonic, align 2
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i16 [[TMP]], -1
// CHECK-ARM-ARM64: ret i16 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedDecrement_acq(long volatile *Addend) {
  return _InterlockedDecrement_acq(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedDecrement_acq(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i32 1 acquire, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i32 [[TMP]], -1
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedDecrement_rel(long volatile *Addend) {
  return _InterlockedDecrement_rel(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedDecrement_rel(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i32 1 release, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i32 [[TMP]], -1
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

long test_InterlockedDecrement_nf(long volatile *Addend) {
  return _InterlockedDecrement_nf(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i32 @test_InterlockedDecrement_nf(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i32 1 monotonic, align 4
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i32 [[TMP]], -1
// CHECK-ARM-ARM64: ret i32 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedDecrement64_acq(__int64 volatile *Addend) {
  return _InterlockedDecrement64_acq(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedDecrement64_acq(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i64 1 acquire, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i64 [[TMP]], -1
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedDecrement64_rel(__int64 volatile *Addend) {
  return _InterlockedDecrement64_rel(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedDecrement64_rel(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i64 1 release, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i64 [[TMP]], -1
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }

__int64 test_InterlockedDecrement64_nf(__int64 volatile *Addend) {
  return _InterlockedDecrement64_nf(Addend);
}
// CHECK-ARM-ARM64: define{{.*}}i64 @test_InterlockedDecrement64_nf(ptr{{.*}}%Addend){{.*}}{
// CHECK-ARM-ARM64: [[TMP:%[0-9]+]] = atomicrmw sub ptr %Addend, i64 1 monotonic, align 8
// CHECK-ARM-ARM64: [[RESULT:%[0-9]+]] = add i64 [[TMP]], -1
// CHECK-ARM-ARM64: ret i64 [[RESULT]]
// CHECK-ARM-ARM64: }
#endif

void test__fastfail(void) {
  __fastfail(42);
}
// CHECK-LABEL: define{{.*}} void @test__fastfail()
// CHECK-ARM: call void asm sideeffect "udf #251", "{r0}"(i32 42) #[[NORETURN:[0-9]+]]
// CHECK-INTEL: call void asm sideeffect "int $$0x29", "{cx}"(i32 42) #[[NORETURN]]
// CHECK-ARM64: call void asm sideeffect "brk #0xF003", "{w0}"(i32 42) #[[NORETURN:[0-9]+]]

// Attributes come last.

// CHECK: attributes #[[NORETURN]] = { noreturn{{.*}} }
