/*
 * csp_autoconfig.h
 *
 * Created: 16.9.2014 23:18:51
 *  Author: Tomas Baca
 */ 


#ifndef CSP_AUTOCONFIG_H_
#define CSP_AUTOCONFIG_H_

#define GIT_REV "unknown"
#define CSP_FREERTOS	1 
#define FREERTOS_VERSION 6
#undef CSP_POSIX 
#undef CSP_WINDOWS
#undef CSP_MACOSX 
#undef CSP_DEBUG
#undef CSP_USE_RDP 
#define CSP_USE_CRC32	1
#undef CSP_USE_HMAC
#undef CSP_USE_XTEA
#undef CSP_USE_PROMISC 
#undef CSP_USE_QOS 
#define CSP_BUFFER_STATIC
#define CSP_CONN_MAX 10

// important
#define CSP_CONN_QUEUE_LENGTH 10

#define CSP_FIFO_INPUT 10
#define CSP_MAX_BIND_PORT 31
#define CSP_RDP_MAX_WINDOW 20
#define CSP_PADDING_BYTES 8
#define CSP_LITTLE_ENDIAN 1
#undef CSP_BIG_ENDIAN 
#define CSP_HAVE_STDBOOL_H 1

#endif /* CSP_AUTOCONFIG_H_ */