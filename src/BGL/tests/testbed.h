#include <libgen.h>

static char testletter = 'a';

const char *
synthesize_testfile_name(
    char *buf,
    size_t buflen,
    char *arg0,
    const char *descr,
    const char *ext
) {
    // Get path from arg0 and get the basename, without any suffix.
    char *out = buf;
    const char *base = basename(arg0);

    while (*base == '/') base++;

    char subtest[5];
    strcpy(subtest, "___");
    subtest[1] = testletter++;

    strncpy(buf, "result_svgs/", buflen);

    strncat(buf, base, buflen);
    for (out = buf; *out && *out != '.'; out++);
    *out = '\0';

    strncat(buf, subtest, buflen);
    strncat(buf, descr, buflen);
    strncat(buf, ".", buflen);
    strncat(buf, ext, buflen);

    return buf;
}


