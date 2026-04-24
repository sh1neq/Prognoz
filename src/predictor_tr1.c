/* ============================================================================
   Требование ПРГ:Тр-1 - Результаты выполнения модуля
   1.1 Расширение структуры признаками
   1.2 Функция установки признака X_updated
   1.3 Функция установки признака P_updated
   1.4 Функция получения признака X_updated
   1.5 Функция получения признака P_updated
   1.6 Модификация прогнозирования для установки признаков
   1.7 Обновление X' и P'
   
   Постусловие: признаки корректно устанавливаются и читаются
   ============================================================================ */

#include "../include/predictor.h"
#include "predictor_utils.h"

void Pred_SetXUpdated(Pred_Params *params) {
    if (params != NULL) params->X_updated = true;
}

void Pred_SetPUpdated(Pred_Params *params) {
    if (params != NULL) params->P_updated = true;
}

bool Pred_IsXUpdated(const Pred_Params *params) {
    return (params != NULL) ? params->X_updated : false;
}

bool Pred_IsPUpdated(const Pred_Params *params) {
    return (params != NULL) ? params->P_updated : false;
}

void Pred_ResetFlags(Pred_Params *params) {
    if (params != NULL) {
        params->X_updated = false;
        params->P_updated = false;
    }
}

// ПРГ:Тр-1.3 - обновление X' (шаг 2 последовательности)
Pred_Status Pred_Step2_UpdateXPrime(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    return Matrix_Copy(params->X, params->X_prime, params->n, 1);
}

// ПРГ:Тр-1.5 - обновление P' (шаг 4 последовательности)
Pred_Status Pred_Step4_UpdatePPrime(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    return Matrix_Copy(params->P, params->P_prime, params->n, params->n);
}
