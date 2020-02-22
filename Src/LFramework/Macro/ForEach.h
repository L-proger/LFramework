#ifndef ForEach_h__
#define ForEach_h__

#define UNPACK(...) __VA_ARGS__

//Count of commas == max args count (32)
#define CNT_PREFIX__CNT_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0 

#define COUNTER_SEQUENCE 32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#define ARGS_CNT2(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31, N, ...) N
#define ARGS_CNT1(__ARGS) ARGS_CNT2 __ARGS
#define ARGS_CNT(...) UNPACK(ARGS_CNT1((CNT_PREFIX_ ## __VA_ARGS__ ## _CNT_POSTFIX, COUNTER_SEQUENCE)))

#define CONCAT(arg1, arg2)   CONCAT1(arg1, arg2)
#define CONCAT1(arg1, arg2)  CONCAT2(arg1, arg2)
#define CONCAT2(arg1, arg2)  arg1##arg2

#define FOR_EACH_0(macro, runner, ...)
#define FOR_EACH_1(macro, runner, x) runner(macro, 0, LAST, x)
#define FOR_EACH_2(macro, runner, x, ...) runner(macro, 1, NOT_LAST, x) UNPACK(FOR_EACH_1(macro, runner, __VA_ARGS__))
#define FOR_EACH_3(macro, runner, x, ...) runner(macro, 2, NOT_LAST, x) UNPACK(FOR_EACH_2(macro, runner, __VA_ARGS__))
#define FOR_EACH_4(macro, runner, x, ...) runner(macro, 3, NOT_LAST, x) UNPACK(FOR_EACH_3(macro, runner, __VA_ARGS__))
#define FOR_EACH_5(macro, runner, x, ...) runner(macro, 4, NOT_LAST, x) UNPACK(FOR_EACH_4(macro, runner, __VA_ARGS__))
#define FOR_EACH_6(macro, runner, x, ...) runner(macro, 5, NOT_LAST, x) UNPACK(FOR_EACH_5(macro, runner, __VA_ARGS__))
#define FOR_EACH_7(macro, runner, x, ...) runner(macro, 6, NOT_LAST, x) UNPACK(FOR_EACH_6(macro, runner, __VA_ARGS__))
#define FOR_EACH_8(macro, runner, x, ...) runner(macro, 7, NOT_LAST, x) UNPACK(FOR_EACH_7(macro, runner, __VA_ARGS__))
#define FOR_EACH_9(macro, runner, x, ...) runner(macro, 8, NOT_LAST, x) UNPACK(FOR_EACH_8(macro, runner, __VA_ARGS__))
#define FOR_EACH_10(macro, runner, x, ...) runner(macro, 9, NOT_LAST, x) UNPACK(FOR_EACH_9(macro, runner, __VA_ARGS__))
#define FOR_EACH_11(macro, runner, x, ...) runner(macro, 10, NOT_LAST, x) UNPACK(FOR_EACH_10(macro, runner, __VA_ARGS__))
#define FOR_EACH_12(macro, runner, x, ...) runner(macro, 11, NOT_LAST, x) UNPACK(FOR_EACH_11(macro, runner, __VA_ARGS__))
#define FOR_EACH_13(macro, runner, x, ...) runner(macro, 12, NOT_LAST, x) UNPACK(FOR_EACH_12(macro, runner, __VA_ARGS__))
#define FOR_EACH_14(macro, runner, x, ...) runner(macro, 13, NOT_LAST, x) UNPACK(FOR_EACH_13(macro, runner, __VA_ARGS__))
#define FOR_EACH_15(macro, runner, x, ...) runner(macro, 14, NOT_LAST, x) UNPACK(FOR_EACH_14(macro, runner, __VA_ARGS__))
#define FOR_EACH_16(macro, runner, x, ...) runner(macro, 15, NOT_LAST, x) UNPACK(FOR_EACH_15(macro, runner, __VA_ARGS__))
#define FOR_EACH_17(macro, runner, x, ...) runner(macro, 16, NOT_LAST, x) UNPACK(FOR_EACH_16(macro, runner, __VA_ARGS__))
#define FOR_EACH_18(macro, runner, x, ...) runner(macro, 17, NOT_LAST, x) UNPACK(FOR_EACH_17(macro, runner, __VA_ARGS__))
#define FOR_EACH_19(macro, runner, x, ...) runner(macro, 18, NOT_LAST, x) UNPACK(FOR_EACH_18(macro, runner, __VA_ARGS__))
#define FOR_EACH_20(macro, runner, x, ...) runner(macro, 19, NOT_LAST, x) UNPACK(FOR_EACH_19(macro, runner, __VA_ARGS__))
#define FOR_EACH_21(macro, runner, x, ...) runner(macro, 20, NOT_LAST, x) UNPACK(FOR_EACH_20(macro, runner, __VA_ARGS__))
#define FOR_EACH_22(macro, runner, x, ...) runner(macro, 21, NOT_LAST, x) UNPACK(FOR_EACH_21(macro, runner, __VA_ARGS__))
#define FOR_EACH_23(macro, runner, x, ...) runner(macro, 22, NOT_LAST, x) UNPACK(FOR_EACH_22(macro, runner, __VA_ARGS__))
#define FOR_EACH_24(macro, runner, x, ...) runner(macro, 23, NOT_LAST, x) UNPACK(FOR_EACH_23(macro, runner, __VA_ARGS__))
#define FOR_EACH_25(macro, runner, x, ...) runner(macro, 24, NOT_LAST, x) UNPACK(FOR_EACH_24(macro, runner, __VA_ARGS__))
#define FOR_EACH_26(macro, runner, x, ...) runner(macro, 25, NOT_LAST, x) UNPACK(FOR_EACH_25(macro, runner, __VA_ARGS__))
#define FOR_EACH_27(macro, runner, x, ...) runner(macro, 26, NOT_LAST, x) UNPACK(FOR_EACH_26(macro, runner, __VA_ARGS__))
#define FOR_EACH_28(macro, runner, x, ...) runner(macro, 27, NOT_LAST, x) UNPACK(FOR_EACH_27(macro, runner, __VA_ARGS__))
#define FOR_EACH_29(macro, runner, x, ...) runner(macro, 28, NOT_LAST, x) UNPACK(FOR_EACH_28(macro, runner, __VA_ARGS__))
#define FOR_EACH_30(macro, runner, x, ...) runner(macro, 29, NOT_LAST, x) UNPACK(FOR_EACH_29(macro, runner, __VA_ARGS__))
#define FOR_EACH_31(macro, runner, x, ...) runner(macro, 30, NOT_LAST, x) UNPACK(FOR_EACH_30(macro, runner, __VA_ARGS__))
#define FOR_EACH_32(macro, runner, x, ...) runner(macro, 31, NOT_LAST, x) UNPACK(FOR_EACH_31(macro, runner, __VA_ARGS__))


#define FOREACH_SIMPLE_RUNNER(macro, id, isLast, x) macro(x)
#define FOREACH_INDEXED_RUNNER(macro, id, isLast, x) macro(id, x)

#define FOREACH_SIMPLE_RUNNER_ISLAST(macro, id, isLast, x) macro(isLast, x)

#define FOR_EACH1_R(macro, runner, N, ...) UNPACK(CONCAT(FOR_EACH_, N)(macro, runner, __VA_ARGS__))
#define FOR_EACH_R(macro, runner, ...) UNPACK(FOR_EACH1_R(macro, runner, ARGS_CNT(__VA_ARGS__), __VA_ARGS__))

#define FOR_EACH(macro, ...) FOR_EACH_R(macro, FOREACH_SIMPLE_RUNNER, __VA_ARGS__)

#define FOR_EACH_N(macro, ...) FOR_EACH_R(macro, FOREACH_INDEXED_RUNNER, __VA_ARGS__)
#define FOR_EACH_L(macro, ...) FOR_EACH_R(macro, FOREACH_SIMPLE_RUNNER_ISLAST, __VA_ARGS__)

static_assert(ARGS_CNT() == 0, "ARGS_CNT test fail");
static_assert(ARGS_CNT(a) == 1, "ARGS_CNT test fail");
static_assert(ARGS_CNT(a, a) == 2, "ARGS_CNT test fail");
static_assert(ARGS_CNT(a, a, a, a) == 4, "ARGS_CNT test fail");
static_assert(ARGS_CNT(a, a, a, a, a, a, a, a) == 8, "ARGS_CNT test fail");
static_assert(ARGS_CNT(a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a) == 16, "ARGS_CNT test fail");
static_assert(ARGS_CNT(a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a) == 32, "ARGS_CNT test fail");


#endif // ForEach_h__
