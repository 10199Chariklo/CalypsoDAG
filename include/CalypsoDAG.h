/* _CK_ */

#ifndef CALYPSO_DAG_H
#define CALYPSO_DAG_H

/* Standard C11 Libraries */
#include <stdbool.h>

/* CalypsoDAG Subsystem */
#include "CalypsoDAG_typefwd.h" // Public type definitions

/**
 * @brief Attaches a node handle as the last child of a parent node handle.
*/
void CKdag_AppendChild(CKgraphNodeHandle parent, CKgraphNodeHandle child);

/**
 * @brief Sub-Tree Lifecycle Detachment Primitive.
 * Unlinks the node from its parent and adjacent siblings safely.
*/
void CKdag_DetachSubTree(CKgraphNodeHandle node);

/**
 * @brief Type-agnostic visitor callback type matching the updated handle interface.
*/
typedef bool (*CKgraphVisitor)(CKgraphNodeHandle node, void* context);

/**
 * @brief Master execution driver for flattening passes and system layout traversal hooks.
*/
bool CKdag_Flatten(CKgraphNodeHandle root, CKtraversalOrder order, CKgraphVisitor visitor, void* context);

/* CalypsoDAG logging functionality */

    /**
     * @brief Dynamic configuration driver to toggle log stream depth at runtime.
    */
    void CKdag_setLogLevel(CKlogLevel level);

    /**
     * @brief Retrieves the current logging level
    */
    CKlogLevel CKdag_getLogLevel(void);

    /**
     * @brief Flushes the internal log stream to console or file output, depending on the configuration
    */
    void CKdag_flushLog(CKlogFormat format);

    /**
     * @brief Internal utility to bubble up log messages to an output handler
    */
    void CKdag_bubbleLog(CKgraphNodeHandle node, const char* message);
    
/* ================================ */

#endif // CALYPSO_DAG_H