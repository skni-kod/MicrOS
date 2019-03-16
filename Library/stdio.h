#ifndef STDIO_H
#define STDIO_H

//! Size of the stream buffer
#define BUFSIZ 1024 * 32

//! Value returned when internal stream position has reached the end of file
#define EOF -1

//! Maximum length of file names
#define FILENAME_MAX 255

//! Potential limit of simultaneous open streams
#define FOPEN_MAX INT32_MAX

//! Minimum length for temporary file name
#define L_tmpnam 0

//! Maximum number of temporary files
#define TMP_MAX 0

//! Seek-Set mode (position is absolute)
#define SEEK_SET 0

//! Seek-Current mode (relative to the current position)
#define SEEK_CUR 1

//! Seek-End mode (relative to the end of the file)
#define SEEK_END 2

//! No Buffering mode
#define _IONBF 0

//! Line Buffering mode
#define _IOLBF 1

//! Full Buffering mode
#define _IOFBF 2

#include <stdint.h>
#include <stdarg.h>
#include "stdlib.h"

//! Unsigned integral type.
typedef uint32_t size_t;

//! Object containing information to specify a position within a file.
typedef uint32_t fpos_t;

//! Stream buffering modest.
typedef enum file_buffering_mode
{
    //! Flush when new data is present (don't store it in buffer).
    file_buffering_mode_none = 0,

    //! Flush if buffer is full, new line char is present or fflush has been called.
    file_buffering_mode_line = 1,

    //! Flush if buffer is full or fflush has been called.
    file_buffering_mode_full = 2
} file_buffering_mode;

//! Object containing information to control a stream.
/*! Object type that identifies a stream and contains the information needed to control it, including a pointer to its buffer, its position indicator and all its state indicators. !*/
typedef struct file
{
    //! Full path of the file (can be empty if stream is assigned to some hardware device).
    char filename[FILENAME_MAX];

    //! Stream buffer.
    char *buffer;

    //! Position of the buffer start in the stream.
    uint32_t base;

    //! Current position in the stream.
    uint32_t pos;

    //! Position of the buffer end in the stream.
    uint32_t size;

    //! Total size of the stream (can be 0 if it's undefined like console or keyboard).
    uint32_t limit;

    //! Error code.
    uint32_t error;

    //! Buffering mode of the stream.
    file_buffering_mode buffering_mode;

    //! Function which loads chunk of the data from stream and stores it to the buffer.
    int (*fetch)(struct file *file);

    //! Function which gets data from the buffer and sends it to the output (and removes it from the buffer).
    void (*flush)(struct file *file);
} FILE;

//! Standard input (default is keyboard).
FILE *stdin;

//! Standard output (default is console).
FILE *stdout;

//! Standard error output (default is console).
FILE *stderr;

//! Open file.
/*!
    Opens the file whose name is specified in the parameter filename and associates it with a stream that can be identified in future operations by the FILE pointer returned. The returned stream is fully buffered by default. 
    \param filename C string containing the name of the file to be opened.
    \param mode C string containing a file access mode (r, w, a, r+, w+, a+).
    \return If the file is successfully opened, the function returns a pointer to a FILE object that can be used to identify the stream on future operations. Otherwise, a null pointer is returned.
*/
FILE *fopen(const char *filename, const char *mode);

//! Reopen stream with different file or mode.
/*!
    Reuses stream to either open the file specified by filename or to change its access mode. If filename is a null pointer, the function attempts to change the mode of the stream.
    \param filename C string containing the name of the file to be opened.
    \param mode C string containing a file access mode (r, w, a, r+, w+, a+).
    \param stream Pointer to a FILE object that identifies the stream to be reopened.
    \return If the file is successfully reopened, the function returns the pointer passed as parameter stream, which can be used to identify the reopened stream. Otherwise, a null pointer is returned.
*/
FILE *freopen(const char *filename, const char *mode, FILE *stream);

//! Close file.
/*!
    Closes the file associated with the stream and disassociates it.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return If the stream is successfully closed, a zero value is returned. On failure, EOF is returned.
*/
int fclose(FILE *stream);

//! Flush stream.
/*!
    If the given stream was open for writing any unwritten data in its output buffer is written to the file. If stream is a null pointer, all such streams are flushed.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return A zero value indicates success. If an error occurs, EOF is returned and the error indicator is set.
*/
int fflush(FILE *stream);

//! Set stream buffer.
/*!
    Specifies the buffer to be used by the stream for I/O operations, which becomes a fully buffered stream. Or, alternatively, if buffer is a null pointer, buffering is disabled for the stream, which becomes an unbuffered stream.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \param buffer User allocated buffer. Shall be at least BUFSIZ bytes long.
*/
void setbuf(FILE *stream, char *buffer);

//! Change stream buffering.
/*!
    Specifies a buffer for stream. The function allows to specify the mode and size of the buffer (in bytes). If buffer is a null pointer, the function automatically allocates a buffer (using size as a hint on the size to use).
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \param buffer User allocated buffer. Shall be at least BUFSIZ bytes long.
    \param mode Specifies a mode for file buffering (full buffering: _IOFBF, line buffering: _IOLBF, no buffering: _IONBF).
    \param size User allocated buffer. Shall be at least BUFSIZ bytes long.
    \return If the buffer is correctly assigned to the file, a zero value is returned. Otherwise, a non-zero value is returned.
*/
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);

//! Get character from stream.
/*!
    Returns the character currently pointed by the internal file position indicator of the specified stream. The internal file position indicator is then advanced to the next character.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, the character read is returned. If the position indicator was at the end-of-file, the function returns EOF.
*/
int fgetc(FILE *stream);

//! Get string from stream.
/*!
    Reads characters from stream and stores them as a C string into str until (num-1) characters have been read or either a newline or the end-of-file is reached, whichever happens first.
    \param str Pointer to an array of chars where the string read is copied.
    \param num Maximum number of characters to be copied into str (including the terminating null-character).
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, the function returns str. If the end-of-file is encountered while attempting to read a character, the eof indicator is set. If this happens before any characters could be read, the pointer returned is a null pointer.
*/
char *fgets(char *str, int num, FILE *stream);

//! Write character to stream.
/*!
    Writes a character to the stream and advances the position indicator.
    \param character The int promotion of the character to be written.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, the character written is returned. If a writing error occurs, EOF is returned and the error indicator (ferror) is set.
*/
int fputc(int character, FILE *stream);

//! Write string to stream.
/*!
    Writes the C string pointed by str to the stream. The function begins copying from the address specified (str) until it reaches the terminating null character ('\0'). This terminating null-character is not copied to the stream.
    \param str C string with the content to be written to stream.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, a non-negative value is returned. On error, the function returns EOF and sets the error indicator.
*/
int fputs(const char *str, FILE *stream);

//! Get character from stream.
/*!
    Returns the character currently pointed by the internal file position indicator of the specified stream. The internal file position indicator is then advanced to the next character.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, the character read is returned. If the position indicator was at the end-of-file, the function returns EOF.
*/
int getc(FILE *stream);

//! Get character from stdin.
/*!
    Returns the next character from the standard input.
    \return On success, the character read is returned. If the position indicator was at the end-of-file, the function returns EOF.
*/
int getchar();

//! Get string from stdin.
/*!
    Reads characters from the standard input (stdin) and stores them as a C string into str until a newline character or the end-of-file is reached.
    \param str Pointer to a block of memory (array of char) where the string read is copied as a C string.
    \return On success, the character read is returned. If the position indicator was at the end-of-file, the function returns EOF.
*/
char *gets(char *str);

//! Write character to stream.
/*!
    Writes a character to the stream and advances the position indicator. The character is written at the position indicated by the internal position indicator of the stream, which is then automatically advanced by one.
    \param character The int promotion of the character to be written.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, the character written is returned. If a writing error occurs, EOF is returned and the error indicator is set.
*/
int putc(int character, FILE *stream);

//! Write character to stdout.
/*!
    Writes a character to the standard output (stdout).
    \param character The int promotion of the character to be written.
    \return On success, the character written is returned. If a writing error occurs, EOF is returned and the error indicator is set.
*/
int putchar(int character);

//! Write string to stdout.
/*!
    Writes the C string pointed by str to the standard output (stdout) and appends a newline character ('\n'). The function begins copying from the address specified (str) until it reaches the terminating null character ('\0'). This terminating null-character is not copied to the stream.
    \param str C string to be printed.
    \return On success, the character written is returned. If a writing error occurs, EOF is returned and the error indicator is set.
*/
int puts(const char *str);

//! Unget character from stream.
/*!
    A character is virtually put back into an input stream, decreasing its internal file position as if a previous getc operation was undone.
    \param character The int promotion of the character to be written.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, the character put back is returned. If the operation fails, EOF is returned.
*/
int ungetc(int character, FILE *stream);

//! Read block of data from stream.
/*!
    Reads an array of count elements, each one with a size of size bytes, from the stream and stores them in the block of memory specified by ptr. The position indicator of the stream is advanced by the total amount of bytes read.
    \param ptr Pointer to a block of memory with a size of at least (size*count) bytes, converted to a void*.
    \param size Size, in bytes, of each element to be read.
    \param count Number of elements, each one with a size of size bytes.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return The total number of elements successfully read is returned. If this number differs from the count parameter, either a reading error occurred or the end-of-file was reached while reading. In both cases, the proper indicator is set.
*/
size_t fread(void *ptr, size_t size, size_t count, FILE *stream);

//! Write block of data to stream.
/*!
    Writes an array of count elements, each one with a size of size bytes, from the block of memory pointed by ptr to the current position in the stream. The position indicator of the stream is advanced by the total number of bytes written.
    \param ptr Pointer to a block of memory with a size of at least (size*count) bytes, converted to a void*.
    \param size Size, in bytes, of each element to be read.
    \param count Number of elements, each one with a size of size bytes.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return The total number of elements successfully written is returned. If this number differs from the count parameter, a writing error prevented the function from completing. In this case, the error indicator will be set for the stream.
*/
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);

//! Get current position in stream.
/*!
    Retrieves the current position in the stream.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \param pos Pointer to a fpos_t object. This should point to an object already allocated.
    \return On success, the function returns zero. In case of error, errno is set to a platform-specific positive value and the function returns a non-zero value.
*/
int fgetpos(FILE *stream, fpos_t *pos);

//! Reposition stream position indicator.
/*!
    Sets the position indicator associated with the stream to a new position.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \param offset Number of bytes to offset from origin.
    \param origin Position used as reference for the offset. It is specified by one of the constants (file begin: SEEK_SET, current position: SEEK_CUR, end of file: SEEK_END).
    \return If successful, the function returns zero. Otherwise, it returns non-zero value and error indicator is set.
*/
int fseek(FILE *stream, long int offset, int origin);

//! Set position indicator of stream.
/*!
    Restores the current position in the stream to pos. 
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \param pos Pointer to a fpos_t object containing a position previously obtained with fgetpos.
    \return If successful, the function returns zero. On failure, a non-zero value is returned and errno is set to a system-specific positive value.
*/
int fsetpos(FILE *stream, const fpos_t *pos);

//! Get current position in stream.
/*!
    Returns the current value of the position indicator of the stream.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return On success, the current value of the position indicator is returned. On failure, -1 is returned, and errno is set to a system-specific positive value.
*/
long int ftell(FILE *stream);

//! Set position of stream to the beginning.
/*!
    Sets the position indicator associated with stream to the beginning of the file.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
*/
void rewind(FILE *stream);

//! Clear error indicators.
/*!
    Resets both the error and the eof indicators of the stream.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
*/
void clearerr(FILE *stream);

//! Check end-of-file indicator.
/*!
    Checks whether the end-of-File indicator associated with stream is set, returning a value different from zero if it is.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return A non-zero value is returned in the case that the end-of-file indicator associated with the stream is set. Otherwise, zero is returned.
*/
int feof(FILE *stream);

//! Check error indicator.
/*!
    Checks if the error indicator associated with stream is set, returning a value different from zero if it is.
    \param stream Pointer to a FILE object that identifies the stream to be closed.
    \return A non-zero value is returned in the case that the error indicator associated with the stream is set. Otherwise, zero is returned.
*/
int ferror(FILE *stream);

int sprintf(char* str, const char* format, ...);

int printf(const char* format, ...);

int fprintf(FILE* file, const char* format, ...);

int __vfprintf(FILE* stream, const char* format, va_list arg);

//! Creates new stream.
/*!
    Creates new stream (not assigned to any file or device) and returns it.
    \return A newly created stream.
*/
FILE *__stdio_create_stream();

#endif
