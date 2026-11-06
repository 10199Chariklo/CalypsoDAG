/* _CK_ */

#ifndef CALYPSO_DAG_TYPEDEF_H
#define CALYPSO_DAG_TYPEDEF_H

// 
struct CKgraphNodeHeader {
    struct CKgraphNodeHeader* parent;
    struct CKgraphNodeHeader* firstChild;
    struct CKgraphNodeHeader* prevSibling;
    struct CKgraphNodeHeader* nextSibling;
};
typedef struct CKgraphNodeHeader CKgraphNodeHeader;

#endif // CALYPSO_DAG_TYPEDEF_H