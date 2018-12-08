#include "data.h"
#include "string.h"
#include "csapp.h"
#include "debug.h"

/*
 * A "blob" consists of arbitrary data having a specified size
 * and content.  The content must not be null.  Once a blob has
 * been created, its size and content do not change.  A blob has
 * a reference count field, which is used to keep track of the
 * number of pointers that currently exist pointing to the blob.
 * New pointers are created using blob_ref(), which increments the
 * reference count.  Pointers are destroyed using blob_unref(),
 * which decrements the reference count.  As long as the reference
 * count of a blob is nonzero, it will not be freed.
 */
// typedef struct blob {
//     pthread_mutex_t mutex;     // Mutex to protect reference count
//     int refcnt;
//     size_t size;
//     char *content;
//     char *prefix;              // String prefix of content (for debugging)
// } BLOB;

/*
 * A key consists of a pointer to a blob and a hash of the blob data.
 */
// typedef struct key {
//     int hash;
//     BLOB *blob;
// } KEY;

/*
 * A VERSION represents a single version of a value associated with a key
 * in the transactional store.  Each version has a creator transaction,
 * a pointer to a blob and links to the next and previous versions in the
 * list of all versions in the same map entry.
 */
// typedef struct version {
//     TRANSACTION *creator;
//     BLOB *blob;
//     struct version *next;
//     struct version *prev;
// } VERSION;

/*
 * Create a blob with given content and size.
 * The content is copied, rather than shared with the caller.
 * The returned blob has one reference, which becomes the caller's
 * responsibility.
 *
 * @param content  The content of the blob.
 * @param size  The size in bytes of the content.
 * @return  The new blob, which has reference count 1.
 */
BLOB *blob_create(char *content, size_t size){

    BLOB *bl = Malloc( sizeof(BLOB));
    memset(bl, 0, sizeof(BLOB));
    bl->content = Malloc(size);
    memset(bl->content, 0, size);
    memcpy(bl->content, content,size);
    //bl->content = val;
    //
    //bl->content[size]='\0';
    bl->prefix = Malloc(size);
    memset(bl->prefix, 0, size);
    memcpy(bl->prefix,content,size);
    bl->size = size;
    bl->refcnt = 0;
    pthread_mutex_init(&bl->mutex, NULL);
    char why[] = "for newly created blob";
    blob_ref(bl,why);
    return bl;


}

/*
 * Increase the reference count on a blob.
 *
 * @param bp  The blob.
 * @param why  Short phrase explaining the purpose of the increase.
 * @return  The blob pointer passed as the argument.
 */
BLOB *blob_ref(BLOB *bp, char *why){

     if(bp == NULL){
         return NULL;
     }
    pthread_mutex_lock(&bp->mutex);
    bp->refcnt = bp->refcnt+1;
   debug("increase ref count for blob %p, %s",bp, why);
    pthread_mutex_unlock(&bp->mutex);
    return bp;

}

/*
 * Decrease the reference count on a blob.
 * If the reference count reaches zero, the blob is freed.
 *
 * @param bp  The blob.
 * @param why  Short phrase explaining the purpose of the decrease.
 */
void blob_unref(BLOB *bp, char *why){
    if(bp == NULL){
        return;
    }
    pthread_mutex_lock(&bp->mutex);
    bp->refcnt = bp->refcnt -1;
    if(bp->refcnt == 0){
        Free(bp->content);
        Free(bp->prefix);
        //bp->prefix = NULL;
        //pthread_mutex_unlock(&bp->mutex);
        Free(bp);
        return;
    }
    pthread_mutex_unlock(&bp->mutex);
}

/*
 * Compare two blobs for equality of their content.
 *
 * @param bp1  The first blob.
 * @param bp2  The second blob.
 * @return 0 if the blobs have equal content, nonzero otherwise.
*/

int blob_compare(BLOB *bp1, BLOB *bp2){

    if( bp1 == NULL){
        return -1;
    }

    if(bp2 == NULL){
        return -1;
    }

    if(bp1->size != bp2->size){
        return -1;
    }

    return memcmp(bp1->content, bp2->content, bp1->size);

}

/*
 * Hash function for hashing the content of a blob.
 *
 * @param bp  The blob.
 * @return  Hash of the blob.
 */
int blob_hash(BLOB *bp){

   if(bp == NULL){
    return -1;
   }
   unsigned int hash = 0;
   int c;
   char *str= bp->content;
  // str[bp->size] = '\0';
   int count = 0;
   while (  (c = str[count]) && (count < bp->size) ){
    count++;
    hash +=c;
   }
   return hash;
}

/*
 * Create a key from a blob.
 * The key inherits the caller's reference to the blob.
 *
 * @param bp  The blob.
 * @return  the newly created key.
 */
KEY *key_create(BLOB *bp){

    //pthread_mutex_lock(&bp->mutex);
    KEY *k = Malloc(sizeof(KEY));
    memset(k, 0, sizeof(KEY));
    k->hash = blob_hash(bp);
    k->blob = bp;
    //pthread_mutex_unlock(&bp->mutex);
    //char why[] = "for key ref";
    //blob_ref(bp, why);

    return k;
}

/*
 * Dispose of a key, decreasing the reference count of the contained blob.
 * A key must be disposed of only once and must not be referred to again
 * after it has been disposed.
 *
 * @param kp  The key.
 */
void key_dispose(KEY *kp){

    //kp->blob->refcnt = kp->blob->refcnt-1;
    char why [] = "key disposal";
    blob_unref(kp->blob, why);
    Free(kp);
}

/*
 * Compare two keys for equality.
 *
 * @param kp1  The first key.
 * @param kp2  The second key.
 * @return  0 if the keys are equal, otherwise nonzero.
 */
int key_compare(KEY *kp1, KEY *kp2){

    if(kp1 == NULL){
        return -1;
    }

    if(kp2 == NULL){
        return -1;
    }

    if(kp1->hash != kp2->hash){
        return -1;
    }
    return blob_compare(kp1->blob, kp2->blob);
}

/*
 * Create a version of a blob for a specified creator transaction.
 * The version inherits the caller's reference to the blob.
 * The reference count of the creator transaction is increased to
 * account for the reference that is stored in the version.
 *
 * @param tp  The creator transaction.
 * @param bp  The blob.
 * @return  The newly created version.
 */
VERSION *version_create(TRANSACTION *tp, BLOB *bp){
   // pthread_mutex_lock(&bp->mutex);
    VERSION *ver = Malloc(sizeof(VERSION));
    ver->creator = tp;
    //ver->creator->refcnt = ver->creator->refcnt+1;
    char why[]= "creating version";
    trans_ref(ver->creator, why);
    ver->blob = bp;
    ver->next = NULL;
    ver->prev = NULL;
    //pthread_mutex_unlock(&bp->mutex);

    return ver;

}

/*
 * Dispose of a version, decreasing the reference count of the
 * creator transaction and contained blob.  A version must be
 * disposed of only once and must not be referred to again once
 * it has been disposed.
 *
 * @param vp  The version to be disposed.
 */
void version_dispose(VERSION *vp){

    //vp->creator->refcnt = vp->creator->refcnt-1;
    //

    char why[]="version dump";
    trans_unref(vp->creator, why);
    blob_unref(vp->blob, why);
    Free(vp);

}

