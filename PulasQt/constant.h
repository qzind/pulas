#ifndef CONSTANT
#define CONSTANT

namespace SERVER {
    const quint16 port      = 1963;
}

namespace MSG_TYPE {
    enum MSG_TYPE {
        MSG_UNKNOWN = 0,
        MSG_LIST_PRINT,
        MSG_SELECT_PRINT,
        MSG_GET_DEFAULT_PRINT,
        MSG_GET_CURRENT_PRINT,
        MSG_SETTING_PRINT,
        MSG_PRINT,
        MSG_GET_SUPPORTED_RESOLUTION,
        MSG_PRINT_PDF_FILE
    };
}

const int TIMEOUT       = 5 * 60 * 1000;

#endif // CONSTANT

