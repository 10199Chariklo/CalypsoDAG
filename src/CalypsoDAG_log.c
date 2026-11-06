/* _CK_ */

/* Standard C11 Libraries */
#include <stdio.h>

/* CalypsoDAG Subsystem */
#include "CalypsoDAG.h" // Public module header

/* Runtime logging configuration flag */
static CKlogLevel g_current_log_level = CK_LOG_NONE;

/* Runtime logging stream buffer */
static char g_log_buffer[1024];
static size_t g_log_offset = 0;

void CKdag_setLogLevel(CKlogLevel level) {
    g_current_log_level = level;
}

CKlogLevel CKdag_getLogLevel(void) {
    return g_current_log_level;
}

void CKdag_flushLog(CKlogFormat format) {
    // If structured output is requested, wrap or dump raw records
    if (format == CK_LOG_JSON) {
        // Placeholder for \n line serialization logic
    } else {
        printf("%s", g_log_buffer);
        printf("\n > CalypsoDAG: Log Stream Flushed.\n");
    }
    // Completely clear the static buffer footprint and reset tracking cursor
    for (size_t i = 0; i < sizeof(g_log_buffer); i++) {
        g_log_buffer[i] = '\0';
    }
    g_log_offset = 0;
}

void CKdag_bubbleLog(CKgraphNodeHandle node, const char* message) {
    // Only execute if logging is globally active
    if (g_current_log_level == CK_LOG_NONE) return;

    // Remaining capacity check
    if (g_log_offset >= sizeof(g_log_buffer) - 1) return;

    size_t capacity = sizeof(g_log_buffer) - g_log_offset;
    
    // Format the line out safely: Include node pointer hex for address tracing
    int written = snprintf(&g_log_buffer[g_log_offset], capacity, 
                           "[NODE: %p] -> %s\n", (void*)node, message);

    // Advance cursor if print was successful and fit within limits
    if (written > 0 && (size_t)written < capacity) {
        g_log_offset += written;
    }
}