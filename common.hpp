#ifndef COMMON_HPP
#define COMMON_HHP

#include <string>
#include <sys/types.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>

// Config:
extern std::string		cert_filename;
extern std::string		key_filename;
extern bool			transparent;
extern unsigned int		min_spare_children;	// Minimum number of children ready and waiting to accept()
extern unsigned int		max_children;		// Absolute maximum number of children, spare or not
extern unsigned int		max_handshake_time;	// TLS handshake must complete within this # of seconds
extern const char*		chroot_directory;	// NULL to not chroot
extern uid_t			drop_uid_network;	// UID for process that talks to network (-1 to not change)
extern gid_t			drop_gid_network;	// GID for process that talks to network (-1 to not change)
extern uid_t			drop_uid_keyserver;	// UID for key server process (-1 to not change)
extern gid_t			drop_gid_keyserver;	// GID for key server process (-1 to not change)

// State:
extern int			listening_sock;
extern int			children_pipe[2];		// Used by children to tell us when they accept a connection

// OpenSSL state:
extern SSL_CTX*			ssl_ctx;

inline void ssl_ctx_set_option (long option, bool state)
{
	if (state) {
		SSL_CTX_set_options(ssl_ctx, option);
	} else {
		SSL_CTX_clear_options(ssl_ctx, option);
	}
}

#endif
