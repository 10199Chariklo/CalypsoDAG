/* _CK_ */

#ifndef CALYPSO_DAG_TYPEFWD_H
#define CALYPSO_DAG_TYPEFWD_H

// Logger flags for runtime configuration
typedef enum {
    CK_LOG_NONE         = 0X00,     // No logging 
    CK_LOG_LIFECYCLE    = 0X01,     // Tracks tree adjustments, attachments, and structural modifications
    CK_LOG_TRAVERSAL    = 0X02,     // Details node visitation, depth tracking and backtracking
    CK_LOG_ALL          = 0XFF      // Enables all logging flags for maximum verbosity
} CKlogLevel;

typedef enum {
    CK_LOG_CONSOLE, // Default; Console output stream (stdout EG line "> CalypsoDAG: [INFO] ")
    CK_LOG_JSON,    // JSON structured output for external parsing and analysis
} CKlogFormat;

// Order flags for linear flattening and visitors
typedef enum {
    CK_PRE_ORDER,   // Parent -> Left Sibling -> Right Sibling (Z-index/Rendering)
    CK_POST_ORDER,  // Left Sibling -> Right Sibling -> Parent (Teardown/Deallocation)
    CK_IN_ORDER     // Left Sibling -> Parent -> Right Sibling (Binary/Sorted Layouts)
} CKtraversalOrder;

typedef struct CKgraphNodeHeader    CKgraphNodeHeader; // Opaque Graph Header Structure
typedef struct CKgraphNodeHeader*   CKgraphNodeHandle; // Public type-agnostic Graph/Tree Node Handle

#endif // CALYPSO_DAG_TYPEFWD_H