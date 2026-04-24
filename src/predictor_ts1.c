/* ============================================================================
   Требование ПРГ:Тс-1 - Назначение и общая логика выполнения
   1.1 Функция последовательного выполнения
   1.2-1.7 Шаги 1-6
   1.8 Атомарность (откат при ошибке)
   1.9 Последовательная зависимость шагов
   
   Постусловие: последовательность выполняется атомарно
   ============================================================================ */

#include "../include/predictor.h"
#include <stdlib.h>
#include <string.h>

/* ============================================================================
   ПРГ:Тс-1.8 - Резервное копирование и откат
   ============================================================================ */

Pred_Status Pred_AllocBackup(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    
    uint32_t n = params->n;
    uint32_t size_vector = n;
    uint32_t size_matrix = n * n;
    
    params->X_prime_backup = (double*)malloc(size_vector * sizeof(double));
    params->P_prime_backup = (double*)malloc(size_matrix * sizeof(double));
    params->X_backup = (double*)malloc(size_vector * sizeof(double));
    params->P_backup = (double*)malloc(size_matrix * sizeof(double));
    
    if (params->X_prime_backup == NULL || params->P_prime_backup == NULL ||
        params->X_backup == NULL || params->P_backup == NULL) {
        Pred_FreeBackup(params);
        return PRED_STATUS_ALLOC_FAIL;
    }
    
    params->backup_allocated = true;
    return PRED_STATUS_OK;
}

void Pred_FreeBackup(Pred_Params *params) {
    if (params == NULL) return;
    
    free(params->X_prime_backup);
    free(params->P_prime_backup);
    free(params->X_backup);
    free(params->P_backup);
    
    params->X_prime_backup = NULL;
    params->P_prime_backup = NULL;
    params->X_backup = NULL;
    params->P_backup = NULL;
    params->backup_allocated = false;
}

Pred_Status Pred_CreateBackup(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    if (!params->backup_allocated) return PRED_STATUS_ALLOC_FAIL;
    
    uint32_t n = params->n;
    
    // Копирование с использованием Matrix_Copy (из predictor_utils)
    extern Pred_Status Matrix_Copy(const double* src, double* dst, uint32_t rows, uint32_t cols);
    
    Matrix_Copy(params->X_prime, params->X_prime_backup, n, 1);
    Matrix_Copy(params->P_prime, params->P_prime_backup, n, n);
    Matrix_Copy(params->X, params->X_backup, n, 1);
    Matrix_Copy(params->P, params->P_backup, n, n);
    
    return PRED_STATUS_OK;
}

void Pred_Rollback(Pred_Params *params) {
    if (params == NULL) return;
    if (!params->backup_allocated) return;
    
    uint32_t n = params->n;
    
    extern Pred_Status Matrix_Copy(const double* src, double* dst, uint32_t rows, uint32_t cols);
    
    Matrix_Copy(params->X_prime_backup, params->X_prime, n, 1);
    Matrix_Copy(params->P_prime_backup, params->P_prime, n, n);
    Matrix_Copy(params->X_backup, params->X, n, 1);
    Matrix_Copy(params->P_backup, params->P, n, n);
}

/* ============================================================================
   ПРГ:Тс-1.6, 1.7 - Формирование признаков (шаги 5 и 6)
   ============================================================================ */

Pred_Status Pred_Step5_SetXFlag(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    Pred_SetXUpdated(params);
    return PRED_STATUS_OK;
}

Pred_Status Pred_Step6_SetPFlag(Pred_Params *params) {
    if (params == NULL) return PRED_STATUS_NULL_PTR;
    Pred_SetPUpdated(params);
    return PRED_STATUS_OK;
}

/* ============================================================================
   ПРГ:Тс-1.1 - Основная функция последовательного выполнения
   ============================================================================ */

Pred_Status Pred_RunSequence(Pred_Params *params) {
    Pred_Status status;
    uint32_t step = 0;
    
    // Валидация входных параметров
    status = Pred_ValidateParams(params);
    if (status != PRED_STATUS_OK) return status;
    
    // Подготовка резервного копирования
    status = Pred_AllocBackup(params);
    if (status != PRED_STATUS_OK) return status;
    
    status = Pred_CreateBackup(params);
    if (status != PRED_STATUS_OK) {
        Pred_FreeBackup(params);
        return status;
    }
    
    // Шаг 1: ПРГ:Тс-1.2 - расчет X
    step = 1;
    status = Pred_Step1_CalcX(params);
    if (status != PRED_STATUS_OK) goto rollback;
    
    // Шаг 2: ПРГ:Тс-1.3 - обновление X'
    step = 2;
    status = Pred_Step2_UpdateXPrime(params);
    if (status != PRED_STATUS_OK) goto rollback;
    
    // Шаг 3: ПРГ:Тс-1.4 - расчет P
    step = 3;
    status = Pred_Step3_CalcP(params);
    if (status != PRED_STATUS_OK) goto rollback;
    
    // Шаг 4: ПРГ:Тс-1.5 - обновление P'
    step = 4;
    status = Pred_Step4_UpdatePPrime(params);
    if (status != PRED_STATUS_OK) goto rollback;
    
    // Шаг 5: ПРГ:Тс-1.6 - формирование признака X
    step = 5;
    status = Pred_Step5_SetXFlag(params);
    if (status != PRED_STATUS_OK) goto rollback;
    
    // Шаг 6: ПРГ:Тс-1.7 - формирование признака P
    step = 6;
    status = Pred_Step6_SetPFlag(params);
    if (status != PRED_STATUS_OK) goto rollback;
    
    // Успех - очистка резервных копий
    Pred_FreeBackup(params);
    return PRED_STATUS_OK;
    
rollback:
    /* ПРГ:Тс-1.8 - откат всех изменений */
    Pred_Rollback(params);
    Pred_FreeBackup(params);
    return PRED_STATUS_SEQUENCE_FAIL;
}
