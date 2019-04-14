#ifndef FENV_H
#define FENV_H

#include <stddef.h>

//! Invalid argument exception.
#define FE_INVALID      1
//! Denormal operand exception.
#define FE_DENORMAL     2
//! Pole error exception.
#define FE_DIVBYZERO    4
//! Overflow exception.
#define FE_OVERFLOW     8
//! Underflow exception.
#define FE_UNDERFLOW    16
//! Pole error exception.
#define FE_INEXACT      32
//! All exceptions.
#define FE_ALL_EXCEPT   (FE_INVALID | FE_DENORMAL | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT)

//! Rounding to nearest.
#define FE_TONEAREST    0
//! Rounding downward.
#define FE_DOWNWARD     1
//! Rounding upward.
#define FE_UPWARD       2
//! Rounding towards zero.
#define FE_TOWARDZERO   3

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

//! Pointer to environment.
/*! Points to default environment. It's set during program startup. */
extern const fenv_t * env;

//! macro that provides access to pointer to environment.
/*! Points to default environment. It's set during program startup. */
#define FE_DFL_ENV (env)

#ifdef __cplusplus
extern "C" {
#endif

// Floating-point exceptions

//! Clear FPU exception flags.
/*!
    Function attempts to clear the supported floating-point exceptions represented by its argument.
    \param excepts Exception flags to clear.
    \return Function returns zero if the excepts argument is zero or if all the specified exceptions were successfully cleared. Otherwise, it returns a nonzero value.
*/
int feclearexcept(int excepts);

//! Get exception flags
/*!
    Function attempts to store an implementation-defined representation of the states of the floating-point status flags indicated by the argument excepts in the object pointed to by the argument flagp.
    \param flagp Pointer to structure where expection flags will be stored.
    \param excepts Defines which excepts will be checked.
    \return Function returns zero if the representation was successfully stored. Otherwise, it returns a nonzero value.
*/
int fegetexceptflag(fexcept_t *flagp, int excepts);

//! Raise FPU exception flags.
/*!
    Function attempts to raise the supported floating-point exceptions represented by its argument.
    \param excepts Exception flags to raise.
    \return Function returns zero if the excepts argument is zero or if all the specified exceptions were successfully raised. Otherwise, it returns a nonzero value.
*/
int feraiseexcept(int excepts);

//! Set exception flags.
/*!
    Function attempts to set the floating-point status flags indicated by the argument excepts to the states stored in the object pointed to by flagp. The value of *flagp shall have been set by a previous call to fegetexceptflag whose second argument represented at least those floating-point exceptions represented by the argument excepts. This function does not raise floatingpoint exceptions, but only sets the state of the flags.
    \param flagp Pointer to structure from where we will read expection.
    \param excepts Defines which excepts will be set.
    \return Function returns zero if the excepts argument is zero or if all the specified flags were successfully set to the appropriate state. Otherwise, it returns a nonzero value.
*/
int fesetexceptflag(const fexcept_t *flagp, int excepts);

//! Test for floating-point exceptions.
/*!
    Function determines which of a specified subset of the floatingpoint exception flags are currently set. The excepts argument specifies the floatingpoint status flags to be queried.
    \param excepts Exception flags to check.
    \return Function returns the value of the bitwise OR of the floating-point exception macros corresponding to the currently set floating-point exceptions included in excepts.
*/
int fetestexcept(int excepts);

// Rounding direction

//! Get current rounding direction.
/*!
    \return Function returns the value of the rounding direction macro representing the current rounding direction or a negative value if there is no such rounding direction macro or the current rounding direction is not determinable.
*/
int fegetround(void);

//! Set rounding direction.
/*!
    Function establishes the rounding direction represented by its argument round. If the argument is not equal to the value of a rounding direction macro, the rounding direction is not changed.
    \param round Rounding direction to set.
    \return Function returns zero if and only if the requested rounding direction was established.
*/
int fesetround(int round);

// Entire environment

//! Get entire environment.
/*!
    Function attempts to store the current floating-point environment in the object pointed to by envp/
    \param envp Pointer to struct where environment will be stored.
    \return Function returns zero if the environment was successfully stored. Otherwise, it returns a nonzero value.
*/
int fegetenv(fenv_t *envp);

//! Set entire environment.
/*!
    The fesetenv function attempts to establish the floating-point environment represented by the object pointed to by envp. The argument envp shall point to an object set by a call to fegetenv or feholdexcept, or equal a floating-point environment macro. Note that fesetenv merely installs the state of the floating-point status flags represented through its argument, and does not raise these floating-point exceptions.
    \param envp Pointer to struct from where environment will read environment.
    \return Function returns zero if the environment was successfully wrote. Otherwise, it returns a nonzero value.
*/
int fesetenv(const fenv_t *envp);

//! Hold floating-point exceptions.
/*!
    Function saves the current floating-point environment in the object pointed to by envp, clears the floating-point status flags, and then installs a non-stop (continue on floating-point exceptions) mode, if available, for all floating-point exceptions.
    \param envp Pointer to struct where environment will be saved.
    \return Function returns zero if and only if non-stop floating-point exception handling was successfully installed.
*/
int feholdexcept(fenv_t *envp);

//! Update floating-point environment.
/*!
    function attempts to save the currently raised floating-point exceptions in its automatic storage, install the floating-point environment represented by the object pointed to by envp, and then raise the saved floating-point exceptions. The argument envp shall point to an object set by a call to feholdexcept or fegetenv, or equal a floating-point environment macro.
    \param envp Pointer to struct from where environment will read environment.
    \return Function returns zero if all the actions were successfully carried out. Otherwise, it returns a nonzero value.
*/
int feupdateenv(const fenv_t *envp);

#ifdef __cplusplus
}
#endif

// Additional content

//! Number value in tag word.
/*! Set when value at given FPU register is valid number. */
#define __FPU_TAG_WORD_VALID        0
//! Zero value in tag word.
/*! Set when value at given FPU register is zero. */
#define __FPU_TAG_ZERO              1
//! Sepcial value in tag word.
/*! Set when value at given FPU register is invalid (NaN, unsupported), infinity or denormal. */
#define __FPU_TAG_SPECIAL           2
//! Empty value in tag word.
/*! Set when given FPU register is empty. */
#define __FPU_TAG_EMPTY             3

//! Single precision. (32 bit)
#define __FPU_PRECISION_SINGLE      0
//! Double precision. (53 bit)
#define __FPU_PRECISION_DOUBLE      1
//! Extended precision. (64 bit)
#define __FPU_PRECISION_EXTENDED    3

//! Rounding to nearest.
#define __FPU_ROUNDING_NEAREST_EVEN     0
//! Rounding downward.
#define __FPU_ROUNDING_FLOOR            1
//! Rounding upward.
#define __FPU_ROUNDING_CEIL             2
//! Rounding towards zero.
#define __FPU_ROUNDING_TRUNCATE         3

//! Projective infinity control.
#define __FPU_INFINITY_CONTROL_PROJECTIVE   0
//! Affine infinity control.
#define __FPU_INFINITY_CONTROL_AFFINE       1

#ifdef __cplusplus
extern "C" {
#endif

//! Read FPU control word.
/*!
    \return Current FPU control word.
*/
fcontrol_t __FPU_read_control_word();

//! Write FPU control word.
/*!
	It also clears FPU exception before write value.
    \param control_word Control word to write into FPU.
*/
void __FPU_write_control_word(fcontrol_t control_word);

//! Read FPU environment.
/*!
    \return Current FPU environment.
*/
fenv_t __FPU_read_environment();

//! Write FPU environment.
/*!
    \param environment environment to write into FPU.
*/
void __FPU_write_environment(fenv_t environment);

//! Read FPU status word.
/*!
    \return Current FPU status word.
*/
fexcept_t __FPU_read_status_word();

//! Clear all FPU exceptions.
void __FPU_clear_exceptions();

#ifdef __cplusplus
}
#endif

#endif // FENV_H