
#define T_RING_BUFFER(T, N, S) 			\
    static T N[S] = {0}; 			\
    static unsigned int N##_m_size  = S; 	\
    static unsigned int N##_rhead = 0; 		\
    static unsigned int N##_whead =0;  		\
    static void N##_init(void) 			\
    {						\
    }						\
    static T N##_get_data(void)			\
    {						\
        return N[N##_rhead++ & S-1];		\
    }						\
    static void N##_put_data(T data)		\
    {						\
        N[N##_whead++ & S-1] = data;		\
    }						\
