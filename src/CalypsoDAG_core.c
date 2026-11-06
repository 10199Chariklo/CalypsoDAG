/* _CK_ */

/* Standard C11 Libraries */
#include <stdbool.h>
#include <stddef.h>

/* CalypsoDAG Subsystem */
#include "CalypsoDAG.h" // Public module header
#include "CalypsoDAG_typedef.h" // Internal type definitions

/* Internal logging hook */
#define CALYPSO_TRACE_EVENT(level, node, message)           \
    do {                                                    \
        if (CKdag_getLogLevel() & (level)) {                \
            CKdag_bubbleLog((node), (message));             \
        }                                                   \
    } while (0)

// Attaches a node handle as the last child of a parent node handle
void CKdag_AppendChild(CKgraphNodeHandle parent, CKgraphNodeHandle child) {
    if (!parent || !child) return;
    
    // Inward conversion from public handle to explicit internal structural layout
    CKgraphNodeHeader* pNode = (CKgraphNodeHeader*)parent;
    CKgraphNodeHeader* cNode = (CKgraphNodeHeader*)child;
    
    cNode->parent = pNode;
    cNode->nextSibling = NULL;
    
    if (!pNode->firstChild) {
        pNode->firstChild = cNode;
        cNode->prevSibling = NULL;
    } else {
        CKgraphNodeHeader* current = pNode->firstChild;
        while (current->nextSibling) {
            current = current->nextSibling;
        }
        current->nextSibling = cNode;
        cNode->prevSibling = current;
    }

    CALYPSO_TRACE_EVENT(CK_LOG_LIFECYCLE, parent, "Appended child node to parent.");
}

// Sub-Tree Lifecycle Detachment Primitive
void CKdag_DetachSubTree(CKgraphNodeHandle node) {
    if (!node) return;
    
    CKgraphNodeHeader* target = (CKgraphNodeHeader*)node;
    
    // Update parent's tracking anchor if this is the first child
    if (target->parent && target->parent->firstChild == target) {
        target->parent->firstChild = target->nextSibling;
    }
    
    // Stitch adjacent siblings together
    if (target->prevSibling) {
        target->prevSibling->nextSibling = target->nextSibling;
    }
    if (target->nextSibling) {
        target->nextSibling->prevSibling = target->prevSibling;
    }
    
    // Isolate the node completely
    target->parent = NULL;
    target->prevSibling = NULL;
    target->nextSibling = NULL;

    CALYPSO_TRACE_EVENT(CK_LOG_LIFECYCLE, node, "Detached node from parent and siblings.");
}

/* Traversal Engine Implementations */

    static bool CKgraph_TraversePreOrder(CKgraphNodeHeader* root, CKgraphVisitor visitor, void* context) {
        if (!root) return true;

        CKgraphNodeHeader* current = root;
        
        while (current) {
            // 1. Visit the active layout node
            if (!visitor((CKgraphNodeHandle)current, context)) return false;

            // 2. Deep Dive: Move down into the child layer if it exists
            if (current->firstChild) {
                current = current->firstChild;
                continue;
            }

            // 3. Surface Mapping: Look for the next lateral layout sibling
            if (current->nextSibling) {
                current = current->nextSibling;
                continue;
            }

            // 4. Backtrack Cascade: If we reach a leaf element, step back up to find uncle branches
            CKgraphNodeHeader* parentNode = current->parent;
            current = NULL; // Reset to catch loop completion conditions safely
            
            while (parentNode && parentNode != root) {
                if (parentNode->nextSibling) {
                    current = parentNode->nextSibling;
                    break; // Found our next unvisited target
                }
                parentNode = parentNode->parent;
            }
        }
        
        CALYPSO_TRACE_EVENT(CK_LOG_TRAVERSAL, root, "Completed pre-order traversal.");
        return true;
    }

    static bool CKgraph_TraversePostOrder(CKgraphNodeHeader* root, CKgraphVisitor visitor, void* context) {
        if (!root) return true;
        
        // Process child cascades completely first
        CKgraphNodeHeader* child = root->firstChild;
        while (child) {
            if (!CKgraph_TraversePostOrder(child, visitor, context)) return false;
            child = child->nextSibling;
        }
        
        // Visit current node last
        CALYPSO_TRACE_EVENT(CK_LOG_TRAVERSAL, root, "Completed post-order traversal.");
        return visitor((CKgraphNodeHandle)root, context);
    }

    static bool CKgraph_TraverseInOrder(CKgraphNodeHeader* root, CKgraphVisitor visitor, void* context) {
        if (!root) return true;
        
        CKgraphNodeHeader* child = root->firstChild;
        
        // 1. Visit first child branch if it exists
        if (child) {
            if (!CKgraph_TraverseInOrder(child, visitor, context)) return false;
            child = child->nextSibling;
        }
        
        // 2. Visit parent (Current root context)
        if (!visitor((CKgraphNodeHandle)root, context)) return false;
        
        // 3. Visit remaining sibling structures
        while (child) {
            if (!CKgraph_TraverseInOrder(child, visitor, context)) return false;
            child = child->nextSibling;
        }

        CALYPSO_TRACE_EVENT(CK_LOG_TRAVERSAL, root, "Completed in-order traversal.");
        return true;
    }

/* ================================ */

// Master execution driver for flattening passes and visitor hooks
bool CKdag_Flatten(CKgraphNodeHandle root, CKtraversalOrder order, CKgraphVisitor visitor, void* context) {
    if (!root || !visitor) return false;
    CALYPSO_TRACE_EVENT(CK_LOG_TRAVERSAL, root, "Beggining flattening pass.");

    CKgraphNodeHeader* internalRoot = (CKgraphNodeHeader*)root;
    
    switch (order) {
        case CK_PRE_ORDER:  return CKgraph_TraversePreOrder(internalRoot, visitor, context);
        case CK_POST_ORDER: return CKgraph_TraversePostOrder(internalRoot, visitor, context);
        case CK_IN_ORDER:   return CKgraph_TraverseInOrder(internalRoot, visitor, context);
        default: return false;
    }
}