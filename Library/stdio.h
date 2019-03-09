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

    //! Function which loads chunk of the data from stream and stores it to the buffeer.
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

int fgetc(FILE *stream);
char *fgets(char *str, int num, FILE *stream);
int fputc(int character, FILE *stream);
int fputs(const char *str, FILE *stream);
int getc(FILE *stream);
int getchar();
char *gets(char *str);
int putc(int character, FILE *stream);
int putchar(int character);
int puts(const char *str);
int ungetc(int character, FILE *stream);

size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);

int fgetpos(FILE *stream, fpos_t *pos);
int fseek(FILE *stream, long int offset, int origin);
int fsetpos(FILE *stream, const fpos_t *pos);
long int ftell(FILE *stream);
void rewind(FILE *stream);

void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);

FILE *streams_create_stream();

#endif