/* _CK_ */
#include <stdio.h>
#include <assert.h>
#include "CalypsoDAG.h"
#include "CalypsoDAG_typedef.h"

// A custom structural wrapper simulating an asset face anchor
typedef struct {
    CKgraphNodeHeader header; // Embedded system primitive layout
    const char* face_label;
    int weight;
} MosaicFaceNode;

// Simple visitor to count nodes and print the hierarchy flattening stream
static bool MockLayoutVisitor(CKgraphNodeHandle handle, void* context) {
    MosaicFaceNode* node = (MosaicFaceNode*)handle;
    int* counter = (int*)context;
    
    printf("  [Stream Element %d] Flattened Face: %s\n", (*counter)++, node->face_label);
    return true; // Keep iterating
}

int main(void) {
    printf("[TETHYS RUNTIME] Initiating CalypsoDAG MVP Verification...\n");
    CKdag_setLogLevel(CK_LOG_ALL);

    // 1. Upfront Static Allocation of our Mosaic Node Space
    MosaicFaceNode root_compound = { .face_label = "Root_Compound_Square", .weight = 100 };
    MosaicFaceNode child_rect_A  = { .face_label = "Subdivision_Face_A",   .weight = 50 };
    MosaicFaceNode child_rect_B  = { .face_label = "Subdivision_Face_B",   .weight = 50 };
    MosaicFaceNode sub_leaf_A1   = { .face_label = "Rectified_Leaf_A1",    .weight = 25 };

    // 2. Assemble the Planar Subdivision Hierarchy
    // root holds [child_rect_A, child_rect_B]
    // child_rect_A holds [sub_leaf_A1]
    CKdag_AppendChild((CKgraphNodeHandle)&root_compound, (CKgraphNodeHandle)&child_rect_A);
    CKdag_AppendChild((CKgraphNodeHandle)&root_compound, (CKgraphNodeHandle)&child_rect_B);
    CKdag_AppendChild((CKgraphNodeHandle)&child_rect_A,  (CKgraphNodeHandle)&sub_leaf_A1);

    // 3. Drive a Post-Order Layout Pass (Teardown/Bottom-up verification)
    int sequence_counter = 0;
    printf("\nExecuting Post-Order Serialization:\n");
    bool post_success = CKdag_Flatten((CKgraphNodeHandle)&root_compound, CK_POST_ORDER, MockLayoutVisitor, &sequence_counter);
    assert(post_success && "Post-order flattening failed.");
    assert(sequence_counter == 4 && "Failed to visit all allocated mosaic nodes.");

    // 4. Test Sub-Tree Lifecycle Detachment Primitive
    printf("\nDetaching Subtree: Subdivision_Face_A...\n");
    CKdag_DetachSubTree((CKgraphNodeHandle)&child_rect_A);

    // 5. Re-Verify Root Isolation
    sequence_counter = 0;
    printf("\nExecuting Post-Order Serialization (Post-Detachment):\n");
    CKdag_Flatten((CKgraphNodeHandle)&root_compound, CK_POST_ORDER, MockLayoutVisitor, &sequence_counter);
    assert(sequence_counter == 2 && "Detachment did not isolate the subtree cleanly.");

    CKdag_flushLog(CK_LOG_CONSOLE);
    printf("\n[SUCCESS] CalypsoDAG Revision01 Layout MVP Contract Met.\n");
    return 0;
}
