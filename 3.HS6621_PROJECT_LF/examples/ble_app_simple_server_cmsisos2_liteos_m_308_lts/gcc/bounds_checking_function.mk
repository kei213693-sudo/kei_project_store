BND_CHK_TOPDIR := third_party/bounds_checking_function

SOURCES  += $(BND_CHK_TOPDIR)/src/fscanf_s.c \
            $(BND_CHK_TOPDIR)/src/memmove_s.c \
            $(BND_CHK_TOPDIR)/src/securecutil.c \
            $(BND_CHK_TOPDIR)/src/strcat_s.c \
            $(BND_CHK_TOPDIR)/src/swprintf_s.c \
            $(BND_CHK_TOPDIR)/src/vsnprintf_s.c \
            $(BND_CHK_TOPDIR)/src/vwscanf_s.c \
            $(BND_CHK_TOPDIR)/src/wcstok_s.c \
            $(BND_CHK_TOPDIR)/src/fwscanf_s.c \
            $(BND_CHK_TOPDIR)/src/memset_s.c \
            $(BND_CHK_TOPDIR)/src/secureprintoutput_w.c \
            $(BND_CHK_TOPDIR)/src/strcpy_s.c \
            $(BND_CHK_TOPDIR)/src/swscanf_s.c \
            $(BND_CHK_TOPDIR)/src/vsprintf_s.c \
            $(BND_CHK_TOPDIR)/src/wcscat_s.c \
            $(BND_CHK_TOPDIR)/src/wmemcpy_s.c \
            $(BND_CHK_TOPDIR)/src/gets_s.c \
            $(BND_CHK_TOPDIR)/src/secureinput_a.c \
            $(BND_CHK_TOPDIR)/src/snprintf_s.c \
            $(BND_CHK_TOPDIR)/src/strncat_s.c \
            $(BND_CHK_TOPDIR)/src/vfscanf_s.c \
            $(BND_CHK_TOPDIR)/src/vsscanf_s.c \
            $(BND_CHK_TOPDIR)/src/wcscpy_s.c \
            $(BND_CHK_TOPDIR)/src/wmemmove_s.c \
            $(BND_CHK_TOPDIR)/src/scanf_s.c \
            $(BND_CHK_TOPDIR)/src/secureinput_w.c \
            $(BND_CHK_TOPDIR)/src/sprintf_s.c \
            $(BND_CHK_TOPDIR)/src/strncpy_s.c \
            $(BND_CHK_TOPDIR)/src/vfwscanf_s.c \
            $(BND_CHK_TOPDIR)/src/vswprintf_s.c \
            $(BND_CHK_TOPDIR)/src/wcsncat_s.c \
            $(BND_CHK_TOPDIR)/src/wscanf_s.c \
            $(BND_CHK_TOPDIR)/src/memcpy_s.c \
            $(BND_CHK_TOPDIR)/src/secureprintoutput_a.c \
            $(BND_CHK_TOPDIR)/src/sscanf_s.c \
            $(BND_CHK_TOPDIR)/src/strtok_s.c \
            $(BND_CHK_TOPDIR)/src/vscanf_s.c \
            $(BND_CHK_TOPDIR)/src/vswscanf_s.c \
            $(BND_CHK_TOPDIR)/src/wcsncpy_s.c

INCLUDES  +=    -I../../../third_party/bounds_checking_function/include

