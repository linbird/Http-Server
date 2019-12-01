#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

#include "https_server.h"
#include "my_config.h"

// unsigned short serverPort = COMMON_HTTPS_PORT;

CONF_INFO config;

void common_setup (void);

int main (int argc, char **argv)
{ 
    /*OpenSSL 初始化 */
    common_setup ();              

    if (argc > 1) {
        char *end_ptr;
        long lp = strtol(argv[1], &end_ptr, 0);
        if (*end_ptr) {
            fprintf(stderr, "Invalid integer\n");
            return -1;
        }
        if (lp <= 0) {
            fprintf(stderr, "Port must be positive\n");
            return -1;
        }
        if (lp >= USHRT_MAX) {
            fprintf(stderr, "Port must fit 16-bit range\n");
            return -1;
        }

        // serverPort = (unsigned short)lp;
    }
    
    /* now run http server (never returns) */
    return serve_some_https();
}

void common_setup (void){ 
    signal (SIGPIPE, SIG_IGN);

    // CRYPTO_set_mem_functions(calloc, realloc, free);
    SSL_library_init ();
    SSL_load_error_strings ();
    OpenSSL_add_all_algorithms ();

    printf ("Using OpenSSL version \"%s\"\nand libevent version \"%s\"\n",
            SSLeay_version (SSLEAY_VERSION),
            event_get_version ());
}