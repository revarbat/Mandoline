
static char testletter = 'a';

const char *
synthesize_testfile_name(
    char *buf,
    size_t buflen,
    const char *arg0,
    const char *descr,
    const char *ext
) {
    // Get path from arg0 and get the basename, without any suffix.
    char *out = buf;
    const char *fullpath = arg0;
    const char *lastmatch = fullpath;
    for (; *fullpath; fullpath++) {
        if (*fullpath == '/') {
	    lastmatch = fullpath;
	}
    }
    if (*lastmatch == '/') {
        lastmatch++;
    }
    while (*lastmatch && *lastmatch != '.') {
        *out++ = *lastmatch++;
    }
    *out = '\0';

    char subtest[5];
    strcpy(subtest, "___");
    subtest[1] = testletter++;

    strncat(buf, subtest, buflen);
    strncat(buf, descr, buflen);
    strncat(buf, ".", buflen);
    strncat(buf, ext, buflen);

    return buf;
}


