#ifndef RWLOCK_HH
#define RWLOCK_HH

#include <pthread.h>

class RWLock {
	public:
		RWLock() {
			pthread_rwlock_init(&this->lock, 0);
		}
		~RWLock() {
			pthread_rwlock_destroy(&this->lock);
		}
		void rdlock() {
			pthread_rwlock_rdlock(&this->lock);
		}
		void wrlock() {
			pthread_rwlock_wrlock(&this->lock);
		}
		void unlock() {
			pthread_rwlock_unlock(&this->lock);
		}
	private:
		pthread_rwlock_t lock;
};

#endif
