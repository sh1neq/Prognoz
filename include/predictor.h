#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>
#include <stdbool.h>

/* ============================================================================
   Требование ПРГ:Ту-1 - Общий состав входных данных
   ============================================================================ */

// Коды возврата (ПРГ:Ту-1.1, ПРГ:Ту-5.6)
typedef uint32_t Pred_Status;

#define PRED_STATUS_OK                  0x00000000U
#define PRED_STATUS_NULL_PTR            0x00000001U
#define PRED_STATUS_INVALID_DIM         0x00000002U
#define PRED_STATUS_SIZE_MISMATCH       0x00000003U
#define PRED_STATUS_MATRIX_SINGULAR     0x00000004U
#define PRED_STATUS_ALLOC_FAIL          0x00000005U
#define PRED_STATUS_SEQUENCE_FAIL       0x00000006U

// Структура параметров (ПРГ:Ту-1.1, ПРГ:Тр-1.1)
typedef struct {
    // Размерности
    uint32_t n;                      // Размерность вектора состояния
    uint32_t p;                      // Размерность вектора входных шумов
    
    // Входные параметры
    double *X_prime;                 // Оценка вектора состояния X'
    double *P_prime;                 // Оценка матрицы ковариации P'
    const double *F;                 // Переходная матрица F (n x n)
    const double *G;                 // Переходная матрица G (n x p)
    const double *Q;                 // Ковариационная матрица Q (p x p)
    
    // Выходные параметры
    double *X;                       // Прогнозируемый вектор X
    double *P;                       // Прогнозируемая матрица P
    
    // Признаки обновления (ПРГ:Тр-1.1)
    bool X_updated;
    bool P_updated;
    
    // Резервные копии для отката (ПРГ:Тс-1.8)
    double *X_prime_backup;
    double *P_prime_backup;
    double *X_backup;
    double *P_backup;
    bool backup_allocated;
} Pred_Params;

/* ============================================================================
   ПРГ:Ту-1 - Функции валидации
   ============================================================================ */
Pred_Status Pred_ValidateParams(const Pred_Params *params);

/* ============================================================================
   ПРГ:Ту-2, Ту-3, Ту-4 - Функции прогнозирования
   ============================================================================ */
Pred_Status Pred_Step1_CalcX(Pred_Params *params);
Pred_Status Pred_Step3_CalcP(Pred_Params *params);

/* ============================================================================
   ПРГ:Тр-1 - Функции управления признаками
   ============================================================================ */
void Pred_SetXUpdated(Pred_Params *params);
void Pred_SetPUpdated(Pred_Params *params);
bool Pred_IsXUpdated(const Pred_Params *params);
bool Pred_IsPUpdated(const Pred_Params *params);
void Pred_ResetFlags(Pred_Params *params);

/* ============================================================================
   ПРГ:Тс-1 - Функции последовательности и отката
   ============================================================================ */
Pred_Status Pred_Step2_UpdateXPrime(Pred_Params *params);
Pred_Status Pred_Step4_UpdatePPrime(Pred_Params *params);
Pred_Status Pred_Step5_SetXFlag(Pred_Params *params);
Pred_Status Pred_Step6_SetPFlag(Pred_Params *params);

Pred_Status Pred_AllocBackup(Pred_Params *params);
void Pred_FreeBackup(Pred_Params *params);
Pred_Status Pred_CreateBackup(Pred_Params *params);
void Pred_Rollback(Pred_Params *params);

Pred_Status Pred_RunSequence(Pred_Params *params);

#endif // PREDICTOR_H
