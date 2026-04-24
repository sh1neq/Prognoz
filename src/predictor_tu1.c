/* ============================================================================
   Требование ПРГ:Ту-1 - Общий состав входных данных
   1.1 Определение структуры данных
   1.2 Функция инициализации с проверкой
   1.3 Проверка указателей на NULL
   1.4 Проверка размерностей
   1.5 Заголовочный файл
   
   Постусловие: все входные параметры проверены на корректность
   ============================================================================ */

#include "../include/predictor.h"
#include <stddef.h>

Pred_Status Pred_ValidateParams(const Pred_Params *params) {
    // ПРГ:Ту-1.3 - проверка указателя на структуру
    if (params == NULL) {
        return PRED_STATUS_NULL_PTR;
    }
    
    // ПРГ:Ту-1.4 - проверка размерностей
    if (params->n == 0 || params->p == 0) {
        return PRED_STATUS_INVALID_DIM;
    }
    
    // ПРГ:Ту-1.3 - проверка всех указателей
    if (params->X_prime == NULL || params->P_prime == NULL ||
        params->F == NULL || params->G == NULL || params->Q == NULL ||
        params->X == NULL || params->P == NULL) {
        return PRED_STATUS_NULL_PTR;
    }
    
    return PRED_STATUS_OK;
}
