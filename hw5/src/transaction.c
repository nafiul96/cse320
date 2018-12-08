// #include "transaction.h"
// #include "csapp.h"
// #include "debug.h"


// /*
//  * A transaction is a context within which to perform a sequence of operations
//  * on a transactional store.  Every transaction has a unique transaction ID.
//  * There is an ordering on transaction IDs, so that it makes sense to say that
//  * one transaction ID is "less than" another.
//  *
//  * When it is first created, a transaction is in the "pending" state.
//  * Each time an operation is performed in the context of a transaction,
//  * there is a possibility that the transaction will enter the "aborted" state,
//  * due to a conflict with other transactions executing concurrently with it.
//  * Once a transaction has aborted, it is not possible to perform any further operations
//  * within it and all effects that it has had on the store are deleted.  Once all the
//  * desired operations have been performed within a transaction, an attempt can be made
//  * to "commit" the transaction.  The transaction will then either abort or enter
//  * the "committed" state.  Once a transaction has entered the committed state,
//  * all effects that it has had on the store are made permanent.
//  */

// /*
//  * Transaction status.
//  * Also used as return values from transaction operations.
//  */
// //typedef enum { TRANS_PENDING, TRANS_COMMITTED, TRANS_ABORTED } TRANS_STATUS;

// /*
//  * A transaction may become "dependent" on another transaction.
//  * This occurs when a transaction accesses an entry in the store that has
//  * previously been accessed by a pending transaction having a smaller transaction
//  * ID.  Once that occurs, the dependent transaction cannot commit until the
//  * the transaction on which it depends has committed.  Moreover, if the
//  * other transaction aborts, then the dependent transaction must also abort.
//  *
//  * The dependencies of a transaction are recorded in a "dependency set",
//  * which is part of the representation of a transaction.  A dependency set
//  * is represented as a singly linked list of "dependency" nodes having the
//  * following structure.  Any given transaction can occur at most once in a
//  * single dependency set.
//  */
// // typedef struct dependency {
// //   struct transaction *trans;  // Transaction on which the dependency depends.
// //   struct dependency *next;    // Next dependency in the set.
// // } DEPENDENCY;

// /*
//  * Structure representing a transaction.
//  */
// /*typedef struct transaction {
//   unsigned int id;           // Transaction ID.
//   unsigned int refcnt;       // Number of references (pointers) to transaction.
//   TRANS_STATUS status;       // Current transaction status.
//   DEPENDENCY *depends;       // Singly-linked list of dependencies.
//   int waitcnt;               // Number of transactions waiting for this one.
//   sem_t sem;                 // Semaphore to wait for transaction to commit or abort.
//   pthread_mutex_t mutex;     // Mutex to protect fields.
//   struct transaction *next;  // Next in list of all transactions
//   struct transaction *prev;  // Prev in list of all transactions.
// } TRANSACTION;

// //
// //  * For debugging purposes, all transactions having a nonzero reference count
// //  * are maintained in a circular, doubly linked list which has the following
// //  * sentinel as its head.
//  //*/
// TRANSACTION trans_list;

// int count;
// pthread_mutex_t myid;

// /*
//  * Initialize the transaction manager.
//  */
// void trans_init(void){
//     pthread_mutex_init(&myid, NULL);
//     pthread_mutex_lock(&myid);
//     trans_list.next = &trans_list;
//     trans_list.prev = &trans_list;
//     count = 0;
//     pthread_mutex_unlock(&myid);
// }

// /*
//  * Finalize the transaction manager.
//  */
// void trans_fini(){
//    pthread_mutex_lock(&myid);
//     TRANSACTION *ptr = &trans_list;
//     for(ptr = ptr->next; ptr != &trans_list; ptr = ptr->next){
//         if(ptr->status == TRANS_PENDING){
//             trans_abort(ptr);
//         }
//     }


//    ptr = &trans_list;

//     for(ptr= ptr->next; ptr != &trans_list; ptr = ptr->next){
//         int ct = ptr->refcnt;
//         for(int i=1; i<ct; i++){
//             trans_unref(ptr, "cleaning up transaction");
//         }
//     }
//     trans_list.next = NULL;
//     trans_list.prev = NULL;

//     pthread_mutex_unlock(&myid);
//    // ptr = &trans_list;
//    // while()
// }

// /*
//  * Create a new transaction.
//  *
//  * @return  A pointer to the new transaction (with reference count 1)
//  * is returned if creation is successful, otherwise NULL is returned.
//  */
// TRANSACTION *trans_create(void){

//     pthread_mutex_lock(&myid);

//     TRANSACTION * temp = Malloc( sizeof(TRANSACTION));
//     pthread_mutex_init(&temp->mutex,NULL);
//     temp->id = count++;
//     Sem_init(&temp->sem,0,0);
//     temp->waitcnt = 0;
//     temp->status = TRANS_PENDING;
//     temp->refcnt = 0;
//     temp->depends = NULL;


//     temp->next = &trans_list;
//     temp->prev = trans_list.prev;
//     trans_list.prev->next = temp;
//     trans_list.prev = temp;


//     char why[]= "create new transaction";
//     trans_ref(temp, why);
//     pthread_mutex_unlock(&myid);
//     return temp;


// }

// /*
//  * Increase the reference count on a transaction.
//  *
//  * @param tp  The transaction.
//  * @param why  Short phrase explaining the purpose of the increase.
//  * @return  The transaction pointer passed as the argument.
//  */
// TRANSACTION *trans_ref(TRANSACTION *tp, char *why){
//     if(tp == NULL){
//         return NULL;
//     }
//     pthread_mutex_lock(&tp->mutex);
//     tp->refcnt++;
//     pthread_mutex_unlock(&tp->mutex);
//     return tp;
// }

// /*
//  * Decrease the reference count on a transaction.
//  * If the reference count reaches zero, the transaction is freed.
//  *
//  * @param tp  The transaction.
//  * @param why  Short phrase explaining the purpose of the decrease.
//  */
// void trans_unref(TRANSACTION *tp, char *why){

//     if(tp == NULL)
//         return;

//     pthread_mutex_lock(&tp->mutex);
//     debug("decreasing ref count for %d, %s",tp->id,why);
//     tp->refcnt--;
//     if(tp->refcnt == 0){

//         if( tp->depends == NULL){
//             //
//             //
//             //blockptr->links.prev->links.next = blockptr->links.next;
//             //blockptr->links.next->links.prev = blockptr->links.prev;

//             //
//             tp->prev->next = tp->next;
//             tp->next->prev = tp->prev;
//             Free(tp);
//         }else{

//             DEPENDENCY *ptr = tp->depends;
//             while(ptr != NULL){
//                 char why[] = "release dependency for freeing transaction";
//                 trans_unref(ptr->trans, why);
//                 tp->depends = tp->depends->next;
//                 Free(ptr);
//             }
//             //
//             //
//             tp->prev->next = tp->next;
//             tp->next->prev = tp->prev;
//             Free(tp);
//         }


//         return;
//     }

//     pthread_mutex_unlock(&tp->mutex);


// }

// /*
//  * Add a transaction to the dependency set for this transaction.
//  *
//  * @param tp  The transaction to which the dependency is being added.
//  * @param dtp  The transaction that is being added to the dependency set.
//  */
// void trans_add_dependency(TRANSACTION *tp, TRANSACTION *dtp){

//     DEPENDENCY *dep = Malloc(  sizeof(DEPENDENCY) );
//     dep->trans = dtp;
//     dep->next = NULL;
//     debug("trying to add transaction %d to depen of transaction %d", dtp->id, tp->id);
//     if(tp->depends == NULL){
//         tp->depends = dep;
//         char why[]="increase dtp to add dependency";
//         trans_ref(dtp,why);
//         return;
//     }

//     DEPENDENCY *ptr = tp->depends;
//     while(ptr->next != NULL){
//         ptr = ptr->next;
//     }
//     ptr->next = dep;
//     char why[]="increase dtp to add dependency";
//     trans_ref(dtp,why);


// }

// /*
//  * Try to commit a transaction.  Committing a transaction requires waiting
//  * for all transactions in its dependency set to either commit or abort.
//  * If any transaction in the dependency set abort, then the dependent
//  * transaction must also abort.  If all transactions in the dependency set
//  * commit, then the dependent transaction may also commit.
//  *
//  * In all cases, this function consumes a single reference to the transaction
//  * object.
//  *
//  * @param tp  The transaction to be committed.
//  * @return  The final status of the transaction: either TRANS_ABORTED,
//  * or TRANS_COMMITTED.
//  */
// TRANS_STATUS trans_commit(TRANSACTION *tp){
//     debug("trying to commit transaction %d", tp->id);
//     if(tp->depends == NULL){
//         tp->status = TRANS_COMMITTED;
//         char why[]="transaction committed";
//             trans_unref(tp, why);
//         return TRANS_COMMITTED;
//     }

//     for(DEPENDENCY *ptr = tp->depends; ptr != NULL; ptr = ptr->next){
//         if(ptr->trans->status == TRANS_PENDING){
//             pthread_mutex_lock(&ptr->trans->mutex);
//             ptr->trans->waitcnt++;
//             pthread_mutex_unlock(&ptr->trans->mutex);
//             P(&ptr->trans->sem);

//         }
//     }


//     for(DEPENDENCY *p = tp->depends; p != NULL; p= p->next){
//         if(p->trans->status == TRANS_ABORTED){

//             tp->status = TRANS_ABORTED;
//              pthread_mutex_lock(&tp->mutex);
//             int ct = tp->waitcnt;
//             for(int i=0; i<ct; i++){
//                 V(&tp->sem);
//             }
//             pthread_mutex_unlock(&tp->mutex);
//             char why[]="transaction aborted";
//             trans_unref(tp, why);
//             return TRANS_ABORTED;
//         }
//     }

//             tp->status = TRANS_COMMITTED;
//             pthread_mutex_lock(&tp->mutex);
//             int ct = tp->waitcnt;
//             for(int i=0; i<ct; i++){
//                 V(&tp->sem);
//             }
//             pthread_mutex_unlock(&tp->mutex);
//             char why[]="transaction Committed";
//             trans_unref(tp, why);
//             return TRANS_COMMITTED;


// }

// /*
//  * Abort a transaction.  If the transaction has already committed, it is
//  * a fatal error and the program crashes.  If the transaction has already
//  * aborted, no change is made to its state.  If the transaction is pending,
//  * then it is set to the aborted state, and any transactions dependent on
//  * this transaction must also abort.
//  *
//  * In all cases, this function consumes a single reference to the transaction
//  * object.
//  *
//  * @param tp  The transaction to be aborted.
//  * @return  TRANS_ABORTED.
//  */
// TRANS_STATUS trans_abort(TRANSACTION *tp){

//     debug("Trying to abort transaction %d", tp->id);

//     if(tp == NULL){
//         return TRANS_ABORTED;
//     }

//     if(tp->status == TRANS_COMMITTED){
//         abort();
//     }

//     if(tp->status == TRANS_PENDING){

//         tp->status = TRANS_ABORTED;
//         pthread_mutex_lock(&tp->mutex);
//         int cnt = tp->waitcnt;
//         for(int i=0; i<cnt; i++){
//             V(&tp->sem);
//         }
//         pthread_mutex_unlock(&tp->mutex);
//         char why[] = "about to abort";
//         trans_unref(tp,why);
//         return TRANS_ABORTED;

//     }

//     return TRANS_ABORTED;


// }

// /*
//  * Get the current status of a transaction.
//  * If the value returned is TRANS_PENDING, then we learn nothing,
//  * because unless we are holding the transaction mutex the transaction
//  * could be aborted at any time.  However, if the value returned is
//  * either TRANS_COMMITTED or TRANS_ABORTED, then that value is the
//  * stable final status of the transaction.
//  *
//  * @param tp  The transaction.
//  * @return  The status of the transaction, as it was at the time of call.
//  */
// TRANS_STATUS trans_get_status(TRANSACTION *tp){
//     return tp->status;
// }

// /*
//  * Print information about a transaction to stderr.
//  * No locking is performed, so this is not thread-safe.
//  * This should only be used for debugging.
//  *
//  * @param tp  The transaction to be shown.
//  */
// void trans_show(TRANSACTION *tp){
//     return;
// }

// /*
//  * Print information about all transactions to stderr.
//  * No locking is performed, so this is not thread-safe.
//  * This should only be used for debugging.
//  */
// void trans_show_all(void){
//     return;
// }


