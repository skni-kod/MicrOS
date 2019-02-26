#ifndef FENV_H
#define FENV_H

#include <stddef.h>

//! Invalid argument exception.
#define FE_INVALID 1
//! Denormal operand exception.
#define FE_DENORMAL 2
//! Pole error exception.
#define FE_DIVBYZERO 4
//! Overflow exception.
#define FE_OVERFLOW 8
//! Underflow exception.
#define FE_UNDERFLOW 16
//! Pole error exception.
#define FE_INEXACT 32
//! All exceptions.
#define FE_ALL_EXCEPT (FE_INVALID | FE_DENORMAL | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT)

//! Rounding to nearest.
#define FE_TONEAREST 0
//! Rounding downward.
#define FE_DOWNWARD 1
//! Rounding upward.
#define FE_UPWARD 2
//! Rounding towards zero.
#define FE_TOWARDZERO 3

//! Type represent state of floating point control word.
/*! Includes exception falgs and control modes to precision, rounding and infnity control. */
typedef struct fcontrol_t
{
    //! Invalid operation exception flag.
	unsigned char invalid_operation : 1;
    //! Denormalized operand exception flag.
	unsigned char denormalized_operand : 1;
    //! Zero divide exception flag.
	unsigned char zero_divide : 1;
    //! Overflow exception flag.
	unsigned char overflow : 1;
    //! Underflow exception flag.
	unsigned char underflow : 1;
    //! Precision exception flag.
	unsigned char precision : 1;
    //! Dummy field to align.
	unsigned char : 2;
    //! Precision control flag.
	unsigned char precision_control : 2;
    //! Rounding control flag.
	unsigned char rounding_control : 2;
    //! Infinity control flag.
	unsigned char infinity_control : 1;
    //! Dummy field to align.
	unsigned char : 3;
} fcontrol_t;

//! Type represent state of floating point status word.
/*! Includes exception flags and other informations associates with their status. */
typedef struct fexcept_t
{
    //! Invalid operation exception.
	unsigned char invalid_operation : 1;
    //! Denormalized operand exception.
	unsigned char denormalized_operand : 1;
    //! Zero divide exception.
	unsigned char zero_divide : 1;
    //! Overflow exception.
	unsigned char overflow : 1;
    //! Underflow exception.
	unsigned char underflow : 1;
    //! Precision exception.
	unsigned char precision : 1;
    //! Stack fault flag.
	unsigned char stack_fault : 1;
    //! Error summary status.
	unsigned char error_summary_status : 1;
    //! Condition code 0.
	unsigned char condition_code_0: 1;
    //! Condition code 1.
	unsigned char condition_code_1: 1;
    //! Condition code 2.
	unsigned char condition_code_2: 1;
    //! Points to top of stack pointer.
	unsigned char top_of_stack_pointer : 3;
    //! Condition code 3.
	unsigned char condition_code_3: 1;
    //! Indicates if FPU is busy.
	unsigned char busy : 1;
} fexcept_t;

//! Tag words.
/*! Describes what values are stored in FPU stack: valid; zero; invalid (NaN, unsupported), infinity or denormal; empty. */
typedef struct ftag_word_t
{
    //! Describes content of 1st register.
    unsigned char tag0 : 2;
    //! Describes content of 2nd register.
    unsigned char tag1 : 2;
    //! Describes content of 3rd register.
    unsigned char tag2 : 2;
    //! Describes content of 4th register.
    unsigned char tag3 : 2;
    //! Describes content of 5th register.
    unsigned char tag4 : 2;
    //! Describes content of 6th register.
    unsigned char tag5 : 2;
    //! Describes content of 7th register.
    unsigned char tag6 : 2;
    //! Describes content of 8th register.
    unsigned char tag7 : 2;
} ftag_word_t;

//! Type represent state of floating point environment.
/*! Includes exception falgs and control modes to precision, rounding and infnity control.
Also tag word, last used opcode, instructions and data pointer offset. */
typedef struct fenv_t
{
    //! FPU control word.
    fcontrol_t control_word;
    //! Unused
    unsigned short unused1;
    //! FPU status word.
    fexcept_t status_word;
    //! Unused
    unsigned short unused2;
    //! FPU tag word.
    ftag_word_t tag_word;
    //! Unused
    unsigned short unused3;
    //! Instruction pointer offset.
    unsigned int instruction_pointer_offset;
    //! Instruction pointer selector.
    unsigned short instruction_pointer_selector;
    //! Last used opcode.
    unsigned short opcode : 11;
    //! Five zeros.
    unsigned char five_zeros : 5;
    //! Data pointer offset.
    unsigned int data_pointer_offset;
    //! Data pointer selector.
    unsigned short data_pointer_selector;
    //! Unused
    unsigned short unused4;
} fenv_t;

//! Pointer to enviroment.
/*! Points to default enviroment. It's set during program startup. */
const fenv_t * env;

//! macro that provides access to pointer to enviroment.
/*! Points to default enviroment. It's set during program startup. */
#define FE_DFL_ENV (env)

#ifdef __cplusplus
extern "C" {
#endif

// Floating-point exceptions

//! Clear FPU exception flags.
/*!
    \param excepts Exception flags to clear.
*/
void feclearexcept(int excepts);

// Floating-point exceptions

//! Get exception flags
/*!
    \param flagp Pointer to structure where expection flags will be stored.
    \param excepts Defines which excepts will be checked.
*/
void fegetexceptflag(fexcept_t *flagp, int excepts);

#ifdef __cplusplus
}
#endif

// Additional content

//! Number value in tag word.
/*! Set when value at given FPU register is valid number. */
#define _FPU_TAG_WORD_VALID 0
//! Zero value in tag word.
/*! Set when value at given FPU register is zero. */
#define _FPU_TAG_ZERO 1
//! Sepcial value in tag word.
/*! Set when value at given FPU register is invalid (NaN, unsupported), infinity or denormal. */
#define _FPU_TAG_SPECIAL 2
//! Empty value in tag word.
/*! Set when given FPU register is empty. */
#define _FPU_TAG_EMPTY 3

//! Single precision. (32 bit)
#define _FPU_PRECISION_SINGLE 0
//! Double precision. (53 bit)
#define _FPU_PRECISION_DOUBLE 1
//! Extended precision. (64 bit)
#define _FPU_PRECISION_EXTENDED 3

//! Rounding to nearest.
#define _FPU_ROUNDING_NEAREST_EVEN FE_TONEAREST
//! Rounding downward.
#define _FPU_ROUNDING_FLOOR FE_DOWNWARD
//! Rounding upward.
#define _FPU_ROUNDING_CEIL FE_UPWARD
//! Rounding towards zero.
#define _FPU_ROUNDING_TRUNCATE FE_TOWARDZERO

//! Projective infinity control.
#define _FPU_INFINITY_CONTROL_PROJECTIVE 0
//! Affine infinity control.
#define _FPU_INFINITY_CONTROL_AFFINE 1

#ifdef __cplusplus
extern "C" {
#endif

//! Read FPU control word.
/*!
    \return Current FPU control word.
*/
fcontrol_t _FPU_read_control_word();

//! Write FPU control word.
/*!
	It also clears FPU exception before write value.
    \param control_word Control word to write into FPU.
*/
void _FPU_write_control_word(fcontrol_t control_word);

//! Read FPU enviroment.
/*!
    \return Current FPU enviroment.
*/
fenv_t _FPU_read_enviroment();

//! Write FPU enviroment.
/*!
    \param enviroment Enviroment to write into FPU.
*/
void _FPU_write_control(fenv_t enviroment);

//! Read FPU status word.
/*!
    \return Current FPU status word.
*/
fexcept_t _FPU_read_status_word();

//! Clear all FPU exceptions.
void _FPU_clear_exceptions();

#ifdef __cplusplus
}
#endif

#endif // FENV_H