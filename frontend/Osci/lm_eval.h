/*
 * Project LevenbergMarquardtLeastSquaresFitting
 *        (more info in lmmin.c)
 *
 * File lm_eval.h (C) Joachim Wuttke 2004-8 www.messen-und-deuten.de/lmfit
 *
 * Default declaration of user data type, data evalution function,
 * and printout routine. Default implementation in lm_eval.c,
 * usage shown in lm_test.c.
 */
 
#ifndef LM_EVAL_H
#define LM_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /* may be modified to hold arbitrary data */
    double *user_t;
    double *user_y;
    double (*user_func) (double user_t_point, double *par);
} lm_data_type;

void lm_evaluate_default(double *par, int m_dat, double *fvec,
			 void *data, int *info);

void lm_print_default(int n_par, double *par, int m_dat, double *fvec,
		      void *data, int iflag, int iter, int nfev);

#ifdef __cplusplus
}
#endif
		      
#endif /* LM_EVAL_H */
