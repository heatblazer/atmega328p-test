
#define T_RING_BUFFER(T, N, S) 			\
    static T N##arr[(S)] = {0}; 		\
    static unsigned short N##_m_size  = (S);	\
    static unsigned short N##_rhead = 0; 	\
    static unsigned short N##_whead =0;  	\
    static void N##_init(void) 			\
    {						\
    }						\
    static void N##_get_data(T* ret)		\
    {						\
	if (N##_rhead < N##_whead)		\
	    *ret = N##arr[N##_rhead++ & S-1];	\
    }						\
    static void N##_put_data(T data)		\
    {						\
	if (N##_whead < N##_m_size &&		\
	    N##_rhead <= N##_whead)		\
        N##arr[N##_whead++ & S-1] = data;	\
    }						
