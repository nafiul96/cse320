#include "transaction.h"
#include "csapp.h"
#include "debug.h"



TRANSACTION trans_list;

int trans_id;
pthread_mutex_t myid;

/*
 * Initialize the transaction manager.
 */
void trans_init(void){
    pthread_mutex_init(&myid, NULL);
    trans_list.next = &trans_list;
    trans_list.prev = &trans_list;
    trans_id = 0;
}

/*
 * Finalize the transaction manager.
 */
void trans_fini(){


for( TRANSACTION *t = trans_list.next; t != &trans_list; t = t->next){
    if(t->status == TRANS_PENDING){
        t->waitcnt++;
        trans_abort(t);
    }
}

trans_show_all();

}

/*
 * Create a new transaction.
 *
 * @return  A pointer to the new transaction (with reference count 1)
 * is returned if creation is successful, otherwise NULL is returned.
 */
TRANSACTION *trans_create(void){

    pthread_mutex_lock(&myid);
    TRANSACTION * temp = Malloc( sizeof(TRANSACTION));
    pthread_mutex_init(&temp->mutex,NULL);
    temp->id = trans_id++;
    Sem_init(&temp->sem,0,0);
    temp->waitcnt = 0;
    temp->status = TRANS_PENDING;
    temp->refcnt = 0;
    temp->depends = NULL;
    temp->next = &trans_list;
    temp->prev = trans_list.prev;
    trans_list.prev->next = temp;
    trans_list.prev = temp;
    char why[]= "create new transaction";
    trans_ref(temp, why);
    pthread_mutex_unlock(&myid);
    return temp;


}

/*
 * Increase the reference count on a transaction.
 *
 * @param tp  The transaction.
 * @param why  Short phrase explaining the purpose of the increase.
 * @return  The transaction pointer passed as the argument.
 */
TRANSACTION *trans_ref(TRANSACTION *tp, char *why){
    pthread_mutex_lock(&tp->mutex);
    debug("Increasing ref count for transaction %d, %s", tp->id, why);
    tp->refcnt++;
    pthread_mutex_unlock(&tp->mutex);
    return tp;
}

/*
 * Decrease the reference count on a transaction.
 * If the reference count reaches zero, the transaction is freed.
 *
 * @param tp  The transaction.
 * @param why  Short phrase explaining the purpose of the decrease.
 */
void trans_unref(TRANSACTION *tp, char *why){

    if(tp == NULL)
        return;

    pthread_mutex_lock(&tp->mutex);
    debug("decreasing ref count for %d, %s",tp->id,why);
    tp->refcnt--;
    if(tp->refcnt == 0){

       if(tp->depends != NULL){

            DEPENDENCY *ptr = NULL;
            while( (ptr=tp->depends) != NULL){
                char why[] = "release dependency for freeing transaction";
                trans_unref(ptr->trans, why);
                tp->depends = tp->depends->next;
                Free(ptr);
            }

        }

        tp->prev->next = tp->next;
        tp->next->prev = tp->prev;
        Free(tp);
        return;
    }

    pthread_mutex_unlock(&tp->mutex);


}

/*
 * Add a transaction to the dependency set for this transaction.
 *
 * @param tp  The transaction to which the dependency is being added.
 * @param dtp  The transaction that is being added to the dependency set.
 */
void trans_add_dependency(TRANSACTION *tp, TRANSACTION *dtp){

    DEPENDENCY *dep = Malloc(  sizeof(DEPENDENCY) );
    dep->trans = dtp;
    dep->next = NULL;
    debug("trying to add transaction %d to depen of transaction %d", dtp->id, tp->id);
    if(tp->depends == NULL){
        tp->depends = dep;
        debug("NUll case dep added to tp's dependency");
        char why[]="increase dtp to add dependency";
        trans_ref(dtp,why);
        return;
    }

    pthread_mutex_lock(&tp->mutex);
    DEPENDENCY *ptr = tp->depends;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->next = dep;
    debug("Non null case, dep addded to tp's dependency");
    char why[]="increase dtp to add dependency";
    pthread_mutex_unlock(&tp->mutex);
    trans_ref(dtp,why);


}

/*
 * Try to commit a transaction.  Committing a transaction requires waiting
 * for all transactions in its dependency set to either commit or abort.
 * If any transaction in the dependency set abort, then the dependent
 * transaction must also abort.  If all transactions in the dependency set
 * commit, then the dependent transaction may also commit.
 *
 * In all cases, this function consumes a single reference to the transaction
 * object.
 *
 * @param tp  The transaction to be committed.
 * @return  The final status of the transaction: either TRANS_ABORTED,
 * or TRANS_COMMITTED.
 */
TRANS_STATUS trans_commit(TRANSACTION *tp){

    /**
     * this is without dependency
     */
    debug("trying to commit transaction %d", tp->id);
    if(tp->depends == NULL){

        //commit the transaction
        tp->status = TRANS_COMMITTED;

        //lock mutex
        pthread_mutex_lock(&tp->mutex);
        debug("Transaction %d 's status changed to COMMITTED",tp->id);
        for(int i=0; i<tp->waitcnt; i++){
            V(&tp->sem);
            debug("Releasing waitcount for Transaction %d, new waitcnt: %d", tp->id, tp->waitcnt);
        }
        tp->waitcnt = 0;
        //unlock mutex
        pthread_mutex_unlock(&tp->mutex);

        char why[]="transaction committed";
        trans_unref(tp, why);
        return TRANS_COMMITTED;
    }

    /**
     * this is with dependency
     */

    /**
     * Wait for a finalized status of dependecy sets
     */
    for(DEPENDENCY *ptr = tp->depends; ptr != NULL; ptr = ptr->next){
        if(ptr->trans->status == TRANS_PENDING){
            pthread_mutex_lock(&ptr->trans->mutex);
            ptr->trans->waitcnt++;
            debug("Transactin %d has waitcount of %d", ptr->trans->id, ptr->trans->waitcnt);
            pthread_mutex_unlock(&ptr->trans->mutex);
            debug("Transaction %d is waiting for Transaction %d",tp->id,ptr->trans->id);
            P(&ptr->trans->sem);
            debug("Transaction %d has return status of %d",ptr->trans->id, ptr->trans->status);
            if(ptr->trans->status == TRANS_ABORTED){

                tp->status = TRANS_ABORTED;
             pthread_mutex_lock(&tp->mutex);
             debug("Transaction %d 's status changed to ABORTED",tp->id);
            for(int i=0; i<tp->waitcnt; i++){
                V(&tp->sem);
                tp->waitcnt--;
                debug("Releasing waitcount for Transaction %d, new waitcnt: %d", tp->id, tp->waitcnt);
                //tp->waitcnt--;
            }
            tp->waitcnt = 0;
            pthread_mutex_unlock(&tp->mutex);
            char why[]="transaction aborted";
            trans_unref(tp, why);
            return TRANS_ABORTED;
            }

        }
    }


            tp->status = TRANS_COMMITTED;
            pthread_mutex_lock(&tp->mutex);
            debug("Transaction %d 's status changed to Committed",tp->id);
            int ct = tp->waitcnt;
            for(int i=0; i<ct; i++){
                V(&tp->sem);
                tp->waitcnt--;
                debug("Releasing waitcount for Transaction %d, new waitcnt: %d", tp->id, tp->waitcnt);
            }
            pthread_mutex_unlock(&tp->mutex);
            char why[]="transaction Committed";
            trans_unref(tp, why);
            return TRANS_COMMITTED;


}

/*
 * Abort a transaction.  If the transaction has already committed, it is
 * a fatal error and the program crashes.  If the transaction has already
 * aborted, no change is made to its state.  If the transaction is pending,
 * then it is set to the aborted state, and any transactions dependent on
 * this transaction must also abort.
 *
 * In all cases, this function consumes a single reference to the transaction
 * object.
 *
 * @param tp  The transaction to be aborted.
 * @return  TRANS_ABORTED.
 */
TRANS_STATUS trans_abort(TRANSACTION *tp){

    debug("Trying to abort transaction %d", tp->id);

    if(tp == NULL){
        return TRANS_ABORTED;
    }

    if(trans_get_status(tp) == TRANS_COMMITTED){
        char why[]="crash the program";
        trans_unref(tp, why);
        abort();
    }

    if(trans_get_status(tp) == TRANS_PENDING){

        tp->status = TRANS_ABORTED;
        pthread_mutex_lock(&tp->mutex);
        debug("Transaction %d 's status changed to ABORTED",tp->id);
        int cnt = tp->waitcnt;
        for(int i=0; i<cnt; i++){
            V(&tp->sem);
            tp->waitcnt--;
            debug("Releasing waitcount for Transaction %d, new waitcnt: %d", tp->id, tp->waitcnt);
        }
        pthread_mutex_unlock(&tp->mutex);
        char why[] = "about to abort";
        trans_unref(tp,why);
        return TRANS_ABORTED;

    }

    debug("Transaction %d has already aborted", tp->id);
    char why[]="for aborting again";
    trans_unref(tp,why);
    return TRANS_ABORTED;


}

/*
 * Get the current status of a transaction.
 * If the value returned is TRANS_PENDING, then we learn nothing,
 * because unless we are holding the transaction mutex the transaction
 * could be aborted at any time.  However, if the value returned is
 * either TRANS_COMMITTED or TRANS_ABORTED, then that value is the
 * stable final status of the transaction.
 *
 * @param tp  The transaction.
 * @return  The status of the transaction, as it was at the time of call.
 */
TRANS_STATUS trans_get_status(TRANSACTION *tp){
    return tp->status;
}

/*
 * Print information about a transaction to stderr.
 * No locking is performed, so this is not thread-safe.
 * This should only be used for debugging.
 *
 * @param tp  The transaction to be shown.
 */
void trans_show(TRANSACTION *tp){
    fprintf(stderr, "[id:%d, status:%d, refcnt:%d, waitcnt:%d] \n",tp->id, tp->status, tp->refcnt, tp->waitcnt);
}

/*
 * Print information about all transactions to stderr.
 * No locking is performed, so this is not thread-safe.
 * This should only be used for debugging.
 */
void trans_show_all(){
    for(TRANSACTION *ptr = trans_list.next; ptr != &trans_list; ptr = ptr->next){
        trans_show(ptr);
    }
}


