#pragma once

#ifndef REFL_REPEAT
#define REFL_REPEAT

#define PASTE(x, y) CONCATE(x, y)
#define CONCATE(x, y) x ## y

#define LOOP_0(func, i, args)
#define LOOP_1(func, i, args) func(i, args)
#define LOOP_2(func, i, args, ...) func(i, args) LOOP_1 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_3(func, i, args, ...) func(i, args) LOOP_2 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_4(func, i, args, ...) func(i, args) LOOP_3 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_5(func, i, args, ...) func(i, args) LOOP_4 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_6(func, i, args, ...) func(i, args) LOOP_5 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_7(func, i, args, ...) func(i, args) LOOP_6 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_8(func, i, args, ...) func(i, args) LOOP_7 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_9(func, i, args, ...) func(i, args) LOOP_8 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_10(func, i, args, ...) func(i, args) LOOP_9 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_11(func, i, args, ...) func(i, args) LOOP_10 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_12(func, i, args, ...) func(i, args) LOOP_11 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_13(func, i, args, ...) func(i, args) LOOP_12 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_14(func, i, args, ...) func(i, args) LOOP_13 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_15(func, i, args, ...) func(i, args) LOOP_14 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_16(func, i, args, ...) func(i, args) LOOP_15 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_17(func, i, args, ...) func(i, args) LOOP_16 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_18(func, i, args, ...) func(i, args) LOOP_17 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_19(func, i, args, ...) func(i, args) LOOP_18 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_20(func, i, args, ...) func(i, args) LOOP_19 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_21(func, i, args, ...) func(i, args) LOOP_20 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_22(func, i, args, ...) func(i, args) LOOP_21 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_23(func, i, args, ...) func(i, args) LOOP_22 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_24(func, i, args, ...) func(i, args) LOOP_23 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_25(func, i, args, ...) func(i, args) LOOP_24 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_26(func, i, args, ...) func(i, args) LOOP_25 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_27(func, i, args, ...) func(i, args) LOOP_26 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_28(func, i, args, ...) func(i, args) LOOP_27 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_29(func, i, args, ...) func(i, args) LOOP_28 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_30(func, i, args, ...) func(i, args) LOOP_29 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_31(func, i, args, ...) func(i, args) LOOP_30 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_32(func, i, args, ...) func(i, args) LOOP_31 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_33(func, i, args, ...) func(i, args) LOOP_32 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_34(func, i, args, ...) func(i, args) LOOP_33 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_35(func, i, args, ...) func(i, args) LOOP_34 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_36(func, i, args, ...) func(i, args) LOOP_35 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_37(func, i, args, ...) func(i, args) LOOP_36 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_38(func, i, args, ...) func(i, args) LOOP_37 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_39(func, i, args, ...) func(i, args) LOOP_38 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R
#define LOOP_40(func, i, args, ...) func(i, args) LOOP_39 BRACKET_L func, i + 1, __VA_ARGS__ BRACKET_R


#endif // !REFL_REPEAT
