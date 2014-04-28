/*
 * Copyright (C) 2014 Andrew Ayer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <string.h>

struct System_error {
	std::string	syscall;
	std::string	target;
	int		number;

	System_error (const std::string& arg_syscall, const std::string& arg_target, int arg_number)
	: syscall(arg_syscall), target(arg_target), number(arg_number) { }
};
struct Openssl_error {
	unsigned long	code;
	explicit Openssl_error (unsigned long arg_code) : code(arg_code) { }

	std::string	message () const { return message(code); }

	static std::string message (unsigned long code)
	{
		char buf[120];
		ERR_error_string_n(code, buf, sizeof(buf));
		return buf;
	}
};
struct Configuration_error {
	std::string	message;
	explicit Configuration_error (const std::string& arg_message) : message(arg_message) { }
};
struct Key_protocol_error {
	std::string	message;
	explicit Key_protocol_error (const std::string& arg_message) : message(arg_message) { }
};

void set_nonblocking (int fd, bool nonblocking);
void set_transparent (int sock_fd);
void set_not_v6only (int sock_fd);

void drop_privileges (const std::string& chroot_directory, uid_t drop_uid, gid_t drop_gid);

void write_all (int fd, const void* data, size_t len);
bool read_all (int fd, void* data, size_t len);

void resolve_address (struct sockaddr_in6* address, const std::string& host, const std::string& port);
uid_t resolve_user (const std::string&);
gid_t resolve_group (const std::string&);

void daemonize ();

inline bool parse_config_bool (const char* str)
{
	if (strcasecmp(str, "yes") == 0 ||
			strcasecmp(str, "true") == 0 ||
			strcasecmp(str, "on") == 0 ||
			(std::isdigit(str[0]) && std::atoi(str) > 0)) {
		return true;
	} else if (strcasecmp(str, "no") == 0 ||
			strcasecmp(str, "false") == 0 ||
			strcasecmp(str, "off") == 0 ||
			(std::isdigit(str[0]) && std::atoi(str) == 0)) {
		return false;
	} 
	throw Configuration_error(std::string("Invalid yes/no value `") + str + "'");
}
inline bool parse_config_bool (const std::string& str) { return parse_config_bool(str.c_str()); }

enum Transparency {
	TRANSPARENT_OFF = 0,
	TRANSPARENT_ON = 1,
	TRANSPARENT_BACKEND_ONLY = 2
};

inline Transparency parse_config_transparency (const char* str)
{
	if (strcasecmp(str, "backend-only") == 0) {
		return TRANSPARENT_BACKEND_ONLY;
	} else {
		return parse_config_bool(str) ? TRANSPARENT_ON : TRANSPARENT_OFF;
	}
}
inline Transparency parse_config_transparency (const std::string& str) { return parse_config_transparency(str.c_str()); }


#endif
