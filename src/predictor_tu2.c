/* ============================================================================
   Требование ПРГ:Ту-2 - Прогнозирование вектора состояния
   Описание: X = F * X'
   Постусловие: выходной вектор X содержит результат умножения F на X'
   ============================================================================ */

#include "../include/predictor.h"
#include "predictor_utils.h"  /* см. predictor_utils.c */

Pred_Status Pred_Step1_CalcX(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    
    uint32_t n = params->n;
    
    // ПРГ:Ту-2 - X = F * X'
    return Matrix_Vector_Mul(params->F, params->X_prime, params->X, n, n);
}
