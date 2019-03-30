#ifndef ERRNO_H
#define ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

//! Type or error.
typedef int errno_t;

//! Last error number.
/*! Value that inform about last error number occured during math operations. */
extern errno_t _errno;
//! Macro to access _errno.
/*! Provides access to _errno. */
#define errno _errno

//! Domain error.
/*! Set when function is called with parameters that are outside of its domain. */
#define EDOM        1
//! Range error.
/*! Set when value is too large to be stored in variable like pole error, overflow, underflow. */
#define ERANGE      2
//! Illegal sequence.
/*! Set when illegal byte sequence appears. */
#define EILSEQ      3

#ifdef __cplusplus
}
#endif

#endif // ERRNO_H