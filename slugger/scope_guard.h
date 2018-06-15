#ifndef __LOG_PRIV_SCOPE_GUARD
#define __LOG_PRIV_SCOPE_GUARD

// private scoped mutex wrapper
class priv_lock_guard
{
public:
    priv_lock_guard(pthread_mutex_t* buffer_mux):
        mux(buffer_mux)
    {
        pthread_mutex_lock(mux);
    }

    ~priv_lock_guard()
    {
        pthread_mutex_unlock(mux);
    }

private:
    pthread_mutex_t* mux;
};

#endif
// vim: set ts=4 sw=4 et :
