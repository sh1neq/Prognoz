/* ============================================================================
   Требование ПРГ:Ту-4 - Прогнозирование матрицы ковариации (часть 2)
   Описание: P = F * P' * F^T + G * Q * G^T
   Постусловие: выходная матрица P содержит полную прогнозируемую ковариацию
   
   Требование ПРГ:Ту-3 также реализовано здесь как часть вычислений
   ============================================================================ */

#include "../include/predictor.h"
#include "predictor_utils.h"

#define MAX_DIM 100

Pred_Status Pred_Step3_CalcP(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    
    uint32_t n = params->n;
    uint32_t p = params->p;
    
    // ПРГ:Ту-5.3 - проверка максимальной размерности
    if (n > MAX_DIM || p > MAX_DIM) return PRED_STATUS_SIZE_MISMATCH;
    
    // Временные матрицы на стеке (ПРГ:Ту-5.3)
    double F_P_prime[MAX_DIM * MAX_DIM];
    double temp1[MAX_DIM * MAX_DIM];
    double G_Q[MAX_DIM * MAX_DIM];
    double temp2[MAX_DIM * MAX_DIM];
    double F_transpose[MAX_DIM * MAX_DIM];
    double G_transpose[MAX_DIM * MAX_DIM];
    
    Pred_Status status;
    
    // ПРГ:Ту-3 - F * P'
    status = Matrix_Mul(params->F, params->P_prime, F_P_prime, n, n, n);
    if (status != PRED_STATUS_OK) return status;
    
    // F^T
    status = Matrix_Transpose(params->F, F_transpose, n, n);
    if (status != PRED_STATUS_OK) return status;
    
    // (F * P') * F^T
    status = Matrix_Mul(F_P_prime, F_transpose, temp1, n, n, n);
    if (status != PRED_STATUS_OK) return status;
    
    // G * Q
    status = Matrix_Mul(params->G, params->Q, G_Q, n, p, p);
    if (status != PRED_STATUS_OK) return status;
    
    // G^T
    status = Matrix_Transpose(params->G, G_transpose, n, p);
    if (status != PRED_STATUS_OK) return status;
    
    // (G * Q) * G^T
    status = Matrix_Mul(G_Q, G_transpose, temp2, n, p, n);
    if (status != PRED_STATUS_OK) return status;
    
    // ПРГ:Ту-4 - P = temp1 + temp2
    return Matrix_Add(temp1, temp2, params->P, n, n);
}
